/**************************************************************************
**
** creator: Paul-Andr� Bisson
**
** File: main.cpp
**
** Contact: None
**
** GNU Lesser General Public License Usage
**
** This file may be used under the terms of the GNU Lesser General Public
** License version 3 as published by the Free Software Foundation and
** appearing in the file LICENSE included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 3 requirements will be met:
** https://www.gnu.org/licenses/gpl-3.0.fr.html.
**
** 
**************************************************************************/

#include "QTFileReplacerV1.h"
#include <QtWidgets/QApplication>

#include "File.h"

int main(int argc, char *argv[])
{

	QApplication a(argc, argv);
	QTFileReplacerV1 w;
	w.show();
	return a.exec();
}
