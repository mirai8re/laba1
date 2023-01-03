
#pragma once
#include "SortBase.h"

class Bubble : public SortBase
{
public:
  Bubble(std::string &output);
  ~Bubble() = default;
  void SortLine(std::string &currentLine) override;
};
