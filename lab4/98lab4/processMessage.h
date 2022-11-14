//
// Created by anya on 10/29/2022.
//

#ifndef INC_98LAB4_PROCESSMESSAGE_H
#define INC_98LAB4_PROCESSMESSAGE_H

#endif //INC_98LAB4_PROCESSMESSAGE_H

#include <string>
#include <cstring>
#include <fstream>

class ProcessMessage{
public:
    ProcessMessage() : message() {};
    ProcessMessage(std::string& str)
    {
        strcpy(message, str.c_str());
    }

    char* getString()
    {
        return message;
    }

    friend std::fstream& operator<<(std::fstream& ofs, const ProcessMessage& processMessage);
    friend std::fstream& operator>>(std::fstream& ifs, ProcessMessage& processMessage);

private:
    char message[20];
};

std::fstream &operator<<(std::fstream &ofs, const ProcessMessage &processMessage)
{
    ofs.write((const char*)&processMessage, sizeof(ProcessMessage));
    return ofs;
}

std::fstream &operator>>(std::fstream &ifs, ProcessMessage &processMessage)
{
    ifs.read((char*)& processMessage, sizeof(ProcessMessage));
    return ifs;
}
