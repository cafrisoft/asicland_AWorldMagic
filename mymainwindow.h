#ifndef MYMAINWINDOW_H
#define MYMAINWINDOW_H

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
class TabDocument;
class DrawingBoard;
class CreateIOMain;
class CreateDesignMain;
class MyMainWindow : public QWidget
{
    Q_OBJECT
public:
#if LIBRARYINTERFACE_TEST_20231206 == 0
    explicit MyMainWindow(LibraryHandler* libInterface, QWidget *parent = nullptr);
#else
    explicit MyMainWindow(LibraryInterface* libInterface, QWidget *parent = nullptr);
#endif

signals:
    void createDesign(VLNV const& vlnv, QString const& directory);

public slots:
    void displayFilePathSlot(const QString &filePath);

private :
#if LIBRARYINTERFACE_TEST_20231206 == 0
    //! The library interface.
    LibraryHandler* library_;
#else
    //! The library interface.
    LibraryInterface* library_;
#endif

#if MYMAINWINDOW_TEST == 1
    DrawingBoard* designTabs_;
#else
    QTabWidget *m_pTabWidget;
#endif

//    CreateIOMain *workTab;
    CreateDesignMain *workTab;



private :
#if MYMAINWINDOW_TEST == 1
    /*!
     *  Set up the drawing view where hierarchical components are edited.
     */
    void setupDrawBoard();
#endif

public:
#if MYMAINWINDOW_TEST == 1
    void AddTapWindow(const QString &type, QString xmlFullFile= "", TabDocument *tabDocument=nullptr);
    void AddTapWindow(TabDocument *tabDocument);
#else
    void AddTapWindow(const QString &type, QString xmlFullFile= "");
    void AddTapWindow(TabDocument *tabDocument);
#endif
};

#endif // MYMAINWINDOW_H
