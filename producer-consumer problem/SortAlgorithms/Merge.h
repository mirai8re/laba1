#pragma once
#include "SortBase.h"
class Merge : public SortBase
{
public:
  Merge::Merge(std::string &output);
  Merge::~Merge() = default;
  void SortLine(std::string &currentLine) override;

private:
std::vector<int8_t> MergeSorted(std::vector<int8_t> &leftSide, std::vector<int8_t> &rightSide);
std::vector<int8_t> MergeStep(std::vector<int8_t> &target);
};
