#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QObject>
#include <QString>
#include <QTextEdit>

#include <UI/Widgets/TabDocument/TabDocument.h>
#include <UI/Editor/TextEditor/TextEditorManager.h>



class TextEditor : public TabDocument
{
    Q_OBJECT

public:
    TextEditor(QWidget *parent);

    ~TextEditor();

    virtual VLNV getIdentifyingVLNV() const;
    virtual void refresh();

    void loadFile(QString filePath);
    void saveFile(QString filePath);
    void loadFileLine();

private:
    TextEditor(const TextEditor& other);
    TextEditor& operator=(const TextEditor& other);

    QString fileExtension;
    QTextEdit *editorLine;
    QTextEdit *editorText;

    void setupLayout();
    void syntaxHighlighter();
    void areaHighlighter();

signals:

public slots:
    virtual bool save();
    virtual bool saveAs();
    virtual void setProtection(bool locked);
};

#endif // TEXTEDITOR_H
