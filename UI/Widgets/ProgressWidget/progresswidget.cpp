#include <QVBoxLayout>

#include <Thread/copythread.h>

#include "progresswidget.h"


ProgressWidget::ProgressWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

//    m_sourceLabel = new QLabel("소스 경로:", this);
//    layout->addWidget(m_sourceLabel);

//    m_sourcePathLabel = new QLabel(this);
//    layout->addWidget(m_sourcePathLabel);

//    m_destinationLabel = new QLabel("목적지 경로:", this);
//    layout->addWidget(m_destinationLabel);

//    m_destinationPathLabel = new QLabel(this);
//    layout->addWidget(m_destinationPathLabel);

//    m_startButton = new QPushButton("복사 시작", this);
//    layout->addWidget(m_startButton);

    m_progressBar = new QProgressBar(this);
    layout->addWidget(m_progressBar);

    setLayout(layout);

//    startCopy();

//    connect(m_startButton, &QPushButton::clicked, this, &ProgressWidget::startCopy);
}

void ProgressWidget::startCopy(const QString& source, const QString& destination)
{
//    QString sourcePath = "/path/to/source";
//    QString destinationPath = "/path/to/destination";

    QString sourcePath = source;
    QString destinationPath = destination;

//    m_sourcePathLabel->setText(sourcePath);
//    m_destinationPathLabel->setText(destinationPath);

    m_progressBar->setValue(0);

    m_copyThread = new CopyThread(sourcePath, destinationPath);
    connect(m_copyThread, &CopyThread::progressChanged, this, &ProgressWidget::updateProgress);
    m_copyThread->start();
}

void ProgressWidget::updateProgress(int value)
{
    m_progressBar->setValue(value);
}

void ProgressWidget::closeWidget()
{
    close();
}
