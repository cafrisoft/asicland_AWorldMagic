#ifndef TEXTEDITORMANAGER_H
#define TEXTEDITORMANAGER_H

#include <QObject>



class TextEditorManager : public QObject
{
    Q_OBJECT

public:
    static TextEditorManager& getInstance()
    {
        static TextEditorManager instance; // Guaranteed to be destroyed, instantiated on first use.
        return instance;
    }

    //! The destructor.
    ~TextEditorManager() = default;

    QList<QStringList> syntaxChecker(QString fileExtension);

private:
    TextEditorManager();

signals:
};

#endif // TEXTEDITORMANAGER_H
