#ifndef CREATEIOGENERAL_H
#define CREATEIOGENERAL_H

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QTableWidget>

class CreateIOGeneral : public QWidget
{
    Q_OBJECT
public:
    explicit CreateIOGeneral(QWidget *parent = nullptr);

signals:

private:
    QLineEdit *nameTxt ;
    QLineEdit *versionTxt ;
    QLineEdit *venderTxt ;
    QLineEdit *authorTxt ;
    QTextEdit *descriptionTxt;
public:
    QString MakeXML(QTableWidget *table);
    //void GetGenralInfo(QString &vendor, QString &name, QString &version, QString &description, QString& author);
    QString LoadXML(QString xmlFile);
    QString GetCurrentFileDir();
    QString strCurDir;
};

#endif // CREATEIOGENERAL_H
