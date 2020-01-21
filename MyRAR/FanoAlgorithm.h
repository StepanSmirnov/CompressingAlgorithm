#pragma once
#include "Algorithm.h"
#include <set>
class FanoAlgorithm : public Algorithm{
    //Класс для алгоритма Фано
public:
    std::map<std::string, std::vector<bool>> makeBinCodes(const std::map<std::string, int> &frequency);
private:
    //Вспомогательные функции
    std::set<std::string> closestSum(const std::map<std::string, int> &frequency, const int W) const;
    static bool compare(const std::pair<std::string, int> &left,const std::pair<std::string, int> &right);
};

