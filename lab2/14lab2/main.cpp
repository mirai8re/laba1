#include <iostream>
#include "errors.h"
#include "minMaxThread.h"
#include "averageThread.h"


void createMinMaxThread(double* arr, int n, returnValues& ret){
    minMaxThread minMax;
    std::thread thread(minMax, arr, n, std::ref(ret));
    thread.join();
}

void createAverageThread(double* arr, int n, returnValues& ret){
    averageThread average;
    std::thread thread(average, arr, n, std::ref(ret));
    thread.join();
}

template<typename T>
void createThread(double* arr, int n, returnValues& ret){
    std::thread thread(T(), arr, n, std::ref(ret));
    thread.join();
}

template<typename T>
void outArr(T* arr, int n)
{
    for (int i = 0; i < n; i++)
    {
        std::cout << arr[i] << ' ';
    }
}


int main() {
    std::cout << "Enter n: ";
    int n;
    std::cin >> n;
    double* arr = new double[n];

    if (!arr)
    {
        std::cout <<  "Couldn't allocate resources", "Out of memory!";
        return 0;
    }

    std::cout << "Enter array elements : ";
    for (int i = 0; i < n; ++i)
    {
        std::cin >> arr[i];
    }

    returnValues ret;
    createThread<minMaxThread>(arr, n, ret);
    createThread<averageThread>(arr, n, ret);

    double min = ret.min;
    double max = ret.max;
    double average = ret.average;

    for (int i = 0; i < n; i++){
        if (arr[i] == min || arr[i] == max) arr[i] = average;
    }

    std::cout << "Final array is : ";
    outArr(arr, n);
    std::cout << std::endl;

    delete[] arr;
    return 0;
}
