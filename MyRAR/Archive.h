#pragma once
#include "Algorithm.h"
#include <memory>
class Archive{
    //Класс архиватора
public:
    Archive(Algorithm *algorithm);

    void encode(const std::string &text, const std::string &file) const;

    std::string decode(const std::string &inFile) const;

    void encodeFromFile(const std::string &inFile, const std::string &outFile) const;

    std::string decodeToFile(const std::string &inFile, const std::string &outFile) const;

private:
    //Вспомогательные функции
    const std::map<std::string, int> countLetters(const std::string &s) const;
    template<class K, class V> std::map<V, K> turnMap(const std::map<K, V> &source) const;

    char* bitsToChars(const std::vector<bool> &bits) const;

    std::vector<bool> charsToBits(const char* bytes, const int n) const;

    std::unique_ptr<Algorithm> algorithm;
};

