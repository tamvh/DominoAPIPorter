#include "dominoapi.h"
#include <QDebug>
#include "http/httpbase2.h"
#include "functionutils.h"

DominoAPI::DominoAPI(QObject *parent) :
    _domainMessage("http://www.dominos.com/message/"),
    _domainAction("http://www.dominos.com/action/%1"),
    QObject(parent)
{

}

bool DominoAPI::initizalize(const QString &username, const QString &password, const QString &domain)
{
    _username = username;
    _password = password;
    _serverDomain = domain + "/remotepulseapi/remotepulseapi.wsdl";
    return true;
}

json DominoAPI::getSoapJson(const std::string &bodyName, json &bodyJson)
{
    json soapRootjson = {
        {"SOAP-ENVELOPE:Envelope", {
             {"@xmlns:SOAP-ENVELOPE", "http://schemas.xmlsoap.org/soap/envelope/"},
             {"SOAP-ENVELOPE:Header", getSoapHeader()},
             {"SOAP-ENVELOPE:Body", getSoapBody(bodyName, bodyJson)}
         }}
    };
    return soapRootjson;
}

json DominoAPI::getSoapHeader()
{

    json soapHeader = {
        {"Authorization", {
             {"FromURI", "dominos.com"},
             {"User", _username.toStdString()},
             {"Password", _password.toStdString()},
             {"TimeStamp", ""}
         }}
    };
    return soapHeader;
}

json DominoAPI::getSoapBody(const std::string &bodyName, json &bodyJson)
{
    json soapBody = {
        {"@SOAP-ENVELOPE:encodingStyle", "http://schemas.xmlsoap.org/soap/encoding/"},
        {bodyName, bodyJson}
    };
    return soapBody;
}

void DominoAPI::sendSoapRequest(const json& soapJson, const QString &actionName, const QObject *receiver,
                                const QMetaMethod &method, Qt::ConnectionType type)
{
    QDomDocument doc = FunctionUtils::convertJsonToXml(soapJson);
    HttpBase *http = new HttpBase(QString(""), this);
    QObject::connect(http, SIGNAL(done(QVariant)), receiver, SLOT(method(QVariant)), Qt::UniqueConnection);
    QObject::connect(http, SIGNAL(error(int,QString)), this, SLOT(onPostError(int, QString)), Qt::UniqueConnection);

    http->setContentType(CONTENT_TYPE_TEXT_XML);
    http->setUrl(QUrl(_serverDomain));
    http->addHeader("SOAPAction", _domainAction.arg(actionName));
    http->process(POST, nullptr, doc.toString(0).toUtf8());
}

bool DominoAPI::getStoreInformation(unsigned long storeId)
{
    json bodyJson = {
        {
             {"@xmlns:DOMINOS", _domainMessage.toStdString()},
             {"StoreID", storeId}
        }
    };
    json soapJson = getSoapJson("DOMINOS:GetStoreInformation", bodyJson);
    QDomDocument doc = FunctionUtils::convertJsonToXml(soapJson);

    HttpBase *http = new HttpBase(QString(""), this);
    QObject::connect(http, SIGNAL(done(QVariant)), this, SLOT(onGetStoreInformationFinish(QVariant)), Qt::UniqueConnection);
    QObject::connect(http, SIGNAL(error(int,QString)), this, SLOT(onPostError(int, QString)), Qt::UniqueConnection);

    http->setContentType(CONTENT_TYPE_TEXT_XML);
    http->setUrl(QUrl(_serverDomain));
    http->addHeader("SOAPAction", "http://www.dominos.com/action/GetStoreInformation");
    http->process(POST, nullptr, doc.toString(0).toUtf8());

    return true;
}

bool DominoAPI::GetStoreStreets(unsigned long storeId, bool getAll)
{
    json bodyJson = {
        {
            {"@xmlns:DOMINOS", "http://www.dominos.com/message/"},
            {"StoreID", storeId},
            {"AllStreets", getAll}
        }
    };
    json soapJson = getSoapJson("DOMINOS:GetStoreStreets", bodyJson);
    QDomDocument doc = FunctionUtils::convertJsonToXml(soapJson);

    HttpBase *http = new HttpBase(QString(""), this);
    QObject::connect(http, SIGNAL(done(QVariant)), this, SLOT(onGetStoreStreetsFinish(QVariant)), Qt::UniqueConnection);
    QObject::connect(http, SIGNAL(error(int,QString)), this, SLOT(onPostError(int, QString)), Qt::UniqueConnection);


    http->setContentType(CONTENT_TYPE_TEXT_XML);
    http->setUrl(QUrl(_serverDomain));
    http->addHeader("SOAPAction", "http://www.dominos.com/action/GetStoreInformation");
    http->process(POST, nullptr, doc.toString(0).toUtf8());
    return true;
}

