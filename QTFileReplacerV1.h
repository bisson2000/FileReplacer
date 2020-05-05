/**************************************************************************
**
** creator: Paul-André Bisson
**
** File: QTFileReplacerV1.h
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

#ifndef QTFILEREPLACEV1_H
#define QTFILEREPLACEV1_H


#include <QtWidgets/QMainWindow>
#include "ui_QTFileReplacerV1.h"

#include <QDesktopWidget>
#include <QApplication>

#include <QMessageBox>
#include <QPushButton>
#include <QWidget>>
#include <QSize>
#include <QSizePolicy>
#include <QBoxLayout>
#include <QT>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QCheckBox>

//File related
#include <QDir>
#include <QFileInfo>
#include <QFileDialog>
#include <QFile>

//Special keys related
#include <unordered_map> //Unordred_map
#include <algorithm> //find
#include <QKeyEvent>

//Classes
#include "file.h"



class QTFileReplacerV1 : public QMainWindow
{
	Q_OBJECT

public:
	QTFileReplacerV1(QWidget* parent = Q_NULLPTR);
	QTFileReplacerV1(File* file, QWidget* parent = Q_NULLPTR);
	~QTFileReplacerV1();

	//Aditionnal ui setup
	void additionnalSetup();

public slots:

	//open file explorer
	void openExplorer();
	//copy warning
	void copyWarning();
	//start file modification
	void startReplacement();
	//Show if the replacement was a succes
	void replacementSuccess();


signals:

protected:
	//filter special keys
	bool eventFilter(QObject* obj, QEvent* event);


private:

	//Methods
	std::string QStringToString(const QString& qstring) const;
	const bool fileCanBeOpened(const QString& fileName);
	
	//File components:
	//file_ To store the file on which the GUI works
	File* file_;
	//fileIsLocal_ To know if we created a new file_ locally
	bool fileIsLocal_;
	//BlackListedKeys
	const std::unordered_map<Qt::Key, std::string> blackListedKeys_{ {Qt::Key::Key_Tab, "\\t"}, {Qt::Key::Key_Enter, "\\n"}, {Qt::Key::Key_Return, "\\n"} };

	//GUI components:
	//basic ui for the application
	Ui::QTFileReplacerV1Class* ui;
	//button for the file explorer
	QPushButton* fileExplorerButton, *startReplacementButton;
	//Graphical edit fields for file path, toReplace and Replacement
	QLineEdit* pathEditor, *toReplaceEditor, *replacementEditor;
	//checkbox for the copy
	QCheckBox* enableCopy_;

};



#endif //QTFILEREPLACEV1_H

