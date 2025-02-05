#ifndef OKXRESTAPI_H
#define OKXRESTAPI_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class OkxRestApi : public QObject {
  Q_OBJECT
public:
  explicit OkxRestApi(QObject *parent, const QString& baseUrl);
  ~OkxRestApi();
  void fetchInstruments(const QString& instType);  // 获取产品列表
signals:
  // 产品信息返回
  void instrumentsReceived(const QList<TradingPairInfo>& instruments);
  // 错误信号
  void error(const QString& errorMsg);
private:
  QNetworkAccessManager* m_networkManager;
  QString m_baseUrl;
  // 构建公共数据请求
  QNetworkRequest buildPublicRequest(const QString& endpoint);
  // 处理响应
  void handleInstrumentsResponse(QNetworkReply* reply);
};

#endif // OKXRESTAPI_H
