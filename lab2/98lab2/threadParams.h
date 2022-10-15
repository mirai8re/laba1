//
// Created by anya on 9/13/2022.
//

#include <cstdlib>

#ifndef INC_98LAB2_THREADPARAMS_H
#define INC_98LAB2_THREADPARAMS_H

#endif //INC_98LAB2_THREADPARAMS_H

struct returnValues{
    returnValues() : min(INT_MIN), max(INT_MAX), average(0.0) {}

    double min;
    double max;
    double average;
};

struct threadParams{
    threadParams() : arr(NULL), n(-1), ret() {}
    threadParams(double *arr, int n) : arr(arr), n(n), ret() {}

    double* arr;
    int n;
    returnValues ret;
};