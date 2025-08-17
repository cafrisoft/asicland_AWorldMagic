
#if LIBRARYINTERFACE_TEST_20231206 == 0
#include <KactusAPI/LibraryHandler.h>
#else
#include <KactusAPI/LibraryInterface.h>
#endif

#include "topmainworkwindow.h"
#include "mymainwindow.h"
#include "CommandWindow/CommandWindow.h"
#include "singleton.h"

#include <QLabel>

#if LIBRARYINTERFACE_TEST_20231206 == 0
TopMainWorkWindow::TopMainWorkWindow(LibraryHandler* libInterface, QWidget *parent)
#else
TopMainWorkWindow::TopMainWorkWindow(LibraryInterface* libInterface, QWidget *parent)
#endif
    : QWidget(parent),
      library_(libInterface)
{
    setMinimumSize(10, 10); // or any size you want

    // Create MyMainWindow and CommandWindow
    MyMainWindow *myMainWindow = new MyMainWindow(library_, this);
    Singleton::getInstance().setMainWorkWind(myMainWindow);

    commandWindow_ = new CommandWindow();
    Singleton::getInstance().setMyCommandWind(commandWindow_);


    myMainWindow->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    commandWindow_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Limit the height of the widgets to 90% of the window's height
//    myMainWindow->setMaximumHeight(this->height() * 0.9);
//    commandWindow_->setMaximumHeight(this->height() * 0.9);

    // Create QSplitter
    QSplitter *splitter = new QSplitter(Qt::Vertical, this); // Vertical orientation for top-down arrangement
    splitter->addWidget(myMainWindow);
    splitter->addWidget(commandWindow_);

    // Optionally set initial sizes (if you want)
    QList<int> initialSizes;
    initialSizes << this->height() * 0.8 << this->height() * 0.2; // e.g., 80% for main window, 20% for command window
    splitter->setSizes(initialSizes);

    // Set the QLabel to fill the widget
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(0); // Reduce the space between widgets to 2 pixels
    layout->setContentsMargins(0, 0, 0, 0); // Set all margins to 0 pixels

    layout->addWidget(splitter); // Add the splitter to the layout

    setLayout(layout);

    QObject::connect(myMainWindow, &MyMainWindow::createDesign, this, &TopMainWorkWindow::createDesign);

    setupMessageConsole();

    //....qDebug() << "TopMainWorkWindow has been created.";
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::setupMessageConsole()
//-----------------------------------------------------------------------------
void TopMainWorkWindow::setupMessageConsole()
{
//    commandWindow_* commandWindow_ = Singleton::getInstance().getMyCommandWind();

//    connect(this, SIGNAL(errorMessage(const QString&)),
//        commandWindow_, SLOT(onErrorMessage(const QString&)), Qt::UniqueConnection);
//    connect(this, SIGNAL(noticeMessage(const QString&)),
//        commandWindow_, SLOT(onNoticeMessage(const QString&)), Qt::UniqueConnection);
}
