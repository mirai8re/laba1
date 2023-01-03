#include "SortAlgorithms/Bubble.h"
#include "SortAlgorithms/Merge.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
using namespace std;

bool CheckFile(const string &path)
{
    bool result{true};
    int counter{};
    string currentLine{};
    ifstream inputFile(path);
    result = inputFile.is_open();

    if (!result)
    {
        cout << "Can't open the input file" << endl;
        return result;
    }

    while (getline(inputFile, currentLine) && result)
    {
        counter++;
        if (currentLine.size() > 100 || counter > 10000)
        {
            cout << "The input file is not in a valid format" << endl;
            result = false;
        }
    }

    return result;
}

int main(int argc, char **argv)
{
    if (argc == 4)
    {
        string inputFile(argv[1]);
 
        if (!CheckFile(inputFile))
        {
            return -1;
        }
        string outputFile(argv[2]);
        string sortAlgorithm(argv[3]);
        unique_ptr<SortBase> sorter{};
 
        if (sortAlgorithm.compare("bubble") == 0)
        {
            sorter = make_unique<Bubble>(outputFile);
        }
        else if (sortAlgorithm.compare("merge") == 0)
        {
            sorter = make_unique<Merge>(outputFile);
        }

        if (sorter == nullptr)
        {
            cout << "Invalid algorithm selected" << endl;
            return -1;
        }

        ifstream inputFileStream(inputFile);
        if (!inputFileStream.is_open())
        {
            cout << "Can't open the input file" << endl;
            return -1;
        }

        string currentLine;
        vector<thread> threadHolder;
        bool couldRead{true};
        while (couldRead)
        {
            for (int i = 0; (i < 4) && couldRead; i++)
            {
                couldRead = !(inputFileStream.eof() || inputFileStream.fail());
                if (couldRead)
                {
                    getline(inputFileStream, currentLine);
                    threadHolder.push_back(
                        std::thread(&SortBase::SortLine,
                                    sorter.get(),
                                    currentLine));
                }
            }
            for (size_t i = 0; i < threadHolder.size(); i++)
            {
                threadHolder.at(i).join();
            }
            threadHolder.clear();
        };
    }
    else
    {
        cout << "Invalid quantity of input parameters" << endl
             << "The usage of this software needs three params as follows:" << endl
             << "    1. Name of input file" << endl
             << "    2. Name of output file" << endl
             << "    3. Name of sorting algorithm, you can use one of this: " << endl
             << "        3.1. bubble" << endl
             << "        3.2. merge" << endl
             << "Please try running again with the correct amount of input parameters...";
    }

    return 0;
}
