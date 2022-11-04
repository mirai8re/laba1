#include <iostream>
#include "threadParams.h"
#include "windows.h"
#include "errors.h"

HANDLE* hThreads;
HANDLE hStartAllEvent;
HANDLE* hDeadlocks;
HANDLE** hAnswers;

CRITICAL_SECTION csMarkElement;

DWORD WINAPI markerThread(void* args)
{
    threadParams* params = static_cast<threadParams*>(args);
    int* arr = params->arr;
    int n = params->n;
    int id = params->id;
    int nMarkedElements = 0;
    int num;
    srand(id);

    WaitForSingleObject(hStartAllEvent, INFINITE);

    while (true)
    {
        num = rand() % n;
        EnterCriticalSection(&csMarkElement);
        if (arr[num] == 0)
        {
            ++nMarkedElements;
            Sleep(5);
            arr[num] = id;
            Sleep(5);
            LeaveCriticalSection(&csMarkElement);

        }
        else
        {
            std::cout << "[marker:" << id << "] "
                      << "nMarkedElements = " << nMarkedElements
                      << "; failed with index " << num << std::endl;
            LeaveCriticalSection(&csMarkElement);

            SetEvent(hDeadlocks[id - 1]);
            DWORD answer = WaitForMultipleObjects(2, hAnswers[id - 1], FALSE, INFINITE);
            if (answer == WAIT_FAILED)
            {
                std::cout<<"Wait function failed";
                return -1;
            }
            switch (answer - WAIT_OBJECT_0){
                case 0:
                    continue;
                case 1:
                    EnterCriticalSection(&csMarkElement);
                    for (int i = 0; i < n; i++)
                    {
                        if (arr[i] == id) arr[i] = 0;
                    }
                    LeaveCriticalSection(&csMarkElement);
                    return 1;
                default:
                    return 2;
            }
        }
    }
}

template<typename T>
void outArr(T* arr, int n)
{
    for (int i = 0; i < n; i++)
    {
        std::cout << arr[i] << ' ';
    }
}

void cleanHandles(int totalThreads)
{
    CloseHandle(hStartAllEvent);
    for (int i = 0; i < totalThreads; i++){
        CloseHandle(hThreads[i]);
        CloseHandle(hDeadlocks[i]);
        CloseHandle(hAnswers[i][0]);
        CloseHandle(hAnswers[i][1]);
    }
}

int main()
{
    int totalThreads;
    int runningThreads;
    int nArr;
    int* arr;
    threadParams* params;
    bool* terminatedFlags;

    std::cout << "Enter number of threads to create : ";
    std::cin >> totalThreads;
    std::cout << "Enter size of array : ";
    std::cin >> nArr;

    arr = new int[nArr];

    hThreads = new HANDLE[totalThreads];
    params = new threadParams[totalThreads];
    terminatedFlags = new bool[totalThreads];

    for (int i = 0; i < nArr; ++i) arr[i] = 0;
    for (int i = 0; i < totalThreads; ++i) terminatedFlags[i] = false;

    if (!hThreads)
    {
        std::cout<<"Couldn't allocate resources";
        std::cout<<"Out of memory!";
    }

    hStartAllEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    for (int i = 1; i <= totalThreads; ++i)
    {
        params[i - 1] = threadParams(i, arr, nArr);
        hThreads[i - 1] = CreateThread(
                NULL,
                0,
                markerThread,
                &params[i - 1],
                0,
                NULL
        );
        if (!hThreads[i - 1])
        {
            std::cout<<"Creating marker thread";
            return -1;
        }
    }

    InitializeCriticalSection(&csMarkElement);

    hAnswers = new HANDLE*[totalThreads];
    hDeadlocks = new HANDLE[totalThreads];
    for (int i = 0; i < totalThreads; i++)
    {
        hDeadlocks[i] = CreateEvent(NULL, TRUE, FALSE, NULL);

        hAnswers[i] = new HANDLE[2];
        hAnswers[i][0] = CreateEvent(NULL, FALSE, FALSE, NULL);
        hAnswers[i][1] = CreateEvent(NULL, FALSE, FALSE, NULL);
    }

    SetEvent(hStartAllEvent);
    runningThreads = totalThreads;
    while (runningThreads)
    {
        WaitForMultipleObjects(totalThreads, hDeadlocks, TRUE, INFINITE);
        std::cout << "Array : ";
        outArr(arr, nArr);
        std::cout << std::endl;

        int idToTerminate;
        bool terminateOk = false;

        do {
            std::cout << "Enter thread id to terminate ( [1, totalAmountOfThreads] ) : ";
            std::cin >> idToTerminate;
            if (idToTerminate < 1 || idToTerminate > totalThreads)
            {
                std::cout << "Incorrect index!" << std::endl;
            }
            else if (terminatedFlags[idToTerminate - 1])
            {
                std::cout << "Thread is already terminated!" << std::endl;
            }
            else
            {
                SetEvent(hAnswers[idToTerminate - 1][1]);
                WaitForSingleObject(hThreads[idToTerminate - 1], INFINITE);
                terminateOk = true;
                terminatedFlags[idToTerminate - 1] = true;
                --runningThreads;

                std::cout << "Array : ";
                outArr(arr, nArr);
                std::cout << std::endl;
            }
        } while (!terminateOk);
        for (int i = 0; i < totalThreads; ++i)
        {
            if (!terminatedFlags[i])
            {
                ResetEvent(hDeadlocks[i]);
                SetEvent(hAnswers[i][0]);
            }
        }
    }

    std::cout << "There are no running threads left..." << std::endl;

    DeleteCriticalSection(&csMarkElement);
    cleanHandles(totalThreads);
    for (int i = 0; i < totalThreads; i++)
    {
        delete[] hAnswers[i];
    }
    delete[] terminatedFlags;
    delete[] hAnswers;
    delete[] hThreads;
    delete[] hDeadlocks;
    delete[] params;
    delete[] arr;

    std::cout << "Done cleaning!" << std::endl;
    system("pause");
    return 0;
}