#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H

//#include <QWidget>
//class AWMEdit;
#include "awmedit.h"

class AWMEdit; // Forward declaration

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(AWMEdit *editor);
    //: QWidget(editor), codeEditor(editor) {}

    QSize sizeHint() const override
    {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    AWMEdit *codeEditor;
};

#endif // LINENUMBERAREA_H
