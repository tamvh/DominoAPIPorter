#ifndef DOMINOAPI_H
#define DOMINOAPI_H

#include <QObject>
#include <QDomDocument>
#include <QDomElement>
#include "json/json.hpp"

using json = nlohmann::json;

class DominoAPI : public QObject
{
    Q_OBJECT
public:
    explicit DominoAPI(QObject *parent = 0);
    bool initizalize(const QString &username, const QString &password, const QString &domain);
signals:
    void eventStoreProducts(const json &storeProducts);
    void eventStoreInformation(const json &storeProducts);
    void eventStoreStreets(const json &storeProducts);
    void eventStoreCoupons(const json &storeProducts);
    void eventPlaceOrder(const json &storeProducts);
    void evenPlaceEditOrder(const json &storeProducts);
    void evenPriceOrder(const json &storeProducts);
    void evenGetOrderStatus(const json &storeProducts);
    void evenGetOrderStatuses(const json &storeProducts);
    void evenCancelOrder(const json &storeProducts);

public slots:
    void onGetStoreInformationFinish(const QVariant &data);
    void onGetStoreProductsFinish(const QVariant &data);
    void onGetStoreStreetsFinish(const QVariant &data);
    void onGetStoreCouponsFinish(const QVariant &data);
    void onPlaceOrderFinish(const QVariant &data);
    void onPlaceEditOrderFinish(const QVariant &data);
    void onPriceOrderFinish(const QVariant &data);
    void onGetOrderStatusFinish(const QVariant &data);
    void onGetOrderStatusesFinish(const QVariant &data);
    void onCancelOrderFinish(const QVariant &data);
    void onPostError(const int &error, const QString &message);

public:
    bool getStoreInformation(unsigned long storeId);
    bool GetStoreStreets(unsigned long storeId, bool getAll);
    bool getStoreProducts(unsigned long storeId);
    bool getStoreCoupons(unsigned long storeId);
    bool placeOrder(json& requestInfo);
    bool PlaceEditedOrder(json& requestInfo);
    bool PriceOrder(json& requestInfo);
    bool GetOrderStatus(json& requestInfo);
    bool GetOrderStatuses(json& requestInfo);
    bool CancelOrder(json& requestInfo);

private:
    json getSoapJson(const std::string &bodyName, json &bodyJson);
    json getSoapHeader();
    json getSoapBody(const std::string &bodyName, json &bodyJson);
    void sendSoapRequest(const json& soapJson, const QString &actionName, const QObject *receiver,
                         const QMetaMethod &method, Qt::ConnectionType type = Qt::AutoConnection);

    json parseStoreProducts(const QString &productText);

private:
    QString _username;
    QString _password;
    QString _serverDomain;
    QString _domainMessage;
    QString _domainAction;
};

#endif // DOMINOAPI_H
