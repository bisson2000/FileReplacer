/**************************************************************************
**
** creator: Paul-André Bisson
**
** File: QTFileReplacerV1.cpp
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


//constructor without external file object

QTFileReplacerV1::QTFileReplacerV1(QWidget *parent)
	: QMainWindow(parent),
	ui(new Ui::QTFileReplacerV1Class)
{
    //Necessary atributes
    file_ = new File;
    fileIsLocal_ = true;

    //Setup
	ui->setupUi(this);
	additionnalSetup();
}

//Constructor with external file object
QTFileReplacerV1::QTFileReplacerV1(File* file, QWidget* parent) 
    : QMainWindow(parent),
    ui(new Ui::QTFileReplacerV1Class)
{
    //Necessary atributes
    file_ = file;
    fileIsLocal_ = false;

    //Setup
    ui->setupUi(this);
    additionnalSetup();
}

//destructor
QTFileReplacerV1::~QTFileReplacerV1() {
    delete ui;

    if (fileIsLocal_)
        delete file_;

}



/// Additionnal setup required for the ui
/// \param
/// \return
void QTFileReplacerV1::additionnalSetup() {

    //Central widget configuration
    centralWidget()->setLayout(new QVBoxLayout);
    centralWidget()->layout()->setAlignment(Qt::AlignTop);
    this->resize(600, 200);

    /***********************************************************************************************/

    //Field for the path
    QLabel* pathLabel = new QLabel;
    pathLabel->setText("Path:");
    pathEditor = new QLineEdit;
    pathEditor->installEventFilter(this);

    QHBoxLayout* pathLayout = new QHBoxLayout;
    pathLayout->addWidget(pathLabel);
    pathLayout->addWidget(pathEditor);
    pathLayout->setAlignment(Qt::AlignCenter);


    //file button
	fileExplorerButton = new QPushButton(this);
	fileExplorerButton->setText("Choose file");
    fileExplorerButton->setMaximumSize(400, 200);
    fileExplorerButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    connect(fileExplorerButton, SIGNAL(clicked()), 
            this, SLOT(openExplorer()));

    QHBoxLayout* fileExplorerButtonLayout = new QHBoxLayout;
    fileExplorerButtonLayout->addWidget(fileExplorerButton, Qt::AlignCenter);


    //File explorer Layout
    QVBoxLayout* fileExplorerLayout = new QVBoxLayout;
    fileExplorerLayout->addLayout(pathLayout);
    fileExplorerLayout->addLayout(fileExplorerButtonLayout, Qt::AlignCenter);


    //File explorer  widget
    QWidget* fileExplorerWidget = new QWidget;
    fileExplorerWidget->setLayout(fileExplorerLayout);

    /***********************************************************************************************/

    //Button to enable copy
    enableCopy_ = new QCheckBox("Copy file before replacing");
    enableCopy_->setCheckState(Qt::CheckState::Checked);

    QHBoxLayout* enableCopyLayout_ = new QHBoxLayout;
    enableCopyLayout_->addWidget(enableCopy_);


    //Field for the strings to replace
    QLabel* toReplaceLabel = new QLabel;
    toReplaceLabel->setText("Choose what to replace:");
    toReplaceLabel->setAlignment(Qt::AlignCenter);
    toReplaceEditor = new QLineEdit;
    toReplaceEditor->installEventFilter(this);

    QVBoxLayout* toReplaceLayout = new QVBoxLayout;
    toReplaceLayout->addWidget(toReplaceLabel);
    toReplaceLayout->addWidget(toReplaceEditor);
    toReplaceLayout->setAlignment(Qt::AlignCenter);


    //Field for the replacement strings
    QLabel* replacementLabel = new QLabel;
    replacementLabel->setText("Choose replacement:");
    replacementLabel->setAlignment(Qt::AlignCenter);
    replacementEditor = new QLineEdit;
    replacementEditor->installEventFilter(this);

    QVBoxLayout* replacementLayout = new QVBoxLayout;
    replacementLayout->addWidget(replacementLabel);
    replacementLayout->addWidget(replacementEditor);
    replacementLayout->setAlignment(Qt::AlignCenter);


    //Start replacement button
    startReplacementButton = new QPushButton(this);
    startReplacementButton->setText("Start replacement");
    startReplacementButton->setMaximumSize(400, 200);
    startReplacementButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    connect(startReplacementButton, SIGNAL(clicked()),
        this, SLOT(startReplacement()));

    QHBoxLayout* startReplacementButtonLayout = new QHBoxLayout;
    startReplacementButtonLayout->addWidget(startReplacementButton, Qt::AlignCenter);


    //options layout
    QVBoxLayout* optionsLayout = new QVBoxLayout;
    optionsLayout->addLayout(enableCopyLayout_);
    optionsLayout->addLayout(toReplaceLayout);
    optionsLayout->addLayout(replacementLayout);
    optionsLayout->addLayout(startReplacementButtonLayout);


    //options widget
    QWidget* optionsWidget = new QWidget;
    optionsWidget->setLayout(optionsLayout);

    /************************************************************************************************/

    //Main layout to be applied to the main widget
    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->addWidget(fileExplorerWidget,70);
    mainLayout->addWidget(optionsWidget,30);


    //main widget to add to central widget
    QWidget* widget = new QWidget;
    widget->setLayout(mainLayout);


    //central widget for the window
    centralWidget()->layout()->addWidget(widget);

    /************************************************************************************************/

    //Additionnal connections:

    //copy warning
    connect(file_, SIGNAL(copyWarning()),
        this, SLOT(copyWarning()));

    //success warning
    connect(file_, SIGNAL(replacementSuccess()),
        this, SLOT(replacementSuccess()));

}


