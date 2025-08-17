#ifndef CREATEIPGENERAL_H
#define CREATEIPGENERAL_H

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>

class CreateIpGeneral : public QWidget
{
    Q_OBJECT
public:
    explicit CreateIpGeneral(QWidget *parent);

    QString IoabsDefFile();
signals:

private:
private:
    QLineEdit *nameTxt ;
    QLineEdit *versionTxt ;
    QLineEdit *vendorTxt ;
    QLineEdit *authorTxt ;
    QTextEdit *descriptionTxt;

};

#endif // CREATEIPGENERAL_H
