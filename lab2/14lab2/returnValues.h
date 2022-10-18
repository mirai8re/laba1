//
// Created by anya on 10/22/2022.
//

#ifndef INC_14LAB2_RETURNVALUES_H
#define INC_14LAB2_RETURNVALUES_H


#include <climits>

class returnValues {

public:
    double average;

    returnValues() : min(INT_MIN), max(INT_MAX) {}

    double min;
    double max;
};


#endif //INC_14LAB2_RETURNVALUES_H
