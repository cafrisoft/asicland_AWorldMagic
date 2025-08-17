#include <QFile>
#include <QTextStream>
#include "xmlhandler.h"
#include "ipxmlgen.h"

IPXmlGen::IPXmlGen(): BaseXMLMgr()
{
    LoadScriptPath("");
}

void IPXmlGen::LoadGenXmlInfo(QString xmlFile, QString &vendor, QString &name, QString &version, QString &description, QString& author)
{
    QDomDocument doc;

    QFile file(xmlFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Cannot open file!";
        return ;
    }

    if (!doc.setContent(&file)) {
        qDebug() << "Failed to parse the XML!";
        file.close();
        return ;
    }
    file.close();


        // 요소 읽기
    QDomElement root = doc.documentElement(); // 루트 요소인 'library'를 얻습니다.

    QDomNodeList VendorNode = root.elementsByTagName("ipxact:vendor"); // 'book' 태그의 요소 목록을 얻습니다.
    for (int i = 0; i < VendorNode.count(); i++) {
        QDomElement VendorElem = VendorNode.at(i).toElement();
                // 태그 바로 아래의 텍스트 콘텐츠 가져오기
        QDomNode textNode = VendorElem.firstChild();
        while (!textNode.isText() && !textNode.isNull()) {
            textNode = textNode.nextSibling();
        }
        vendor = textNode.isNull() ? "" : textNode.nodeValue().trimmed();
    }


    QDomNodeList NameNode = root.elementsByTagName("ipxact:name"); // 태그의 요소 목록을 얻습니다.
    for (int i = 0; i < NameNode.count(); i++) {
        QDomElement Elem = NameNode.at(i).toElement();
                // 태그 바로 아래의 텍스트 콘텐츠 가져오기
        QDomNode textNode = Elem.firstChild();
        while (!textNode.isText() && !textNode.isNull()) {
            textNode = textNode.nextSibling();
        }
        name = textNode.isNull() ? "" : textNode.nodeValue().trimmed();
    }


    QDomNodeList VersionNode = root.elementsByTagName("ipxact:version");
    for (int i = 0; i < VersionNode.count(); i++) {
        QDomElement Elem = VersionNode.at(i).toElement();
                // 태그 바로 아래의 텍스트 콘텐츠 가져오기
        QDomNode textNode = Elem.firstChild();
        while (!textNode.isText() && !textNode.isNull()) {
            textNode = textNode.nextSibling();
        }
        version = textNode.isNull() ? "" : textNode.nodeValue().trimmed();
    }

    QDomNodeList DescriptNode = root.elementsByTagName("ipxact:description"); // '' 태그의 요소 목록을 얻습니다.
    for (int i = 0; i < DescriptNode.count(); i++) {
        QDomElement Elem = DescriptNode.at(i).toElement();
                // 태그 바로 아래의 텍스트 콘텐츠 가져오기
        QDomNode textNode = Elem.firstChild();
        while (!textNode.isText() && !textNode.isNull()) {
            textNode = textNode.nextSibling();
        }
        description = textNode.isNull() ? "" : textNode.nodeValue().trimmed();
    }

    QDomNodeList ExtensionNode = root.elementsByTagName("ipxact:vendorExtensions");
    for (int i = 0; i < ExtensionNode.count(); i++) {
        QDomElement Elem = ExtensionNode.at(i).toElement();
                // 태그 바로 아래의 텍스트 콘텐츠 가져오기
        QDomNode textNode = Elem.firstChild();
        while (!textNode.isText() && !textNode.isNull()) {
            textNode = textNode.nextSibling();
        }
        author = textNode.isNull() ? "" : textNode.nodeValue().trimmed();
    }
}


