#ifndef VPINPARTPARSER_H
#define VPINPARTPARSER_H

#include <QFile>
#include <QStringList>
#include <QDebug>

class VPinPartParser {
public:
    VPinPartParser() {}

    bool parse(QString dataString) {

        bool inRecord = false;
        bool inModule = true;  // 이미 module안의 문자를 파싱한 상태로 들어옴
        QStringList currentRecordData;
        QString currentRecordName;
        QStringList commonData;  // 'common' 항목을 위한 QStringList

        //QStringList lines = dataString.split('\n', Qt::SkipEmptyParts);
        QStringList lines = dataString.split('\n', Qt::SkipEmptyParts);

        for (const QString& line : lines) {
            QString trimmedLine = line.trimmed();

            if (trimmedLine.startsWith("module")) {
                inModule = true;
            } else if (trimmedLine.endsWith(");")) {   //<-- ");" 문자는 이미 파싱에서 제거되어 이곳에 안옴
                inModule = false;
                if (!commonData.isEmpty()) {
                    m_IORecords.insert("common", commonData);
                    commonData.clear();
                }
            } else if (trimmedLine.startsWith("`ifdef")) {
                inRecord = true;
                currentRecordName = trimmedLine.split(' ')[1]; // Get the name after `ifdef
            } else if (trimmedLine.startsWith("`endif")) {
                inRecord = false;
                m_IORecords.insert(currentRecordName, currentRecordData);
                currentRecordName.clear();
                currentRecordData.clear();
            } else if (inModule && !inRecord) {
                if (trimmedLine.endsWith(",")) {
                    trimmedLine.chop(1);
                }
                commonData.append(trimmedLine);
            } else if (inRecord) {
                if (trimmedLine.endsWith(",")) {
                    trimmedLine.chop(1);
                }
                currentRecordData.append(trimmedLine);
            }
        }

        // --  뒤 부분 옵션이 있으면 추가함
        if (!commonData.isEmpty()) {
            m_IORecords.insert("common", commonData);
            commonData.clear();
        }
        return true;
    }

    void printRecords() const {
        for (auto it = m_IORecords.begin(); it != m_IORecords.end(); ++it) {
            qDebug() << "Record Name:" << it.key();
            qDebug() << "Data:";
            for (const QString& data : it.value()) {
                qDebug() << data;
            }
        }
    }

private:
    QString m_filePath;
    QMap<QString, QStringList> m_IORecords;
};


#endif // VPINPARTPARSER_H
