#include "Bubble.h"
using namespace std;

Bubble::Bubble(string &output)
    : SortBase(output)
{
}

void Bubble::SortLine(string &currentLine)
{
    auto lineData = ExtractLineData(move(currentLine));
    for (size_t i = 0; i < lineData.size(); i++)
    {
        for (size_t j = i + 1; j < lineData.size(); j++)
        {
            if (lineData.at(i) > lineData.at(j))
            {
                int8_t temp{lineData.at(j)};
                lineData[j] = lineData.at(i);
                lineData[i] = temp;
            }
        }
    }
    SaveOutputString(PrepareOutputString(lineData));
}