bool DominoAPI::getStoreProducts(unsigned long storeId)
{
    json bodyJson = {
        {
             {"@xmlns:DOMINOS", "http://www.dominos.com/message/"},
             {"StoreID", storeId}
        }
    };
    json soapJson = getSoapJson("DOMINOS:GetStoreProducts", bodyJson);
    QDomDocument doc = FunctionUtils::convertJsonToXml(soapJson);

    HttpBase *http = new HttpBase(QString(""), this);
    QObject::connect(http, SIGNAL(done(QVariant)), this, SLOT(onGetStoreProductsFinish(QVariant)), Qt::UniqueConnection);
    QObject::connect(http, SIGNAL(error(int,QString)), this, SLOT(onPostError(int, QString)), Qt::UniqueConnection);


    http->setContentType(CONTENT_TYPE_TEXT_XML);
    http->setUrl(QUrl(_serverDomain));
    http->addHeader("SOAPAction", "http://www.dominos.com/action/GetStoreProducts");
    http->process(POST, nullptr, doc.toString(0).toUtf8());

    return false;
}

bool DominoAPI::getStoreCoupons(unsigned long storeId)
{
    json bodyJson = {
        {
             {"@xmlns:DOMINOS", "http://www.dominos.com/message/"},
             {"StoreID", storeId}
        }
    };
    json soapJson = getSoapJson("DOMINOS:GetStoreCoupons", bodyJson);
    QDomDocument doc = FunctionUtils::convertJsonToXml(soapJson);

    HttpBase *http = new HttpBase(QString(""), this);
    QObject::connect(http, SIGNAL(done(QVariant)), this, SLOT(onGetStoreCouponsFinish(QVariant)), Qt::UniqueConnection);
    QObject::connect(http, SIGNAL(error(int,QString)), this, SLOT(onPostError(int, QString)), Qt::UniqueConnection);

    http->setContentType(CONTENT_TYPE_TEXT_XML);
    http->setUrl(QUrl(_serverDomain));
    http->addHeader("SOAPAction", "http://www.dominos.com/action/GetStoreInformation");
    http->process(POST, nullptr, doc.toString(0).toUtf8());
    return true;
}

bool DominoAPI::placeOrder(json &requestInfo)
{
    requestInfo["@xmlns:DOMINOS"] = _domainMessage.toStdString();
    json soapJson = getSoapJson("DOMINOS:PlaceOrder", requestInfo);
    QDomDocument doc = FunctionUtils::convertJsonToXml(soapJson);

    HttpBase *http = new HttpBase(QString(""), this);
    QObject::connect(http, SIGNAL(done(QVariant)), this, SLOT(onPlaceOrderFinish(QVariant)), Qt::UniqueConnection);
    QObject::connect(http, SIGNAL(error(int,QString)), this, SLOT(onPostError(int, QString)), Qt::UniqueConnection);

    http->setContentType(CONTENT_TYPE_TEXT_XML);
    http->setUrl(QUrl(_serverDomain));
    http->addHeader("SOAPAction", _domainAction.arg("PlaceOrder"));
    http->process(POST, nullptr, doc.toString(0).toUtf8());
    return true;
}

bool DominoAPI::PlaceEditedOrder(json &requestInfo)
{
    requestInfo["@xmlns:DOMINOS"] = _domainMessage.toStdString();
    json soapJson = getSoapJson("DOMINOS:PlaceEditedOrder", requestInfo);
    QDomDocument doc = FunctionUtils::convertJsonToXml(soapJson);

    HttpBase *http = new HttpBase(QString(""), this);
    QObject::connect(http, SIGNAL(done(QVariant)), this, SLOT(onPlaceEditOrderFinish(QVariant)), Qt::UniqueConnection);
    QObject::connect(http, SIGNAL(error(int,QString)), this, SLOT(onPostError(int, QString)), Qt::UniqueConnection);

    http->setContentType(CONTENT_TYPE_TEXT_XML);
    http->setUrl(QUrl(_serverDomain));
    http->addHeader("SOAPAction", _domainAction.arg("PlaceEditedOrder"));
    http->process(POST, nullptr, doc.toString(0).toUtf8());
    return true;
}

