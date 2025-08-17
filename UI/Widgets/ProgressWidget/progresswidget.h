#ifndef PROGRESSWIDGET_H
#define PROGRESSWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>

class CopyThread;
class ProgressWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ProgressWidget(QWidget *parent = 0);

private:
//    QLabel *m_sourceLabel;
//    QLabel *m_sourcePathLabel;
//    QLabel *m_destinationLabel;
//    QLabel *m_destinationPathLabel;
//    QPushButton *m_startButton;
    QProgressBar *m_progressBar;
    CopyThread *m_copyThread;

signals:

private slots:
     void updateProgress(int value);

public slots:
     void startCopy(const QString& source, const QString& destination);
     void closeWidget();
};

#endif // PROGRESSWIDGET_H
