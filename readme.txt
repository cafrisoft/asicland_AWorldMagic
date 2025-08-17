xml //.. 파싱정보 :populate   참고코드

CreateIOLeftMenu   <- io.xml create 버튼 있는곳

<?xml version="1.0" encoding="UTF-8"?>
<library>
    <book id="1">
        HelloWorld
        <title>The Great Gatsby</title>
        <author>F. Scott Fitzgerald</author>
    </book>
    <book id="2">
        <title>1984</title>
        <author>George Orwell</author>
    </book>
</library>


---------------------------------------------------

#include <QDomDocument>
#include <QFile>
#include <QDebug>

int main() {
    QFile file("path_to_your_xml_file.xml");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Cannot open file!";
        return -1;
    }

    QDomDocument doc;
    if (!doc.setContent(&file)) {
        qDebug() << "Failed to parse the XML!";
        file.close();
        return -1;
    }
    file.close();

    // 요소 읽기
    QDomElement root = doc.documentElement(); // 루트 요소인 'library'를 얻습니다.
    QDomNodeList books = root.elementsByTagName("book"); // 'book' 태그의 요소 목록을 얻습니다.

    for (int i = 0; i < books.count(); i++) {
        QDomElement bookElem = books.at(i).toElement();
        QString id = bookElem.attribute("id");

        // <book> 태그 바로 아래의 텍스트 콘텐츠 가져오기
        QDomNode textNode = bookElem.firstChild();
        while (!textNode.isText() && !textNode.isNull()) {
            textNode = textNode.nextSibling();
        }
        QString bookContent = textNode.isNull() ? "" : textNode.nodeValue().trimmed();

        QString title = bookElem.firstChildElement("title").text();
        QString author = bookElem.firstChildElement("author").text();

        qDebug() << "ID:" << id << ", Content:" << bookContent << ", Title:" << title << ", Author:" << author;
    }

    // 수정된 XML 출력하기
    qDebug() << doc.toString();

    return 0;
}

-------------------------------

QStringList filters = {"*.c", "*.h", "*.tcl", "*.sdc", "*.rtl", ""};
QFileSystemModel* ipModel = createFilteredModel(filters);
QFileSystemModel* ioModel = createFilteredModel(filters);

ipTreeView->setModel(ipModel);
ioTreeView->setModel(ioModel);



------------------------------------------------------
설치프로그램 만들기
Qt Installer Framework 3.2.0 버젼을 설치했음

$ /home/dubi/Qt/QtIFW-3.2.2/bin/binarycreator

사용예)
binarycreator --offline-only -c config/config.xml -p packages MyApplicationInstaller.run

qt 런타임을 알려고 시도
[dubi@localhost bin]$ qmake-qt5 -query QT_INSTALL_PREFIX
/usr/lib64/qt5

