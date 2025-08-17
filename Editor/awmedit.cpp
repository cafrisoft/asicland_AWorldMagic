#include "awmedit.h"
#include <QPainter>
#include <QTextBlock>
//#include <QTextEdit>
#include <QFile>
#include <QTextStream>
#include <QMimeData>

#include "./Editor/highlighter.h"
#include "linenumberarea.h"


AWMEdit::AWMEdit(QWidget *parent)
    : QPlainTextEdit{parent}
{
     setAcceptDrops(true); // This enables the widget to accept drop events

    setupEditor();
    //setCentralWidget(this);
    setWindowTitle(tr("Syntax Highlighter"));


    lineNumberArea = new LineNumberArea(this);

    connect(this, &AWMEdit::blockCountChanged, this, &AWMEdit::updateLineNumberAreaWidth);
    connect(this, &AWMEdit::updateRequest, this, &AWMEdit::updateLineNumberArea);
    connect(this, &AWMEdit::cursorPositionChanged, this, &AWMEdit::highlightCurrentLine);

    updateLineNumberAreaWidth(0);
   // highlightCurrentLine();
}

AWMEdit::~AWMEdit() {
    // 이곳에 AWMEdit의 소멸자 코드를 작성하세요.
}


void AWMEdit::setupEditor()
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    //editor = new QTextEdit;
    this->setFont(font);

    highlighter = new Highlighter(this->document());

   // QFile file("mainwindow.h");
   // if (file.open(QFile::ReadOnly | QFile::Text))
//        this->setPlainText(file.readAll());
}


int AWMEdit::lineNumberAreaWidth()
{

    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3;//.... + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}

//![extraAreaWidth]

//![slotUpdateExtraAreaWidth]

void AWMEdit::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

//![slotUpdateExtraAreaWidth]

//![slotUpdateRequest]

void AWMEdit::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

//![slotUpdateRequest]

//![resizeEvent]

void AWMEdit::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    // 아래라인이 없으면 paintEvent(...)이 발생하지 않지롱~
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

//![resizeEvent]

//![cursorPositionChanged]

void AWMEdit::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::yellow).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

//![cursorPositionChanged]

//![extraAreaPaintEvent_0]

void AWMEdit::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

    //![extraAreaPaintEvent_0]

    //![extraAreaPaintEvent_1]
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());
    //![extraAreaPaintEvent_1]

    //![extraAreaPaintEvent_2]
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}
//![extraAreaPaintEvent_2]


void AWMEdit::dragEnterEvent(QDragEnterEvent *e) {
    if (e->mimeData()->hasUrls()) { // Check if the event has a URL, which we need for a file path
        e->acceptProposedAction(); // Accept the event
    }
}

void AWMEdit::dropEvent(QDropEvent *e) {
    for (const QUrl &url: e->mimeData()->urls()) {
        QString fileName = url.toLocalFile();
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            this->setPlainText(in.readAll()); // Reads the file and sets the text to the text edit
        }
    }
}

void  AWMEdit::OpenFile(QString fileName) {
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        this->setPlainText(in.readAll()); // Reads the file and sets the text to the text edit
    }
}


