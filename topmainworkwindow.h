#ifndef TOPMAINWORKWINDOW_H
#define TOPMAINWORKWINDOW_H

#include <IPXACTmodels/common/VLNV.h>

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QSplitter>


#if LIBRARYINTERFACE_TEST_20231206 == 0
class LibraryHandler;
#else
class LibraryInterface;
#endif
class CommandWindow;
class TopMainWorkWindow : public QWidget
{
    Q_OBJECT
public:
#if LIBRARYINTERFACE_TEST_20231206 == 0
    explicit TopMainWorkWindow(LibraryHandler* libInterface, QWidget *parent = nullptr);
#else
    explicit TopMainWorkWindow(LibraryInterface* libInterface, QWidget *parent = nullptr);
#endif

private:
    CommandWindow * commandWindow_;
#if LIBRARYINTERFACE_TEST_20231206 == 0
    //! The library interface.
    LibraryHandler* library_;
#else
    //! The library interface.
    LibraryInterface* library_;
#endif


signals:
    /*!
     *  Transfers error messages between the main window and the message console.
     *
     *      @param [in] message     The error message.
     */
    void errorMessage(QString const& message);

    /*!
     *  Transfers notice messages between the main window and the message console.
     *
     *      @param [in] message     The notice message.
     */
    void noticeMessage(QString const& message);

    void createDesign(VLNV const& vlnv, QString const& directory);

private:
    /*!
     *  Setup the message console and the containing output dock widget.
     */
    void setupMessageConsole();

public slots:
};

#endif // TOPMAINWORKWINDOW_H
