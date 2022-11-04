//
// Created by anya on 10/20/2022.
//

#include <iostream>
#include "windows.h"

#ifndef UNTITLED1_ERRORS_H
#define UNTITLED1_ERRORS_H

#endif //UNTITLED1_ERRORS_H

#define ERROR_PREFIX "[ERROR]"

std::string GetLastErrorAsString()
{
    DWORD errorId = GetLastError();

    if (errorId == 0)
    {
        return std::string();
    }

    LPSTR msgBuf = NULL;
    int size = FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            errorId,
            MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
            (LPSTR)&msgBuf,
            0,
            NULL
    );

    std::string msg(msgBuf, size);

    LocalFree(msgBuf);

    return msg;
}

