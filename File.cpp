/**************************************************************************
**
** creator: Paul-André Bisson
**
** File: File.cpp
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

#include "File.h"

using namespace std;

/// Constructor of the file class
/// \param path         full path to the file
/// \param toReplace	string to replace in file
/// \param replacement  string that will replace the toReplace string
/// \return                     
File::File(const string& path, const string& toReplace, const string& replacement) :
	workingPath_(path),
	toReplace_(toReplace),
	replacement_(replacement)
{
}

/// Destructor of the file class
/// \param 		
/// \return   
File::~File() {
}

/// Sets the Qt filename
/// \param fileName		QString of the filename 
/// \return   
void File::setFileName(const QString& fileName)
{
	fileName_ = fileName;
}

/// Sets the proper file path
/// \param path			path to the file
/// \return  
void File::setPath(const string& path)
{
	workingPath_ = path;
}

/// Sets the toReplace string
/// \param toReplace	string with the that indicates what to replace     
/// \return  
void File::setToReplace(const string& toReplace)
{
	toReplace_ = toReplace;
}

/// Sets the replacement string
/// \param replacement	string indicating the replacement needed to be made 
/// \return  
void File::setReplacement(const string& replacement)
{
	replacement_ = replacement;
}

/// Gets the Qt filename
/// \param 
/// \return  QString	QString of the filename
QString File::getFileName() const
{
	return fileName_;
}

/// Gets the proper file path
/// \param	
/// \return	string		path to the file
string File::getPath() const
{
	return workingPath_;
}

/// Gets the toReplace string
/// \param	   
/// \return	string		string with the that indicates what to replace 
string File::getToReplace() const
{
	return toReplace_;
}

/// Gets the replacement string
/// \param 
/// \return	string		string indicating the replacement needed to be made 
string File::getReplacement() const
{
	return replacement_;
}

/// Replaces written escaped caracters with their proper strings
/// \param line			line that will be changed 
/// \return  string		line that changed
string File::escapedCaractersConversion(const std::string& line)
{
	string newLine = line;
	newLine = replaceStrings(newLine, "\\a", "\a");
	newLine = replaceStrings(newLine, "\\b", "\b");
	newLine = replaceStrings(newLine, "\\f", "\f");
	newLine = replaceStrings(newLine, "\\n", "\n");
	newLine = replaceStrings(newLine, "\\r", "\r");
	newLine = replaceStrings(newLine, "\\t", "\t");
	newLine = replaceStrings(newLine, "\\v", "\v");
	return newLine;
}

/// Searches for a number in the path in order to determine how to copy
/// \param 
/// \return  int		number found
int File::searchNumberInPath() const
{
	int number = 0;
	std::stringstream ss(workingPath_);

	while (!ss.eof()) {

		int tempNumber = 0;
		string tempString = "";
		ss >> tempString;

		//If there is a number in the string
		if (stringstream(tempString) >> tempNumber) {
			number = tempNumber;
		}
	}
	return number;
}

/// Translates the Qt fileName into a usable path
/// \param	filename	QString of the filename to be translated
/// \return  string		file path that can be openend
string File::fileNameToPath(const QString& fileName) const
{
	string path = fileName.toLocal8Bit().constData();
	std::replace(path.begin(), path.end(), '/', '\\');
	return path;
}

/// Copy file and add number to the end of it
/// Sends warning if the extension was not found
/// uses std::filesystem to copy
/// \param	
/// \return  
void File::copyFile() {

	using namespace std::filesystem;
	constexpr char DEFAULT_EXTENSION[] = ".txt";
	constexpr char DEFAULT_NUMBER[] = " 1";

	const size_t pos = workingPath_.find_last_of(".");
	const size_t extension = workingPath_.find_last_of("\\");
	oldPath_ = workingPath_;
	
	//extension not found
	if (pos == -1 || pos < extension) {
		workingPath_ += DEFAULT_NUMBER;
		workingPath_ += DEFAULT_EXTENSION;
		copy_file(oldPath_, workingPath_, copy_options::skip_existing);
		emit copyWarning();
	}

	//extension found, but is at the start of the path
	else if (pos == 0) {
		workingPath_.insert(pos, DEFAULT_NUMBER);
		copy_file(oldPath_, workingPath_, copy_options::skip_existing);
	}

	//extension found
	else {


		int copyNumber = searchNumberInPath();

		string oldNumber = to_string(copyNumber);
		string newNumber = to_string(++copyNumber);

		//first copy
		if(oldNumber == "0")
			workingPath_.insert(pos, " " + newNumber);
		//number found
		else
			workingPath_.replace(pos - oldNumber.length(), oldNumber.length(), newNumber);

		copy_file(oldPath_, workingPath_, copy_options::skip_existing);

	}

}

/// Replaces all occurences of a string in a line with another string
/// \param	line		Line that will have replacements
/// \param	toReplace	String that will be replaced once found in the line
/// \param	replacement Replacements made onto the toReplace strings located in the line
/// \return string		Line that has been modified
std::string File::replaceStrings(const std::string& line, const std::string& toReplace, const std::string& replacement)
{
	string newLine = line;
	size_t position = newLine.find(toReplace);
	while (position != std::string::npos) {
		newLine.replace(position, toReplace.length(), replacement);
		position = newLine.find(toReplace, position + replacement.length());
	}
	return newLine;
}

/// Replaces all occurences of a specific string with another in a file
/// makes a temporary copy of the file when replacing
///	emits replacementSuccess() once completed
/// \param
/// \return 		
void File::replaceFile()
{
	//creation of temporary file
	string tempPath = workingPath_ + "temp";

	//opening files
	workingFile_ = fstream(workingPath_, ios::in);
	fstream tempFile = fstream(tempPath, ios::out | ios::trunc);

	//verification if files opened
	if (workingFile_.fail() || tempFile.fail())
		throw exception("Could not open file to write");

	workingFile_.seekg(0, ios::beg);

	//start modification
	while(!workingFile_.eof()){

		string line = "";
		getline(workingFile_, line, '\0');

		//find position and replace
		line = replaceStrings(line, toReplace_, replacement_);

		tempFile << line;

	}

	//close files
	workingFile_.close();
	tempFile.close();

	//remove and rename
	remove(workingPath_.c_str());
	rename(tempPath.c_str(), workingPath_.c_str());

	emit replacementSuccess();
}




