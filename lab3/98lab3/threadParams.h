//
// Created by anya on 10/20/2022.
//

#include <cstdlib>

#ifndef UNTITLED1_THREADPARAMS_H
#define UNTITLED1_THREADPARAMS_H

#endif //UNTITLED1_THREADPARAMS_H
struct threadParams{
    threadParams() : id(-1), arr(NULL), n(-1) {}
    threadParams(int threadId, int *arr, int n) : id(threadId), arr(arr), n(n) {}

    int id;

    int* arr;
    int n;
};
