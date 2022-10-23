#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "windows.h"
#include "employee.h"


void invokeCreator(std::string& binFilename, int n){
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    std::ostringstream osstream;
    osstream << binFilename << ' ' << n;

    bool createOk = CreateProcess( "creator.exe",
                                   &osstream.str()[0],
                                   NULL,
                                   NULL,
                                   FALSE,
                                   CREATE_NEW_CONSOLE,
                                   NULL,
                                   NULL,
                                   &si,
                                   &pi
    );
    if (!createOk)
    {
        std::cout<<"CreateProcess failed!";
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

void invokeReporter(std::string& binFilename, std::string& reportFilename, double perHour){
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    std::ostringstream osstream;
    osstream << binFilename << ' ' << reportFilename << ' ' << perHour;

    bool createOk = CreateProcess( "reporter.exe",
                                   &osstream.str()[0],
                                   NULL,
                                   NULL,
                                   FALSE,
                                   CREATE_NEW_CONSOLE,
                                   NULL,
                                   NULL,
                                   &si,
                                   &pi
    );
    if (!createOk)
    {
        std::cout<<"CreateProcess failed!";
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

int main() {
    std::string binFilename;
    std::string reportFilename;
    int n;
    double perHour;

    std::cout << "Enter binary file name:" << std::endl;
    std::cin >> binFilename;
    std::cout << "Enter num of entries:" << std::endl;
    std::cin >> n;

    invokeCreator(binFilename, n);

    std::ifstream fin(binFilename.c_str(), std::ios::binary);
    employee emp;

    std::cout << "---------------------------------" << std::endl;
    std::cout << binFilename << "'s contents" << std::endl;
    std::cout << "---------------------------------" << std::endl;

    while (fin.read((char*)&emp, sizeof(employee)))
    {
        std::cout << std::setw(12) << std::left << emp.num;
        std::cout << std::setw(11) << std::left << emp.name;
        std::cout << std::setw(12) << std::left << emp.hours << std::endl;
    }

    std::cout << "Enter report file's name:" << std::endl;
    std::cin >> reportFilename;
    std::cout << "Enter salary per hour:" << std::endl;
    std::cin >> perHour;

    invokeReporter(binFilename, reportFilename, perHour);

    fin.close();
    fin.open(reportFilename.c_str());

    std::cout << "---------------------------------" << std::endl;
    std::cout << reportFilename << "'s contents" << std::endl;
    std::cout << "---------------------------------" << std::endl;

    std::string reportLine;

    while (std::getline(fin, reportLine))
    {
        std::cout << reportLine << std::endl;
    }
    system("pause");
    return 0;
}