bool DominoAPI::PriceOrder(json &requestInfo)
{
    requestInfo["@xmlns:DOMINOS"] = _domainMessage.toStdString();
    json soapJson = getSoapJson("DOMINOS:PriceOrder", requestInfo);
    QDomDocument doc = FunctionUtils::convertJsonToXml(soapJson);

    HttpBase *http = new HttpBase(QString(""), this);
    QObject::connect(http, SIGNAL(done(QVariant)), this, SLOT(onPriceOrderFinish(QVariant)), Qt::UniqueConnection);
    QObject::connect(http, SIGNAL(error(int,QString)), this, SLOT(onPostError(int, QString)), Qt::UniqueConnection);

    http->setContentType(CONTENT_TYPE_TEXT_XML);
    http->setUrl(QUrl(_serverDomain));
    http->addHeader("SOAPAction", _domainAction.arg("PriceOrder"));
    http->process(POST, nullptr, doc.toString(0).toUtf8());
    return true;
}

bool DominoAPI::GetOrderStatus(json &requestInfo)
{
    requestInfo["@xmlns:DOMINOS"] = _domainMessage.toStdString();
    json soapJson = getSoapJson("DOMINOS:GetOrderStatus", requestInfo);
    QDomDocument doc = FunctionUtils::convertJsonToXml(soapJson);

    HttpBase *http = new HttpBase(QString(""), this);
    QObject::connect(http, SIGNAL(done(QVariant)), this, SLOT(onGetOrderStatusFinish(QVariant)), Qt::UniqueConnection);
    QObject::connect(http, SIGNAL(error(int,QString)), this, SLOT(onPostError(int, QString)), Qt::UniqueConnection);

    http->setContentType(CONTENT_TYPE_TEXT_XML);
    http->setUrl(QUrl(_serverDomain));
    http->addHeader("SOAPAction", _domainAction.arg("GetOrderStatus"));
    http->process(POST, nullptr, doc.toString(0).toUtf8());
    return true;
}

bool DominoAPI::GetOrderStatuses(json &requestInfo)
{
    requestInfo["@xmlns:DOMINOS"] = _domainMessage.toStdString();
    json soapJson = getSoapJson("DOMINOS:GetOrderStatuses", requestInfo);
    QDomDocument doc = FunctionUtils::convertJsonToXml(soapJson);

    HttpBase *http = new HttpBase(QString(""), this);
    QObject::connect(http, SIGNAL(done(QVariant)), this, SLOT(onGetOrderStatusesFinish(QVariant)), Qt::UniqueConnection);
    QObject::connect(http, SIGNAL(error(int,QString)), this, SLOT(onPostError(int, QString)), Qt::UniqueConnection);

    http->setContentType(CONTENT_TYPE_TEXT_XML);
    http->setUrl(QUrl(_serverDomain));
    http->addHeader("SOAPAction", _domainAction.arg("GetOrderStatuses"));
    http->process(POST, nullptr, doc.toString(0).toUtf8());
    return true;
}

bool DominoAPI::CancelOrder(json &requestInfo)
{
    requestInfo["@xmlns:DOMINOS"] = _domainMessage.toStdString();
    json soapJson = getSoapJson("DOMINOS:CancelOrder", requestInfo);
    QDomDocument doc = FunctionUtils::convertJsonToXml(soapJson);

    HttpBase *http = new HttpBase(QString(""), this);
    QObject::connect(http, SIGNAL(done(QVariant)), this, SLOT(onCancelOrderFinish(QVariant)), Qt::UniqueConnection);
    QObject::connect(http, SIGNAL(error(int,QString)), this, SLOT(onPostError(int, QString)), Qt::UniqueConnection);

    http->setContentType(CONTENT_TYPE_TEXT_XML);
    http->setUrl(QUrl(_serverDomain));
    http->addHeader("SOAPAction", _domainAction.arg("CancelOrder"));
    http->process(POST, nullptr, doc.toString(0).toUtf8());
    return true;
}

