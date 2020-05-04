#include "QTFileReplacerV1.h"
#include <QtWidgets/QApplication>

#include "File.h"

int main(int argc, char *argv[])
{
	
	File file("D:\\Users\\Paul-André\\Documents\\exercices programmation\\C++ VS2019\\ExerciceLibre\\File_test\\fichier.txt");


	QApplication a(argc, argv);
	QTFileReplacerV1 w;
	w.show();
	return a.exec();
}
