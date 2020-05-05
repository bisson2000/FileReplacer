/**************************************************************************
**
** creator: Paul-André Bisson
**
** File: File.h
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

#pragma once

#ifndef FILE_H
#define FILE_H


#include <QObject>
#include <QString>
#include <Qt>

#include <fstream>
#include <filesystem>
#include <sstream>
#include <algorithm>
#include <string>

#include <unordered_map>

class File : public QObject {

	Q_OBJECT

public:

	File(const std::string& path = "", const std::string& toReplace = "", const std::string& replacement = "");
	~File();

	//setters
	void setFileName(const QString& fileName);
	void setPath(const std::string& path);
	void setToReplace(const std::string& toReplace);
	void setReplacement(const std::string& replacement);

	//getters
	QString getFileName() const;
	std::string getPath() const;
	std::string getToReplace() const;
	std::string getReplacement() const;

	//Methods
	std::string escapedCaractersConversion(const std::string& caracter);
	std::string fileNameToPath(const QString& fileName) const;
	std::string replaceStrings(const std::string& line, const std::string& toReplace, const std::string& replacement);
	void copyFile();
	void replaceFile();

public slots:


signals:

	//warning for copy
	void copyWarning();
	//replacement success
	void replacementSuccess();

private:

	int searchNumberInPath() const;


	//void copyFile();

	QString fileName_;
	std::string oldPath_;
	std::string workingPath_;
	std::string toReplace_;
	std::string replacement_;
	std::fstream workingFile_;
};



#endif //FILE_H