#include <iostream>
#include <fstream>
#include "FanoAlgorithm.h"
#include "Archive.h"

using namespace std;

//remove using namespace
//use const and references
int main(int argc, char* argv[]){
    //��� �������� argv: argv[0] - ��� ���������, argv[1] ��������(e ��� d), argv[2] - �������� ����, argv[3] - ���������� ����
	if (argc == 4){
        Archive archive(new FanoAlgorithm());
        if (argv[1][0] == 'e'){
            //����������� (encoding)
            archive.encodeFromFile(argv[2], argv[3]);
        }
        else if (argv[1][0] == 'd'){
            //����������� (decoding)
            archive.decodeToFile(argv[2], argv[3]);
        }
		system("pause");
	}
    else cout << "�������� ����� ����������";
	return 0;
}