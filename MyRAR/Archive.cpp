#include "Archive.h"
#include <iostream>
#include <fstream>

//Функция для частотного анализа
const std::map<std::string, int> Archive::countLetters(const std::string &s) const{
    std::map<std::string, int> result;
    for (int i = 0; i < s.size(); i++){//Смотрим текст
        result[std::string(1, s.at(i))]++;//Наращиваем счетчик текущей буквы (По умолчанию 0)
    }
    return result;
}

//Вспомогательная функция для перестановки элементов пары (было K:V, стало V:K)
template<class K, class V> std::map<V, K> Archive::turnMap(const std::map<K, V> &source) const{
    std::map<V, K> result;
    for each (std::pair<K, V> var in source){
        result.insert(std::pair<V, K>(var.second, var.first));
    }
    return result;
}

//Распределение битов в байты, чтобы потом их можно было записать в файл
char* Archive::bitsToChars(const std::vector<bool> &bits) const {
    const int n = ceil(bits.size() / 8.0);//Считаем кол-во байт
    std::vector<bool>::const_iterator it = bits.begin();
    char *result = new char[n];
    for (int i = 0; i < n; i++) {
        char byte = 0;
        for (int j = 0; j < 8; j++) {//Создаем байт из 8 бит
            byte <<= 1;//Сдвигаем цифры влево, чтобы освободить место для разряда единиц
            if (it != bits.end()) byte |= *(it++);//Устанавливаем правый крайний бит
        }
        /*Допустим вот первые 8 бит :11011010, byte=0 Вот что произойдет в цикле
        j=0, byte<<1 = 00, byte | 1 = 01
        j=1, byte<<1 = 010, byte | 1 = 011
        j=2, byte<<1 = 0110 byte | 0 = 0110
        j=3, byte<<1 = 01100 byte | 1 = 01101 и т.д.*/
        result[i] = byte;
    }
    return result;
}

//Разбиение байтов на биты,так их удобнее раскодировать, чем байты целиком
std::vector<bool> Archive::charsToBits(const char* bytes, const int n) const {
    std::vector<bool> result;
    for (int i = n - 1; i >= 0; i--) {
        char byte = bytes[i];
        for (int j = 0; j < 8; j++) {
            result.push_back(byte & 1);//Смотрим значение крайнего правого бита
            byte >>= 1;//Сдвигаем вправо, чтобы посмотреть следующий бит
            /*Пусть t=01001110
            j=0, t & 1 = 01001110 & =0, t>>1= 00100111
                         00000001
            j=1, t & 1 = 00100111 & =1, t>>1= 00010011
                         00000001
            j=2, t & 1 = 00010011 & =1, t>>1= 00001001
                         00000001
            */
        }
    }
    return result;
}

Archive::Archive(Algorithm *algorithm) : algorithm(algorithm){}
    //Сжатие строки text и запись её в файлы file.enc и file.key
void Archive::encode(const std::string &text, const std::string &outFile)const{
    std::map<std::string, int> letters = countLetters(text);
    std::ofstream key(outFile + ".key", std::ios::out);//Файл с частотами сиволов
    std::ofstream out(outFile + ".enc", std::ios::out | std::ios::binary);//Файл с сжатым текстом
    std::vector<bool> encoded;//Биты сжатого текста
    for each (std::pair<std::string, int> var in letters)
    {
        key << var.first + " " + std::to_string(var.second) + "\n";//Записываем частоты в файл
    }
    key.close();
    letters["end"] = 1;/*Добавляем символ конца файла, он в файл с частотами уже не попадет.
    При распаковке он тоже добавится в программе*/
    std::map<std::string, std::vector<bool>> codes = algorithm->makeBinCodes(letters);//Создаем таблицу кодировки
    for (int i = 0; i < text.size(); i++){//Читаем исходный текст
        std::string currChar = std::string(1, text.at(i));
        encoded.insert(encoded.end(), codes[currChar].begin(), codes[currChar].end());//Добавляем в биты код текущего символа
    }
    encoded.insert(encoded.end(), codes["end"].begin(), codes["end"].end());//Добавляем символ конца файла
    char *chars = bitsToChars(encoded);//Делаем из битов символы
    out.write(chars, ceil(encoded.size() / 8.0));//Записываем их в файл
    out.close();
}

    void Archive::encodeFromFile(const std::string &inFile, const std::string &outFile) const{
        std::ifstream in(inFile);
        std::string s;
        while (in) s += in.get();
        if (!s.empty()) {
            s.erase(--s.end());
            this->encode(s, outFile);
        }
        in.close();
    }

//Распаковка из file.enc и file.key и возврат раскодированного текста
    std::string Archive::decode(const std::string &inFile) const{
        std::ifstream in(inFile + ".enc", std::ios::in | std::ios::binary);
        std::ifstream key(inFile + ".key", std::ios::out);
    std::map<std::string, int> letters;
    std::map<std::vector<bool>, std::string>::iterator i;
    std::string s, result;
    while (key){//Считываем частоты символов из файла .key в letters
        s += key.get();
        if ((s.size() > 1) && (s.back() == '\n')){
            letters.insert(std::pair<std::string, int>(std::string(1, s.at(0)), atoi(s.substr(2).c_str())));
            s.clear();
        }
    }
    key.close();
    letters["end"] = 1;//Добавляем символ конца файла
    std::map<std::vector<bool>, std::string> codes = turnMap(algorithm->makeBinCodes(letters));//Создаем заново таблицу кодировки
    /*Можно было бы записывать в файл не частоты, а сразу таблицу кодировки, но это немного сложнее*/
    std::string code;
    while (in) code += in.get();//Читаем сжатый файл в code
    in.close();
    std::vector<bool> bits = charsToBits(code.c_str(), code.size()), help;//Преобразуем символы в ,биты
    for (std::vector<bool>::reverse_iterator it = bits.rbegin(); it != bits.rend(); it++)
    {
        help.push_back(*(it));//Обходим в цикле биты и добавляем их в help
        if ((i = codes.find(help)) != codes.end()){//Если в таблице кодировки есть код, который в help
            if (i->second == "end") break;//Если найден конец файла, завершаем распаковку
            result += i->second;//Записываем найденный символ в распакованный текст
            help.clear();//Очищаем help
        }
    }
    //cout << result;
    return result;
}

    std::string Archive::decodeToFile(const std::string &inFile, const std::string &outFile) const{
        std::ofstream out(outFile, std::ios::out | std::ios::binary);
        std::string s = this->decode(inFile);
        out << s;
        out.close();
        return s;
    }
