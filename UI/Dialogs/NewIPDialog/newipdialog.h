#ifndef NEWIPDIALOG_H
#define NEWIPDIALOG_H

#include <QDialog>

#include <IPXACTmodels/common/TagData.h>

#include <IPXACTmodels/kactusExtensions/KactusAttribute.h>


namespace Ui {
class NewIPDialog;
}

class VLNV;
class NewIPDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewIPDialog(QWidget *parent = 0);
    ~NewIPDialog();

private:
    Ui::NewIPDialog *ui;

private:
    void setupLayout();

signals:
    /*!
     *  Signaled when a ip should be created.
     */
    void createComponent(KactusAttribute::ProductHierarchy prodHier, KactusAttribute::Firmness firmness,
        QVector<TagData> tags, VLNV const& vlnv, QString const& directory);

//    void createComponent(KactusAttribute::ProductHierarchy prodHier, KactusAttribute::Firmness firmness,
//        QVector<TagData> tags, VLNV const& vlnv);

private slots:
    /*!
     *  Called when the user presses OK.
     */
    virtual void accept();

public:
    /*! Get the vendor of the VLNV.
     *
     *      @return The name of the vendor.
     */
    QString getVendor() const;

    /*! Get the library of the VLNV.
     *
     *      @return The name of the library
     */
    QString getLibrary() const;

    /*! Get the name of the IP item in VLNV.
     *
     *      @return The name of the IP item in VLNV.
     */
    QString getName() const;

    /*! Get the version of the VLNV.
     *
     *      @return the The version of the VLNV.
     */
    QString getVersion() const;
};

#endif // NEWIPDIALOG_H
