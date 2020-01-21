#include "FanoAlgorithm.h"
#include <algorithm>

//Функия принимает символы и их частоты и задаёт для них двоичные коды
std::map<std::string, std::vector<bool>> FanoAlgorithm::makeBinCodes(const std::map<std::string, int> &frequency)
{
    static std::map<std::string, std::vector<bool>> result;//Почтитайте про static переменные в функциях
    static int depth = 0;//Глубина рекурсии
    depth++;
    if (depth == 1) result.clear();//Если функция вызвана не из самой себя очищаем результат, т.к. он остается от предыдущего вызова
    std::map<std::string, int>::iterator i;
    int middle = 0;
    for each (std::pair<std::string, int> var in frequency)//Считаем сумму частот O(n)
    {
        middle += var.second;
    }
    std::map<std::string, int> right, left;
    std::set<std::string> sum = closestSum(frequency, middle / 2);//Находим символы, сумма частот которых ближе к средней O(n*w)
    for each (std::pair<std::string, int> var in frequency)//O(n)
    {
        if (sum.find(var.first) != sum.end()) {
            left.insert(var);//Заносим символы из sum в левую ветку
            result[var.first].push_back(true);//Заодно заносим 1 или 0 (true, false) в код символа
        }
        else {
            right.insert(var);//А остальные в правую
            result[var.first].push_back(false);
        }
    }
    if (left.size()>1) makeBinCodes(left);//Если в левая ветка может разделиться, продолжаем дерево
    if (right.size()>1) makeBinCodes(right);//То же самое делаем с правой
    depth--;
    return result;
}

//Функция для сравнения букв по их частотам
bool FanoAlgorithm::compare(const std::pair<std::string, int> &left,const std::pair<std::string, int> &right){
    return left.second < right.second;
}

/*Задача о рюкзаке. Метод динамического программирования. Алгоритм взят из
http://neerc.ifmo.ru/wiki/index.php?title=%D0%97%D0%B0%D0%B4%D0%B0%D1%87%D0%B0_%D0%BE_%D1%80%D1%8E%D0%BA%D0%B7%D0%B0%D0%BA%D0%B5#.D0.A0.D0.B5.D0.B0.D0.BB.D0.B8.D0.B7.D0.B0.D1.86.D0.B8.D1.8F
Возвращает набор букв из frequency, сумма которых ближе всего к числу W
Стоимость предметов в данном случае равна их весу, поэтому p[k]=w[k]
*/
std::set<std::string> FanoAlgorithm::closestSum(const std::map<std::string, int> &frequency, const int W) const{
    std::vector<std::pair<std::string, int*>> A;
    std::set<std::string> result;
    A.push_back(std::pair<std::string, int*>("zero", new int[W]));
    std::fill(A.begin()->second, A.begin()->second + W, 0);//Заполняем первую строку A 0;
    for each (std::pair<std::string, int> var in frequency)
    {
        A.push_back(std::pair<std::string, int*>(var.first, new int[W]{0}));
    }//Заполняем 1-ый столбец A 0;
    std::vector<std::pair<std::string, int*>>::iterator prev = A.begin();
    for (std::vector<std::pair<std::string, int*>>::const_iterator i = ++A.cbegin(); i != A.cend(); i++){//Заполняем A
        for (int s = 1; s < W; s++){
            int temp = frequency.at(i->first);//w[k]
            if (s >= temp)
                i->second[s] = std::max(prev->second[s], prev->second[s - temp] + temp);
            else
                i->second[s] = prev->second[s];
        }
        prev++;//A[k-1]
    }
    prev = --A.end();
    int s = W-1;
    //Функцию find я переписал в виде этого цикла
    for (std::vector<std::pair<std::string, int*>>::reverse_iterator i = A.rbegin(); i != --A.rend(); i++){
        --prev;
        if (prev->second[s] != i->second[s]){
            s -= frequency.at(i->first);
            if (s <= 0) break;
            result.insert(i->first);
        }
        delete[W] i->second;
    }
    if (result.empty())//Если все частоты были больше W, как при a:10, W=5, и алгоритм не нашел ответ
        result.insert(min_element(frequency.begin(), frequency.end(), compare)->first);//Берем символ с минимальной частотой
    delete[W] A.begin()->second;
    return result;
}

