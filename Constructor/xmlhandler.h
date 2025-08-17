#ifndef XMLHANDLER_H
#define XMLHANDLER_H


#include <QDomDocument>
#include <QFile>
#include <QTextStream>

class XmlHandler {
public:
    XmlHandler();
    bool newXml();
    bool loadXml(const QString &path);
    bool saveXml(const QString &path);
    void modifyElement(const QString &tagName, const QString &newValue);
    void modifyGrandchildElement(const QString &grandParentTag, const QString &childTag, const QString &newValue);
    void appendChildNode(const QString &parentTag, const QString &newChildTag, const QString &nodeValue);
    void appendGrandchildNode(const QString &grandParentTag, const QString &parentTag, const QString &newChildTag, const QString &nodeValue);
    void appendChildAttribute(const QString &childTag, const QString &attributeName, const QString &attributeValue);
    void appendGrandchildAttribute(const QString &parentTag, const QString &grandChildTag, const QString &attributeName, const QString &attributeValue);
    void modifyChildAttribute(const QString &childTag, const QString &attributeName, const QString &newAttributeValue);
    void modifyGrandchildAttribute(const QString &parentTag, const QString &grandChildTag, const QString &attributeName, const QString &newAttributeValue);
    bool getElement(const QString &tagName, QDomNode& node, QString &childName, QString &childContent );

private:
    QDomDocument doc;
};

#endif // XMLHANDLER_H
