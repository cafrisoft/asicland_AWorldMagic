#ifndef CREATEDESIGNGENERAL_H
#define CREATEDESIGNGENERAL_H

#include <IPXACTmodels/common/VLNV.h>
#include <IPXACTmodels/kactusExtensions/KactusAttribute.h>

#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>

#if LIBRARYINTERFACE_TEST_20231206 == 0
class LibraryHandler;
#else
class LibraryInterface;
#endif
class Component;
class CreateDesignGeneral : public QWidget
{
    Q_OBJECT
public:
#if LIBRARYINTERFACE_TEST_20231206 == 0
explicit CreateDesignGeneral(LibraryHandler* libInterface, QWidget *parent);
#else
explicit CreateDesignGeneral(LibraryInterface* libInterface, QWidget *parent);
#endif


private:
#if LIBRARYINTERFACE_TEST_20231206 == 0
    //! The library interface.
    LibraryHandler* library_;
#else
    //! The library interface.
    LibraryInterface* library_;
#endif

    //! Extension for design object.
    QString designExt_;

    //! Extension for designcfg object.
    QString designConfExt_;

#ifdef CREATE_PROJECT_TEST
    QLineEdit *projectDirTxt ;
#else
    QComboBox *projectComboBox ;
#endif

#if CREATEDESIGNGENERAL_20231206 == 0
    QLineEdit *nameTxt ;
    QLineEdit *versionTxt ;
    QLineEdit *authorTxt ;
    QTextEdit *descriptionTxt;
#else
    QLineEdit *vendorTxt ;
    QLineEdit *libraryTxt ;
    QLineEdit *nameTxt ;
    QLineEdit *versionTxt ;
#endif

private:
    /*!
     *  Generates a VLNV that matches the contents of the editor.
     */
    VLNV getVLNV() const;

//    /*!
//     *  Returns the design VLNV.
//     */
//    VLNV getDesignVLNV() const;

//    /*!
//     *  Returns the design configuration VLNV.
//     */
//    VLNV getDesignConfVLNV() const;

 signals:
//    void createEmptyDesign();
    //! Signaled when a design should be created.
//    void createDesign(KactusAttribute::ProductHierarchy prodHier, KactusAttribute::Firmness firmness, VLNV const& vlnv, QString const& directory);
    void createDesign(VLNV const& vlnv, QString const& directory);

private slots:
#ifdef CREATE_PROJECT_TEST
    void onHandleBrowse();
#endif

//public slots:
public:
    void onCreateEmptyDesign();

public:
#ifdef CREATE_PROJECT_TEST
    QString selectDirectory(QWidget *parent = nullptr);
#endif
    QString designcfgFile();



};

#endif // CREATEDESIGNGENERAL_H
