#include <iostream>
#include "threadParams.h"
#include "windows.h"


DWORD WINAPI minMaxThread(void* args)
{
    threadParams* params = static_cast<threadParams*>(args);
    double* arr = params->arr;
    int n = params->n;
    double min = INT_MAX;
    double max = INT_MIN;

    for (int i = 0; i < n; ++i)
    {
        if (arr[i] < min) min = arr[i];
        if (arr[i] > max) max = arr[i];
        Sleep(7);
    }

    std::cout << "min is : " << min << std::endl;
    std::cout << "max is : " << max << std::endl;

    params->ret.min = min;
    params->ret.max = max;

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
    std::cout << "Average is : " << average << std::endl;
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
        std::cout<<"Creating \"minMax\" thread";
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
        std::cout<<"Creating \"average\" thread";
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
        std::cout << arr[i] << ' ';
    }
}

int main()
{
    std::cout << "Enter n: ";
    int n;
    std::cin >> n;
    double* arr = new double[n];

    if (!arr)
    {
        std::cout<<"Couldn't allocate resources";
        return 0;
    }

    std::cout << "Enter array elements : ";
    for (int i = 0; i < n; ++i)
    {
        std::cin >> arr[i];
    }

    threadParams* params = new threadParams(arr, n);
    if (!params)
    {
        std::cout<<"Couldn't allocate resources";
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

    std::cout << "Final array is : ";
    outArr(arr, n);
    std::cout << std::endl;

    delete params;
    delete[] arr;
    return 0;
}