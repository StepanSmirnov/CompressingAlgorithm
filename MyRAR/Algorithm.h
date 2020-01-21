#pragma once
#include <string>
#include <map>
#include <vector>

class Algorithm{
    //Абстрактный класс для создания различных алгоритмов
public:
    virtual std::map<std::string, std::vector<bool>> makeBinCodes(const std::map<std::string, int> &frequency) = 0;
    virtual ~Algorithm(){};
};

