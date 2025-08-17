#ifndef VHDLRECORDPARSER_H
#define VHDLRECORDPARSER_H

#include <QFile>
#include <QString>
#include <QTextStream>
#include <QRegularExpression>
#include <QList>
#include <QDebug>

#include "vpinpartparser.h"

class VhdlRecordParser {
private:
    //QString m_filePath;
    QList<VPinPartParser*> m_part_records;

    // Moved regular expressions to static members
    static QRegularExpression recordPattern;
    static QRegularExpression ifdefPattern;

  //  VPinPartParser* pinParser;

public:
    VhdlRecordParser(){}
//    VhdlRecordParser(const QString& filePath) : m_filePath(filePath) {}

    bool parse(QString vhdlFile) {
        QFile file(vhdlFile);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            return false;
        }

        QTextStream in(&file);
        QString content = in.readAll();
        file.close();

        // ifdef 와 안맞는 부분파싱
        QString prog_code = getNonMatchingParts(content);
        qDebug() << "prog_code \n" << prog_code ;


        //ifdef ~ endif 사이를 파싱한다.
        QRegularExpressionMatchIterator recordMatchIterator = recordPattern.globalMatch(content);

        while (recordMatchIterator.hasNext()) {
            QRegularExpressionMatch recordMatch = recordMatchIterator.next();
            QString targetblock = recordMatch.captured(2);  // This should capture the content inside the ().

            if(targetblock.contains("hclk,"))
            {
                qDebug() << "";
            }

            VPinPartParser* pinParser = new VPinPartParser();
            pinParser->parse(targetblock);
            m_part_records.append(pinParser);
        }

        return true;
    }

    //문법파서 부분... ifdef 비교, parametor 처리 를 위해 미스매치부분 추출

    static QString getNonMatchingParts(const QString& input) {
        static QRegularExpression recordPattern(R"((\w+ \w+ #?\()\s*([^;]+)\);)");

        // Search for all matches
        QRegularExpressionMatchIterator matchIterator = recordPattern.globalMatch(input);

        // Keep a current position to slice the input
        int lastEnd = 0;

        // Store non-matching parts
        QString nonMatchingString;

        // Loop over matches - 미스매칭하는 부분만 모아서 문자열을 만들다.출
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();

            // Append non-matching part to the result string
            nonMatchingString += input.mid(lastEnd, match.capturedStart() - lastEnd);

            // Update the end position
            lastEnd = match.capturedEnd();
        }

        // Append any remaining text after the last match
        nonMatchingString += input.mid(lastEnd);

        return nonMatchingString;
    }

    void printRecords() const {
        for (int i = 0; i < m_part_records.size(); ++i) {
            qDebug() << "---------------------------- m_records[" << i << "]----------------------:" ; //<< m_part_records[i];
            m_part_records[i]->printRecords();
        }
    }
};

// Outside the class, initialize static members
QRegularExpression VhdlRecordParser::recordPattern(R"((\w+ \w+ #?\()\s*([^;]+)\);)");
QRegularExpression VhdlRecordParser::ifdefPattern(R"(`ifdef\s+(\w+)[\s\S]+?`endif)");


#endif // VHDLRECORDPARSER_H
