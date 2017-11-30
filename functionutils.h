#ifndef FUNCTIONUTILS_H
#define FUNCTIONUTILS_H

#include <QObject>
#include <QDomDocument>
#include <QDomElement>
#include "json/json.hpp"

using json = nlohmann::json;

class FunctionUtils : public QObject
{
    Q_OBJECT
public:
    explicit FunctionUtils(QObject *parent = 0);
    static json convertXmlToJson(const QString &xmlText, const QString &fromNode = "");
    static json convertXmlToJson(const QDomNode &xmlNode);

    static QDomDocument convertJsonToXml(const json &infoJson);
signals:

public slots:

private:
    static bool convertNode(const QDomNode &xmlNode, json &resultJson);
    static bool convertAttributeXml(const QDomNode &xmlNode, json &resultJson);
    static bool checkChildTextNodeXml(const QDomNode &xmlNode);
    static void analyticNodeList(const QDomNode &xmlNode, QHash<QString, int> &xmlArrayNodeList, QHash<QString, int> &xmlObjectNodeList);

    static bool convertJson(const json &infoJson, QDomDocument &doc, QDomNode &xmlNode);
    static bool checkIsAttribute(const std::string &name);
    static bool checkIsTextNode(const std::string &name);
    static QString getValue(const json &infoJson);
};

#endif // FUNCTIONUTILS_H
