//
// Created by anya on 10/22/2022.
//

#ifndef INC_14LAB2_AVERAGETHREAD_H
#define INC_14LAB2_AVERAGETHREAD_H


#include <thread>
#include <iostream>
#include "returnValues.h"

class averageThread {
public:
    void operator() (double* arr, int n, returnValues& ret){
        double average = 0.0;

        for (int i = 0; i < n; ++i)
        {
            average += arr[i];
            std::this_thread::sleep_for(std::chrono::milliseconds(12));
        }
        if (n != 0) average /= n;
        std::cout << "Average is : " << average << std::endl;
        ret.average = average;
    }


};


#endif //INC_14LAB2_AVERAGETHREAD_H
