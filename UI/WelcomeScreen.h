#ifndef WELCOMESCREEN_H
#define WELCOMESCREEN_H

#include <QTextEdit>

#include <UI/Widgets/TabDocument/TabDocument.h>

class WelcomeScreen : public TabDocument
{
    Q_OBJECT

public:
    WelcomeScreen(QWidget *parent);

    ~WelcomeScreen();

    virtual VLNV getIdentifyingVLNV() const;
    virtual void refresh();

private:
    QTextEdit *updateLog;

    void loadFile();

signals:

public slots:
    virtual bool save();
    virtual bool saveAs();
    virtual void setProtection(bool locked);
};

#endif // WELCOMESCREEN_H