void DominoAPI::onGetStoreInformationFinish(const QVariant &data)
{
    json result = FunctionUtils::convertXmlToJson(data.toString(), "StoreInformation");
    emit eventStoreInformation(result);
}

void DominoAPI::onGetStoreProductsFinish(const QVariant &data)
{
//    QDomDocument doc;

//    doc.setContent(data.toString());
//    QDomNodeList nodeList = doc.elementsByTagName("Products");
//    if (nodeList.size() > 0) {
//        json products = parseStoreProducts(nodeList.at(0).childNodes().at(0).nodeValue());
//        emit eventStoreProducts(products);
//    }
    QString str = data.toString();
    qDebug() << "str pro: " << str;
}

void DominoAPI::onGetStoreStreetsFinish(const QVariant &data)
{
    json result = FunctionUtils::convertXmlToJson(data.toString(), "StoreStreets");
    emit eventStoreStreets(result);

    HttpBase *http = qobject_cast<HttpBase*>(sender());
    http->deleteLater();
}

void DominoAPI::onGetStoreCouponsFinish(const QVariant &data)
{
    json result = FunctionUtils::convertXmlToJson(data.toString(), "StoreCoupons");
    emit eventStoreCoupons(result);

    HttpBase *http = qobject_cast<HttpBase*>(sender());
    http->deleteLater();
}

void DominoAPI::onPlaceOrderFinish(const QVariant &data)
{
    json result = FunctionUtils::convertXmlToJson(data.toString(), "OrderReply");
    emit eventPlaceOrder(result);

    HttpBase *http = qobject_cast<HttpBase*>(sender());
    http->deleteLater();
}

void DominoAPI::onPlaceEditOrderFinish(const QVariant &data)
{
    json result = FunctionUtils::convertXmlToJson(data.toString(), "OrderReply");
    emit evenPlaceEditOrder(result);

    HttpBase *http = qobject_cast<HttpBase*>(sender());
    http->deleteLater();
}

void DominoAPI::onPriceOrderFinish(const QVariant &data)
{
    json result = FunctionUtils::convertXmlToJson(data.toString(), "OrderReply");
    emit evenPriceOrder(result);

    HttpBase *http = qobject_cast<HttpBase*>(sender());
    http->deleteLater();
}

void DominoAPI::onGetOrderStatusFinish(const QVariant &data)
{
    json result = FunctionUtils::convertXmlToJson(data.toString(), "OrderReply");
    emit evenGetOrderStatus(result);

    HttpBase *http = qobject_cast<HttpBase*>(sender());
    http->deleteLater();
}

void DominoAPI::onGetOrderStatusesFinish(const QVariant &data)
{
    json result = FunctionUtils::convertXmlToJson(data.toString(), "OrderStatuses");
    emit evenGetOrderStatuses(result);

    HttpBase *http = qobject_cast<HttpBase*>(sender());
    http->deleteLater();
}

void DominoAPI::onCancelOrderFinish(const QVariant &data)
{
    json result = FunctionUtils::convertXmlToJson(data.toString(), "OrderReply");
    emit evenCancelOrder(result);

    HttpBase *http = qobject_cast<HttpBase*>(sender());
    http->deleteLater();
}

void DominoAPI::onPostError(const int &error, const QString &message)
{
    qDebug() << "on post error " << error << ", message = " << message;

    HttpBase *http = qobject_cast<HttpBase*>(sender());
    http->deleteLater();
}

json DominoAPI::parseStoreProducts(const QString &productText)
{
    json rootJson;
    json productsJson;
    QStringList eleList = productText.split("\r\n");
    if (eleList.length() <= 0) {
        return {};
    }

    QStringList colNameList = eleList.at(0).split("\t");
    QStringList colValueList;
    int colLength = 0;

    for (int i = 1; i < eleList.size(); i++) {
        json product;
        colValueList = eleList.at(i).split("\t");
        colLength = colValueList.length() <= colNameList.length() ? colValueList.length() : colNameList.length();
        for (int j = 0; j < colLength; j++) {
            product[colNameList.at(j).toStdString()] = colValueList.at(j).toStdString();
        }
        productsJson.push_back(product);
    }

    rootJson["StoreProducts"]["Products"] = productsJson;
    return rootJson;
}


