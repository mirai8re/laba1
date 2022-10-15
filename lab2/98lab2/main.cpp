#include <iostream>
#include "threadParams.h"
#include "windows.h"


DWORD WINAPI minMaxThread(void* args)
{
    threadParams* params = static_cast<threadParams*>(args);
    double* arr = params->arr;
    int n = params->n;
    int min = INT_MAX;
    double max = INT_MIN;

    for (int i = 0; i < n; ++i)
    {
        if (arr[i] < min) min = arr[i];
        if (arr[i] > max) max = arr[i];
        Sleep(7);
    }

    params->ret.min = min;
    params->ret.max = max;

    printf("min is : %d /n", static_cast<int>(min));
    printf("max is : %d /n", static_cast<int>(max));

    return 0;
}

DWORD WINAPI averageThread(void* args)
{
    threadParams* params = static_cast<threadParams*>(args);
    double* arr = params->arr;
    int n = params->n;
    double average = 0.0;

    for (int i = 0; i < n; ++i)
    {
        average += arr[i];
        Sleep(12);
    }
    if (n != 0) average /= n;
    printf("Average is :  %d /n", static_cast<int>(average));
    params->ret.average = average;

    return 0;
}

bool createMinMax(threadParams* params){
    HANDLE hThread;
    DWORD IDThread;

    hThread = CreateThread(
            NULL,
            0,
            minMaxThread,
            static_cast<void*>(params),
            0,
            &IDThread
    );
    if (hThread == NULL)
    {
        printf("Creating \"minMax\" thread   /n");

        return false;
    }

    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
    return true;
}

bool createAverage(threadParams* params){
    HANDLE hThread;
    DWORD IDThread;

    hThread = CreateThread(
            NULL,
            0,
            averageThread,
            static_cast<void*>(params),
            0,
            &IDThread
    );
    if (hThread == NULL)
    {
        printf("Creating \"average\" thread   /n");
        return false;
    }

    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
    return true;
}

template<typename T>
void outArr(T* arr, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf(reinterpret_cast<const char *>(static_cast<int>(arr[i])));

    }
}

int main()
{

    printf("Enter n: /n");
    int n;
    scanf_s("%d", &n);
    double* arr = new double[n];

    if (!arr)
    {
        printf("Couldn't allocate resources ");
        return 0;
    }
    printf("Enter array elements : ");
    for (int i = 0; i < n; ++i)
    {
        std::cin >> arr[i];
    }

    threadParams* params = new threadParams(arr, n);
    if (!params){
        printf("Couldn't allocate resources ");
        return 0;
    }

    createMinMax(params);
    createAverage(params);

    double min = params->ret.min;
    double max = params->ret.max;
    double average = params->ret.average;

    for (int i = 0; i < n; i++){
        if (arr[i] == min || arr[i] == max) arr[i] = average;
    }


    printf("Final array is : ");
    outArr(arr, n);
    printf("/n");

    delete params;
    delete[] arr;
    return 0;
}