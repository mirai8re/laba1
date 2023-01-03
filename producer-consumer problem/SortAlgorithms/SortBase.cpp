#include "SortBase.h"
#include <cctype>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>
using namespace std;

SortBase::SortBase(string &output)
    : _outputPath{output}
{
}

string SortBase::PrepareOutputString(const vector<int8_t> &sortedVector) const
{
  stringstream dataBuffer;
  for (auto &&currentChar : sortedVector)
  {
    dataBuffer << currentChar << ",";
  }
  string result{dataBuffer.str()};
  result.pop_back();
  result.append("\n");
  return result;
}

vector<int8_t> SortBase::ExtractLineData(string &&currentLine) const
{
  vector<int8_t> buffer;
  for (char currentChar : currentLine)
  {
    if (isdigit(currentChar))
    {
      buffer.push_back(currentChar);
    }
    else if (isspace(currentChar))
    {
      this_thread::sleep_for(chrono::seconds(1));
    }
  }

  return buffer;
}

void SortBase::SaveOutputString(string &&formattedOutput)
{
  lock_guard<mutex> fileGuard(_outputMutex);
  ofstream outputFile(_outputPath, ios_base::app | ios_base::out);
  outputFile << formattedOutput;
}
