#include "okxrestapi.h"

/**
 * @brief 构造函数
 * 
 * 初始化OkxRestApi对象：
 * 1. 设置父对象
 * 2. 初始化网络管理器
 * 3. 设置基础URL
 * 
 * @param parent 父对象指针，用于Qt的对象树管理
 * @param baseUrl 基础URL
 */
OkxRestApi::OkxRestApi(QObject *parent, const QString& baseUrl)
  : QObject(parent) 
  , m_baseUrl(baseUrl)
  , m_networkManager(new QNetworkAccessManager(this))
  {  
  qDebug() << Q_FUNC_INFO; 
}

/**
 * @brief 析构函数
 * 
 * 释放资源：
 * 1. 输出调试信息
 * 2. 由于QObject会自动删除子对象，不需要手动删除m_networkManager
 */
OkxRestApi::~OkxRestApi() {
  qDebug() << Q_FUNC_INFO; 
  // QObject 会自动删除子对象，不需要手动删除 m_networkManager
}

/**
 * @brief 构建公共数据请求
 * 
 * 构建完整的URL并设置请求头
 * 
 * @param endpoint API端点
 * @return QNetworkRequest 构建的请求对象
 */
QNetworkRequest OkxRestApi::buildPublicRequest(const QString& endpoint) {
  qDebug() << Q_FUNC_INFO; 
  // 构建完整的URL
  QUrl url(m_baseUrl + endpoint);
  QNetworkRequest request(url);
  // 设置请求头
  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
  return request;
}

/**
 * @brief 发送GET请求的通用方法
 * @param endpoint API端点
 * @param handler 处理响应的回调函数
 */
void OkxRestApi::sendGetRequest(const QString& endpoint, 
    std::function<void(const QJsonObject&)> handler) {
  // 构建请求
  QNetworkRequest request = buildPublicRequest(endpoint);
  
  // 发送GET请求
  QNetworkReply* reply = m_networkManager->get(request);
  
  // 连接完成信号
  connect(reply, &QNetworkReply::finished, this, [this, reply, handler]() {
    // 确保reply会被删除
    reply->deleteLater();
    
    // 检查网络错误
    if (reply->error() != QNetworkReply::NoError) {
      emit error(reply->errorString());
      return;
    }
    
    // 解析响应数据
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    QJsonObject obj = doc.object();
    
    // 检查API响应状态
    if (obj["code"].toString() != "0") {
      emit error(obj["msg"].toString());
      return;
    }
    
    // 调用回调处理数据
    handler(obj);
  });
}

/**
 * @brief 获取产品列表
 * 
 * 构建API端点并发送GET请求
 * 
 * @param instType 产品类型(如"SPOT"现货,"SWAP"永续合约)
 */
void OkxRestApi::fetchInstruments(const QString& instType) {
  qDebug() << Q_FUNC_INFO;
  QString endpoint = QString("/public/instruments?instType=%1").arg(instType);
  
  sendGetRequest(endpoint, [this](const QJsonObject& obj) {
    // 解析数据
    QList<TradingPairInfo> instruments;
    QJsonArray dataArray = obj["data"].toArray();
    
    for (const auto& item : dataArray) {
      QJsonObject itemObj = item.toObject();
      TradingPairInfo info;
      info.instId = itemObj["instId"].toString();
      info.baseCoin = itemObj["baseCcy"].toString();
      info.quoteCoin = itemObj["quoteCcy"].toString();
      info.state = itemObj["state"].toString();
      info.minSize = itemObj["minSz"].toString().toDouble();
      info.tickSize = itemObj["tickSz"].toString().toDouble();
      instruments.append(info);
    }
    
    emit instrumentsReceived(instruments);
  });
}
