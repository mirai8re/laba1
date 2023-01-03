
#pragma once
#include <mutex>
#include <string>
#include <vector>

class SortBase
{
protected:
  std::mutex _outputMutex;
  std::string _outputPath;

public:
  SortBase(std::string &output);
  virtual ~SortBase() = default;
  virtual void SortLine(std::string &currentLine) = 0;

protected:
  std::string PrepareOutputString(const std::vector<int8_t> &sortedVector) const;
  std::vector<int8_t> ExtractLineData(std::string &&currentLine) const;
  void SaveOutputString(std::string &&formattedOutput);
};
