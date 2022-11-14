//
// Created by anya on 10/29/2022.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include "windows.h"
#include "constants.h"
#include "processMessage.h"

HANDLE hFileMutex;
HANDLE hEmptySpaceSemaphore;
HANDLE hMsgsToHandleSemaphore;

HANDLE readyEvent;

void prepareWin32(int processID)
{
    hFileMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, fileMutexName.c_str());
    hEmptySpaceSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, emptySpaceSemName.c_str());
    hMsgsToHandleSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, msgsToHandleSemName.c_str());

    std::ostringstream name;
    name << creatorName << "_ready_" << processID;
    readyEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, &name.str()[0]);
}

void cleanWin32()
{
    CloseHandle(readyEvent);
    CloseHandle(hMsgsToHandleSemaphore);
    CloseHandle(hEmptySpaceSemaphore);
    CloseHandle(hFileMutex);
}

void senderFunc(std::string& binaryFileName)
{
    std::fstream fileStream;

    while(true)
    {
        int selection;
        bool selectionOK = false;
        do
        {
            std::cout << "What to do?" << std::endl;
            std::cout << "1. Send message" << std::endl;
            std::cout << "2. Exit" << std::endl;
            std::cout << "Your selection : ";
            std::cin >> selection;
            if (selection != 1 && selection != 2) std::cout << "Wrong selection! Try again." << std::endl;
            else selectionOK = true;
        } while(!selectionOK);

        if (selection == 2) break;

        WaitForSingleObject(hEmptySpaceSemaphore, INFINITE);

        std::string messageToSend;
        std::cout << "Enter message to send : " << std::endl;
        std::cin >> messageToSend;
        ProcessMessage processMessage(messageToSend);

        WaitForSingleObject(hFileMutex, INFINITE);
        fileStream.open(binaryFileName.c_str(), std::ios::binary | std::ios::out | std::ios::app);
        fileStream << processMessage;
        fileStream.close();

        ReleaseMutex(hFileMutex);
        ReleaseSemaphore(hMsgsToHandleSemaphore, 1, NULL);
    }
}

int main(int argc, char *argv[])
{
    int processID = std::atoi(argv[0]);
    std::string binaryFileName = argv[1];

    prepareWin32(processID);

    SetEvent(readyEvent);

    senderFunc(binaryFileName);
    return 0;}
