//
// Created by anya on  11/20/2022.
//

#include <cstdlib>

#ifndef LAB3__98__THREADPARAMS_H
#define LAB3__98__THREADPARAMS_H

#endif //LAB3__98__THREADPARAMS_H

struct threadParams{
    threadParams() : id(-1), arr(NULL), n(-1) {}
    threadParams(int threadId, int *arr, int n) : id(threadId), arr(arr), n(n) {}

    int id;
    int* arr;
    int n;
};