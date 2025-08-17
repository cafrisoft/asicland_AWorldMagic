#ifndef CREATEPROJECTGENERAL_H
#define CREATEPROJECTGENERAL_H

#include <QLineEdit>
#include <QTextEdit>
#include <QTableWidget>


class CreateProjectGeneral : public QWidget
{
    Q_OBJECT
public:
    explicit CreateProjectGeneral(QWidget *parent = nullptr);

signals:

private:
    QLineEdit *nameTxt ;
    QLineEdit *saveDirTxt ;
    QLineEdit *venderTxt ;
    QLineEdit *authorTxt ;
    QTextEdit *descriptionTxt;
public:
    QString MakeXML(QTableWidget *table);
    //void GetGenralInfo(QString &vendor, QString &name, QString &version, QString &description, QString& author);
    QString LoadXML(QString xmlFile);
    QString GetCurrentFileDir();
    QString strCurDir;
    void handleBrowse();
    QString GetProjectFileName();
};

#endif // CREATEPROJECTGENERAL_H
