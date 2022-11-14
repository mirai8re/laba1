//
// Created by anya on 10/29/2022.
//

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include "windows.h"
#include "errors.h"
#include "constants.h"
#include "processMessage.h"

HANDLE hFileMutex;
HANDLE hEmptySpaceSemaphore;
HANDLE hMsgsToHandleSemaphore;

std::vector<HANDLE> senders;
std::vector<HANDLE> sendersReadyEvents;

HANDLE createProcess(int processID, std::string& binaryFileName)
{
    std::ostringstream args;
    args << processID << ' ' << binaryFileName;

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    bool createOk = CreateProcess( senderExeName.c_str(),
                                   &args.str()[0],
                                   NULL,
                                   NULL,
                                   FALSE,
                                   CREATE_NEW_CONSOLE,
                                   NULL,
                                   NULL,
                                   &si,
                                   &pi
    );
    if (!createOk)
    {
        std::cout<<"CreateProcess failed!";
    }

    CloseHandle(pi.hThread);
    return pi.hProcess;
}

bool prepareWin32(int numberOfSenders, int msgsCapacity, std::string& binaryFileName)
{
    senders = std::vector<HANDLE>(numberOfSenders);
    sendersReadyEvents = std::vector<HANDLE>(numberOfSenders);

    hFileMutex = CreateMutex(NULL, FALSE, fileMutexName.c_str());

    hEmptySpaceSemaphore = CreateSemaphore(NULL, msgsCapacity, msgsCapacity, emptySpaceSemName.c_str());
    hMsgsToHandleSemaphore = CreateSemaphore(NULL, 0, msgsCapacity, msgsToHandleSemName.c_str());

    for (int i = 0; i < numberOfSenders; ++i)
    {
        std::ostringstream handleName;
        handleName << creatorName << "_ready_" << i;

        sendersReadyEvents[i] = CreateEvent(NULL, TRUE, FALSE, &handleName.str()[0]);
        senders[i] = createProcess(i, binaryFileName);
    }
}

void cleanWin32(int numberOfSenders)
{
    for (int i = 0; i < numberOfSenders; ++i)
    {
        CloseHandle(sendersReadyEvents[i]);
        CloseHandle(senders[i]);
    }

    CloseHandle(hMsgsToHandleSemaphore);
    CloseHandle(hEmptySpaceSemaphore);
    CloseHandle(hFileMutex);
}

void receiverFunc(int msgsCapacity, const std::string& binaryFileName)
{
    std::fstream fileStream;
    while (true)
    {
        int selection;
        bool selectionOK = false;
        do
        {
            std::cout << "What to do?" << std::endl;
            std::cout << "1. Read message" << std::endl;
            std::cout << "2. Exit" << std::endl;
            std::cout << "Your selection : ";
            std::cin >> selection;
            if (selection != 1 && selection != 2) std::cout << "Wrong selection! Try again." << std::endl;
            else selectionOK = true;
        } while(!selectionOK);

        if (selection == 2) break;

        WaitForSingleObject(hMsgsToHandleSemaphore, INFINITE);
        WaitForSingleObject(hFileMutex, INFINITE);

        fileStream.open(binaryFileName.c_str(), std::ios::binary | std::ios::in);
        ProcessMessage processMessage;
        fileStream >> processMessage;

        std::cout << "New message : " << processMessage.getString() << std::endl;

        std::vector<ProcessMessage> allFileMessages;
        while (fileStream >> processMessage)
        {
            allFileMessages.push_back(processMessage);
        }
        fileStream.close();

        fileStream.open(binaryFileName.c_str(), std::ios::binary | std::ios::out);
        for (int i = 0; i < allFileMessages.size(); ++i)
        {
            fileStream << allFileMessages[i];
        }
        fileStream.close();

        ReleaseMutex(hFileMutex);
        ReleaseSemaphore(hEmptySpaceSemaphore, 1, NULL);
    }
}

int main (int argc, char *argv[])
{
    std::string binaryFileName;
    int numberOfSenders;
    int msgsCapacity;

    std::cout << "Enter binary file's name : ";
    std::cin >> binaryFileName;
    std::cout << "Enter number of senders : ";
    std::cin >> numberOfSenders;
    std::cout << "Enter messages capacity : ";
    std::cin >> msgsCapacity;

    prepareWin32(numberOfSenders, msgsCapacity, binaryFileName);

    std::fstream fileStream (binaryFileName.c_str(), std::ios::binary | std::ios::out | std::ios::trunc);
    fileStream.close();

    WaitForMultipleObjects(numberOfSenders, sendersReadyEvents.data(), TRUE, INFINITE);

    receiverFunc(msgsCapacity, binaryFileName);

    cleanWin32(numberOfSenders);
    return 0;
}