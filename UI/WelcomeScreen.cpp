#include "WelcomeScreen.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QSize>
#include <QSizePolicy>
#include <QTextEdit>
#include <QString>
#include <QFile>
#include <QTextStream>



WelcomeScreen::WelcomeScreen(QWidget *parent) : TabDocument(parent, DOC_PROTECTION_SUPPORT)
{
    supportedWindows_ |= TabDocument::WELCOMEWINDOW;

    // Setup UI
    QHBoxLayout *hBoxLayout = new QHBoxLayout(this);
    hBoxLayout->SetMaximumSize;
    hBoxLayout->setSpacing(1);

    QLabel *labelLogo = new QLabel(this);
    labelLogo->setFixedSize(300, 180);
    labelLogo->setScaledContents(true);
    QPixmap logoImg(":/resources/Res/asicland_logo.png");
    logoImg.scaled(400, 240, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    labelLogo->setPixmap(logoImg);

    QVBoxLayout *vBoxLayout = new QVBoxLayout(this);
    vBoxLayout->SetMaximumSize;
    vBoxLayout->setSpacing(1);

    QLabel *labelTitle = new QLabel(this);
    labelTitle->setText("Welcome to AWorldMagic\u2122 !");
    labelTitle->setStyleSheet("font-size: 30px; font-weight: bold;");

    QLabel *labelUpdate = new QLabel(this);
    labelUpdate->setText("Update Log :");
    labelUpdate->setStyleSheet("font-weight: bold;");

    updateLog = new QTextEdit(this);
    updateLog->setReadOnly(true);
    updateLog->setUndoRedoEnabled(false);
    updateLog->setFocusPolicy(Qt::NoFocus);

    QSizePolicy textEditPolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    updateLog->setSizePolicy(textEditPolicy);

    hBoxLayout->addSpacing(50);
    hBoxLayout->addLayout(vBoxLayout);
    hBoxLayout->addSpacing(20);
    hBoxLayout->addWidget(labelLogo);
    hBoxLayout->addSpacing(50);

    vBoxLayout->addWidget(labelTitle);
    vBoxLayout->addSpacing(10);
    vBoxLayout->addWidget(labelUpdate);
    vBoxLayout->addWidget(updateLog);

    setLayout(hBoxLayout);

    setObjectName("Welcome");
    setDocumentName("Welcome");

    loadFile();
}



WelcomeScreen::~WelcomeScreen()
{

}



void WelcomeScreen::setProtection( bool locked )
{
    TabDocument::setProtection(locked);
}



VLNV WelcomeScreen::getIdentifyingVLNV() const
{
    return getDocumentVLNV();
}



void WelcomeScreen::refresh()
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    bool locked = isProtected();

    setModified(false);

    TabDocument::refresh();

    setProtection(locked);

    QApplication::restoreOverrideCursor();
}



bool WelcomeScreen::save()
{
    setModified(false);

    return false;
}



bool WelcomeScreen::saveAs()
{
    setModified(false);

    return true;
}



void WelcomeScreen::loadFile()
{
    QFile file(":/resources/Res/UpdateLog.txt");
    file.open(QFile::ReadOnly | QFile::Text);

    QTextStream fileText(&file);

    QString fileTexts = fileText.readAll();

    if (fileTexts.endsWith("\n"))
    {
        fileTexts.chop(1);
    }

    updateLog->setText(fileTexts);

    file.close();
}
