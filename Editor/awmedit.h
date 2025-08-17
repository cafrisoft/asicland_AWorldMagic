#ifndef AWMEDIT_H
#define AWMEDIT_H


#include <QWidget>
#include <QPlainTextEdit>
#include "highlighter.h"


/*
QT_BEGIN_NAMESPACE
class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
QT_END_NAMESPACE
*/

class AWMEdit : public QPlainTextEdit
{

    Q_OBJECT

public:

    AWMEdit(QWidget *parent = nullptr);
    ~AWMEdit();

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

//signals:

private:
    QWidget *lineNumberArea;

    void setupEditor();
//    QTextEdit *editor;
    Highlighter *highlighter;

protected:
    void resizeEvent(QResizeEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *e) override;
    void dropEvent(QDropEvent *e) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rect, int dy);

public :
    void OpenFile(QString fileName);
};

#endif // AWMEDIT_H
