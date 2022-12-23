#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "errors.h"
#include "employee.h"
#include "windows.h"
#include "entryState.h"

int employeesCount;
int clientsCount;
std::string binaryFileName;

CRITICAL_SECTION csFileWrite;
CRITICAL_SECTION csBlockedFlags;

HANDLE hStartAllEvent;

std::vector<HANDLE> hReadyEvents;
std::vector<HANDLE> hServerThreads;
std::vector<EntryState> entryStates;

void prepareBinaryFile()
{
    std::fstream fout(binaryFileName.c_str(), std::ios::binary | std::ios::out);
    std::ostringstream prefix;
    for (int i = 0; i < employeesCount; ++i)
    {
        Employee emp;
        prefix << '[' << i + 1 << "] ";
        std::cout << prefix.str() << "ID : ";
        std::cin >> emp.id;
        std::cout << prefix.str() << "Name : ";
        std::cin >> emp.name;
        std::cout << prefix.str() << "Working hours : ";
        std::cin >> emp.hours;

        fout << emp;

        prefix.str("");
        prefix.clear();
    }
}

void printBinaryFile()
{
    Employee emp;
    std::fstream fs(binaryFileName.c_str(), std::ios::binary | std::ios::in);
    for (int i = 0; i < employeesCount; ++i)
    {
        fs >> emp;
        emp.print(std::cout);
        std::cout << std::endl;
    }
}

int findEmployeeInFile(int id)
{
    std::fstream fs(binaryFileName.c_str(), std::ios::binary | std::ios::in);
    Employee emp;
    for (int i = 0; i < employeesCount; ++i)
    {
        fs >> emp;
        if (emp.id == id)
        {
            return i;
        }
    }
    return -1;
}

bool createClient(int clientID)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    std::ostringstream args;
    args << clientID;

    bool createOk = CreateProcess( clientExeName.c_str(),
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
        logErrorWin32("CreateProcess failed!");
    }

    CloseHandle(pi.hThread);
    return createOk;
}

bool prepareSyncObjects()
{
    InitializeCriticalSection(&csBlockedFlags);
    InitializeCriticalSection(&csFileWrite);
    hStartAllEvent = CreateEvent(NULL, TRUE, FALSE, startAllEventName.c_str());
    entryStates = std::vector<EntryState>(clientsCount);
    if (hStartAllEvent == NULL)
    {
        logErrorWin32("CreateEvent failed!");
        return false;
    }
    return true;
}

bool prepareWin32()
{
    bool syncObjPrepared = prepareSyncObjects();
    bool clientsPrepared = true;

    hReadyEvents = std::vector<HANDLE>(clientsCount);

    std::ostringstream handleName;
    for (int i = 0; i < clientsCount; ++i)
    {
        handleName << creatorName << "_ready_" << i;
        hReadyEvents[i] = CreateEvent(NULL, TRUE, FALSE, &handleName.str()[0]);
        clientsPrepared &= createClient(i);

        handleName.str("");
        handleName.clear();
    }
    return syncObjPrepared && clientsPrepared;
}

void cleanWin32()
{
    for (int i = 0; i < clientsCount; ++i)
    {
        CloseHandle(hReadyEvents[i]);
    }

    CloseHandle(hStartAllEvent);
    DeleteCriticalSection(&csFileWrite);
    DeleteCriticalSection(&csBlockedFlags);
}

