#ifndef COPYTHREAD_H
#define COPYTHREAD_H

#include <QThread>


class CopyThread : public QThread
{    
    Q_OBJECT

public:
    CopyThread(const QString& source, const QString& destination);

private:
    QString m_source;
    QString m_destination;

signals:
    void progressChanged(int value);

private:
    void copyRecursive(const QString& src, const QString& dst);

protected:
    void run() override {
        copyRecursive(m_source, m_destination);
    }

};

#endif // COPYTHREAD_H
