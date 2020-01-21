#include <iostream>
#include <fstream>
#include "FanoAlgorithm.h"
#include "Archive.h"

using namespace std;

//remove using namespace
//use const and references
int main(int argc, char* argv[]){
    //Что содержит argv: argv[0] - имя программы, argv[1] действие(e или d), argv[2] - исходный файл, argv[3] - получаемый файл
	if (argc == 4){
        Archive archive(new FanoAlgorithm());
        if (argv[1][0] == 'e'){
            //Кодирование (encoding)
            archive.encodeFromFile(argv[2], argv[3]);
        }
        else if (argv[1][0] == 'd'){
            //Расшифровка (decoding)
            archive.decodeToFile(argv[2], argv[3]);
        }
		system("pause");
	}
    else cout << "Неверное число параметров";
	return 0;
}