DWORD WINAPI processingThread(LPVOID params)
{
    HANDLE hPipe = params;
    bool readSuccess = false;
    bool sendSuccess = false;
    char message[MESSAGE_MAX_SIZE];
    int id;
    Employee emp;

    while (true)
    {
        DWORD cbRead;
        DWORD cbWritten;
        bool allowModify = false;

        readSuccess = ReadFile(hPipe, message, MESSAGE_MAX_SIZE, &cbRead, NULL);
        if (!readSuccess)
        {
            logErrorWin32("Failed to read the message!");
            break;
        }

        id = std::atoi(message + 2);
        EnterCriticalSection(&csFileWrite);
        int posInFile = findEmployeeInFile(id);
        if (posInFile != -1)
        {
            std::fstream fs(binaryFileName.c_str(), std::ios::binary | std::ios::in);
            fs.seekg(posInFile * sizeof(Employee));
            fs >> emp;
        }
        else emp.id = -1;
        LeaveCriticalSection(&csFileWrite);
        EnterCriticalSection(&csBlockedFlags);
        switch (message[0])
        {
            case 'r':
            {
                if (posInFile != -1) entryStates[posInFile] = IS_BEING_READ;
                break;
            }
            case 'w':
            {
                if(posInFile != -1)
                {
                    if (entryStates[posInFile] == IS_BEING_READ) emp.clear();
                    else
                    {
                        entryStates[posInFile] = IS_BEING_MODIFIED;
                        allowModify = true;
                    }
                }
                break;
            }
            case 'c':
            {
                if (posInFile != -1) entryStates[posInFile] = IS_FREE;
                break;
            }
        }
        LeaveCriticalSection(&csBlockedFlags);
        if (message[0] == 'c') continue;
        sendSuccess = WriteFile(hPipe, &emp, sizeof(Employee), &cbWritten, NULL);
        if (!sendSuccess)
        {
            logErrorWin32("Failed to send the message!");
            break;
        }

        if (message[0] == 'w' && allowModify)
        {
            readSuccess = ReadFile(hPipe, &emp, sizeof(Employee), &cbRead, NULL);
            if (!readSuccess)
            {
                logErrorWin32("Failed to read client's answer");
                break;
            }
            if (posInFile != - 1)
            {
                EnterCriticalSection(&csFileWrite);
                std::fstream fs(binaryFileName.c_str(), std::ios::binary | std::ios::out);
                fs.seekp(posInFile * sizeof(Employee));
                fs << emp;
                fs.close();
                LeaveCriticalSection(&csFileWrite);
                EnterCriticalSection(&csBlockedFlags);
                entryStates[posInFile] = IS_FREE;
                LeaveCriticalSection(&csBlockedFlags);
            }
        }
    }
    FlushFileBuffers(hPipe);
    DisconnectNamedPipe(hPipe);
    CloseHandle(hPipe);
}

bool makeConnection()
{
    hServerThreads = std::vector<HANDLE>(clientsCount);
    HANDLE hPipe;
    for (int i = 0; i < clientsCount; ++i)
    {
        hPipe = CreateNamedPipe(
                pipeName.c_str(),
                PIPE_ACCESS_DUPLEX,
                PIPE_TYPE_MESSAGE |
                PIPE_READMODE_MESSAGE |
                PIPE_WAIT,
                PIPE_UNLIMITED_INSTANCES,
                0,
                0,
                0,
                NULL);

        if (hPipe == INVALID_HANDLE_VALUE)
        {
            logErrorWin32("CreateNamedPipe failed!");
            return false;
        }

        bool isConnected = ConnectNamedPipe(hPipe, NULL) != 0 || (GetLastError() == ERROR_PIPE_CONNECTED);
        if (isConnected)
        {
            hServerThreads[i] = CreateThread(NULL, 0, processingThread, (LPVOID) hPipe, 0, NULL);
            if (hServerThreads[i] == NULL)
            {
                logErrorWin32("CreateThread for server failed!");
                return false;
            }
        }
        else
        {
            CloseHandle(hPipe);
        }
    }
    return true;
}

int main()
{
    std::cout << "Enter the number of employees : ";
    std::cin >> employeesCount;
    std::cout << "Enter the number of clients : ";
    std::cin >> clientsCount;
    std::cout << "Enter binary file's name : ";
    std::cin >> binaryFileName;

    consoleMessage("Creating binary file.");
    consoleMessage("Please enter all entries : ");
    prepareBinaryFile();

    bool prepareWin32OK = prepareWin32();
    if (!prepareWin32OK)
    {
        consoleMessage("Something went wrong while preparing Win32 stuff!");
        return -1;
    }
    else
    {
        consoleMessage("Successfully prepared Win32 stuff.");
    }

    consoleMessage("Waiting for all clients to be ready...");
    WaitForMultipleObjects(clientsCount, hReadyEvents.data(), TRUE, INFINITE);
    consoleMessage("All clients are ready. Starting...");
    SetEvent(hStartAllEvent);

    bool connectionOK = makeConnection();
    if (!connectionOK)
    {
        consoleMessage("Something went wrong while establishing server-client connections!");
        return -1;
    }
    else
    {
        consoleMessage("All clients connected successfully.");
    }
    WaitForMultipleObjects(clientsCount, hServerThreads.data(), TRUE, INFINITE);
    consoleMessage("Final file : ");
    printBinaryFile();
    cleanWin32();

    consoleMessage("Bye!");
    system("pause");
    return 0;
}