/// Slot to open the file explorer
/// If nothing is chosen, do nothing
/// If a file selected cannot be opened, abort
/// Modifies the shown text for the path
/// \param	
/// \return 
void QTFileReplacerV1::openExplorer() {

    //get FileName
    QString filters = "All File (*.*) ;; Text File (*.txt)";
    QString fileName;
    if(pathEditor->text().isEmpty() || file_->getFileName().isEmpty())
        fileName = QFileDialog::getOpenFileName(nullptr, "Choose file", QDir::homePath(), filters);
    else
        fileName = QFileDialog::getOpenFileName(nullptr, "Choose file", file_->getFileName(), filters);

    //If file can be opened
    if (fileName.isEmpty()) {
        return;
    }
    if (!fileCanBeOpened(fileName)) {
        return;
    }

    //set fileName
    file_->setFileName(fileName);
    //Set PathEditor text
    pathEditor->setText(fileName);

}

/// Sends a warning if the copy modifies the file format
/// \param
/// \return 
void QTFileReplacerV1::copyWarning()
{
    QMessageBox::warning(this, "Opening warning", "File has no extensions \nAutomatic \".txt\" extension will be added");
}

/// Starts the replacement for a file according to what is in the QLineEdits
/// If the file cannot be openend, abort
/// \param
/// \return 
void QTFileReplacerV1::startReplacement()
{

    //setting up file
    file_->setFileName(pathEditor->text());
    file_->setPath(file_->fileNameToPath(pathEditor->text()));
    file_->setToReplace(file_->escapedCaractersConversion(QStringToString(toReplaceEditor->text())));
    file_->setReplacement(file_->escapedCaractersConversion(QStringToString(replacementEditor->text())));

    //verification of valid file
    if (!fileCanBeOpened(file_->getFileName()))
        return;

    //If copy is desired
    if(enableCopy_->checkState() == Qt::CheckState::Checked)
        file_->copyFile();

    file_->replaceFile();

}

/// Informs the user that the replacement was a succes
/// \param
/// \return
void QTFileReplacerV1::replacementSuccess()
{
    QMessageBox::information(this, "Replacement status", "Replacement completed", QMessageBox::StandardButton::Ok);
}

/// Filters blacklisted keys when entering replacements, to prevent switching for widget to widget
/// \param	obj 		object on which the filter is applied
/// \param	event       event that will be filtered
/// \return bool		if something has been filtered
bool QTFileReplacerV1::eventFilter(QObject* obj, QEvent* event)
{
    if (auto currentEdit = dynamic_cast<QLineEdit*> (obj)) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

            //search for blacklisted keys
            auto it = blackListedKeys_.begin();
            for (; it != blackListedKeys_.end(); it++) {
                if (it->first == keyEvent->key())
                    break;
            }

            //Add to text
            if (it != blackListedKeys_.end())
            {
                currentEdit->setText(currentEdit->text() + QString::fromStdString(it->second));
                return true;
            }
        }
    }
    // pass the event on to the parent class
    return QMainWindow::eventFilter(obj, event);
}

/// Verify if file can be opened
/// \param	fileName    Opens the file with Qt to ensure it exists
/// \return bool		if the file can be opened
const bool QTFileReplacerV1::fileCanBeOpened(const QString& fileName)
{
    QFile file(fileName);

    if (fileName.isEmpty() || !file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::critical(this, "Warning", "Cannot open file");
        file.close();
        return false;
    
    }
    file.close(); // can cause sharing violations if not closed and try to copy file

    return true;
}

/// Converts a QString to a string with a specific format
/// \param	qstring     QString that will be formated
/// \return string 		String result of the formated QString
std::string QTFileReplacerV1::QStringToString(const QString& qstring) const
{
    return qstring.toLocal8Bit().constData();
}







