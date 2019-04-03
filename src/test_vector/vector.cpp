

#include "vector.h"

//using namespace std;

std::vector<int> valList1;// = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

void ShowVec1(const std::vector<int>& valList)
{
    valList1.push_back(0);
    int count = valList.size();
    for (int i = 0; i < count;i++)
    {
        std::cout << valList[i] << std::endl;
    }
}

void ShowVec2(const std::vector<int>& valList)
{
    for (std::vector<int>::const_iterator iter = valList.cbegin(); iter != valList.cend(); iter++)
    {
        std::cout << (*iter) << std::endl;
    }
}

void ShowVec3(const std::vector<int>& valList)
{
    for (auto val : valList)
    {
        std::cout << val << std::endl;
    }
}
