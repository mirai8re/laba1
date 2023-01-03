#include "Merge.h"
using namespace std;

Merge::Merge(string &output)
    : SortBase(output)
{
}

void Merge::SortLine(string &currentLine)
{
    auto lineData = ExtractLineData(move(currentLine));
    lineData = MergeStep(lineData);
    SaveOutputString(PrepareOutputString(lineData));
}

vector<int8_t> Merge::MergeSorted(vector<int8_t> &leftSide, vector<int8_t> &rightSide)
{
    vector<int8_t> result;
    size_t leftRunner = 0;
    size_t rightRunner = 0;
    while (leftRunner < leftSide.size() && rightRunner < rightSide.size())
    {
        if (leftSide.at(leftRunner) <= rightSide.at(rightRunner))
        {
            result.push_back(leftSide.at(leftRunner));
            leftRunner++;
        }
        else
        {
            result.push_back(rightSide.at(rightRunner));
            rightRunner++;
        }
    }

    while (leftRunner < leftSide.size())
    {
        result.push_back(leftSide.at(leftRunner));
        leftRunner++;
    }

    while (rightRunner < rightSide.size())
    {
        result.push_back(rightSide.at(rightRunner));
        rightRunner++;
    }
    return result;
}

vector<int8_t> Merge::MergeStep(vector<int8_t> &target)
{
    if (target.size() == 1)
    {
        return target;
    }
    else
    {
        auto midPoint = target.size() / 2;
        vector<int8_t> leftSide(target.begin(), target.begin() + midPoint);
        vector<int8_t> rightSide(target.begin() + midPoint, target.end());
        leftSide = MergeStep(leftSide);
        rightSide = MergeStep(rightSide);
        return MergeSorted(leftSide, rightSide);
    }
}
