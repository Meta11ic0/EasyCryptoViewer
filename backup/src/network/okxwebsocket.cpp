/**
 * @brief OKX WebSocket客户端实现文件
 * @file okxwebsocket.cpp
 * 
 * 实现与OKX交易所WebSocket API的通信功能：
 * - WebSocket连接的建立和维护
 * - 心跳机制的实现
 * - 订阅产品列表和行情数据
 * - 消息的发送和接收处理
 */
#include "okxwebsocket.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

/**
 * @brief 构造函数
 * 
 * 初始化WebSocket客户端：
 * 1. 初始化基本成员变量
 * 2. 创建并配置心跳定时器
 * 3. 连接所有必要的信号和槽
 * 
 * @param parent 父对象指针，用于Qt的对象树管理
 * @param baseUrl 基础URL
 */
OKXWebSocket::OKXWebSocket(QObject *parent, const QString& baseUrl) 
  : QObject(parent)
  , m_baseUrl(baseUrl)
  , webSocket()          // 初始化WebSocket对象
  , isConnected(false)   // 初始化连接状态为未连接
  , pingTimer(new QTimer(this))
  , pongTimer(new QTimer(this))
{
  qDebug() << Q_FUNC_INFO;
  // 连接定时器信号
  connect(pingTimer, &QTimer::timeout, 
          this, &OKXWebSocket::sendPing);
  connect(pongTimer, &QTimer::timeout, 
          this, &OKXWebSocket::onPongTimeout);
  // 连接WebSocket信号
  connect(&webSocket, &QWebSocket::connected, 
          this, &OKXWebSocket::onConnected);
  connect(&webSocket, &QWebSocket::disconnected, 
          this, &OKXWebSocket::onDisconnected);
  connect(&webSocket, &QWebSocket::textMessageReceived, 
          this, &OKXWebSocket::onTextMessageReceived);
  connect(&webSocket, &QWebSocket::errorOccurred,
          this, &OKXWebSocket::onError);
}

/**
 * @brief 析构函数
 * 
 * 清理资源：
 * 1. 断开WebSocket连接
 * 2. 停止所有定时器
 * 3. Qt会自动删除子对象(定时器等)
 */
OKXWebSocket::~OKXWebSocket() {
  qDebug() << Q_FUNC_INFO;
  disconnectFromServer();
}

/**
 * @brief 连接到WebSocket服务器
 * 
 * 建立WebSocket连接：
 * 1. 检查当前连接状态，避免重复连接
 * 2. 尝试建立新的连接
 * 3. 连接成功后会触发onConnected信号
 * 
 * @param url WebSocket服务器地址
 * @return bool 连接请求是否成功发送
 */
bool OKXWebSocket::connectToServer(const QString& url) {
  qDebug() << Q_FUNC_INFO;
  qDebug() << "开始连接到服务器:" << url;
  if (isConnected) {
    qDebug() << "已经连接,无需重复连接";
    return true;
  }
  webSocket.open(QUrl(url));
  qDebug() << "连接请求已发送";
  return true;
}

/**
 * @brief 断开与服务器的连接
 * 
 * 执行断开连接操作：
 * 1. 检查当前是否已连接
 * 2. 关闭WebSocket连接
 * 3. 停止所有定时器
 * 4. 更新连接状态
 */
void OKXWebSocket::disconnectFromServer() {
  qDebug() << Q_FUNC_INFO;
  qDebug() << "执行断开连接操作";
  if (isConnected) {
    qDebug() << "正在关闭WebSocket连接";
    webSocket.close();
    if (pingTimer) {
      qDebug() << "停止ping定时器";
      pingTimer->stop();
    }
    if (pongTimer) {
      qDebug() << "停止pong定时器";
      pongTimer->stop();
    }
    isConnected = false;
  }
}


/**
 * @brief 订阅行情数据
 * 
 * 向服务器发送行情数据订阅请求：
 * 1. 检查连接状态
 * 2. 构建订阅消息
 * 3. 发送订阅请求
 * 
 * @param instType 产品类型
 * @param instId 产品ID(如"BTC-USDT")
 * @return bool 订阅请求是否成功发送
 */
bool OKXWebSocket::subscribeTickers(const QString& instType, const QString& instId) {
  qDebug() << Q_FUNC_INFO;
  if (!isConnected) {
    emit errorOccurred("未连接到服务器");
    return false;
  }
  qDebug() << "开始订阅行情数据" << " instType:" << instType << " instId:" << instId;
  // 构建订阅请求
  QJsonObject args;
  args["channel"] = "tickers";   // 设置频道为行情数据
  args["instType"] = instType;   // 设置产品类型
  args["instId"] = instId;       // 设置产品ID
  QJsonArray argsArray;
  argsArray.append(args);

  QJsonObject request;
  request["op"] = "subscribe";   // 操作类型为订阅
  request["args"] = argsArray;   // 添加参数数组

  // 发送订阅请求
  QString message = QJsonDocument(request).toJson(QJsonDocument::Compact);
  return webSocket.sendTextMessage(message) != -1;
}

/**
 * @brief 处理连接成功事件
 * 
 * WebSocket连接成功后的处理：
 * 1. 更新连接状态为已连接
 * 2. 启动心跳定时器
 * 3. 发送connected信号通知外部
 */
void OKXWebSocket::onConnected() {
  qDebug() << Q_FUNC_INFO;
  qDebug() << "WebSocket连接成功,开始初始化";
  isConnected = true;
  if (pingTimer) {
    qDebug() << "启动ping定时器,间隔:" << PING_INTERVAL << "ms";
    pingTimer->start(PING_INTERVAL);
    qDebug() << "发送首次ping";
    sendPing();
  }
  qDebug() << "发送connected信号";
  emit connected();
}

/**
 * @brief 处理连接断开事件
 * 
 * WebSocket连接断开后的处理：
 * 1. 更新连接状态为未连接
 * 2. 停止所有定时器
 * 3. 发送disconnected信号通知外部
 */
void OKXWebSocket::onDisconnected() {
  qDebug() << Q_FUNC_INFO;
  qDebug() << "WebSocket连接断开,开始清理";
  isConnected = false;
  if (pingTimer) {
    qDebug() << "停止ping定时器";
    pingTimer->stop();
  }
  if (pongTimer) {
    qDebug() << "停止pong定时器";
    pongTimer->stop();
  }
  qDebug() << "发送disconnected信号";
  emit disconnected();
}

/**
 * @brief 处理收到的文本消息
 * 
 * 处理服务器发送的WebSocket消息：
 * 1. 检查是否是pong消息
 * 2. 解析JSON格式的消息
 * 3. 根据消息类型分发处理
 * 
 * @param message 收到的消息内容
 */
void OKXWebSocket::onTextMessageReceived(const QString& message) {
  qDebug() << Q_FUNC_INFO;
  qDebug() << "收到WebSocket消息:" << message;
  // 处理 pong 响应
  if (message == "pong") {
    qDebug() << "收到pong响应";
    pongTimer->stop();
    return;
  }
  // 解析JSON消息
  QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
  if (doc.isNull()) {
    qDebug() << "消息解析失败:无效的JSON";
    emit errorOccurred("无效的JSON消息");
    return;
  }
  QJsonObject obj = doc.object();
  // 根据channel分发消息
  QString channel = obj["arg"].toObject()["channel"].toString();
  if (channel == "instruments") {
    emit instrumentsDataReceived(message);  // 发送产品列表数据
  } else if (channel == "tickers") {
    emit tickerDataReceived(message);       // 发送行情数据
  }
}

/**
 * @brief 处理WebSocket错误
 * 
 * 处理WebSocket连接过程中的错误：
 * 1. 获取错误信息
 * 2. 记录错误日志
 * 3. 通知外部错误发生
 * 
 * @param error WebSocket错误类型
 */
void OKXWebSocket::onError(QAbstractSocket::SocketError error) {
  qDebug() << Q_FUNC_INFO;
  qDebug() << "WebSocket错误:" << error;
  QString errorMessage = webSocket.errorString();
  qDebug() << "错误信息:" << errorMessage;
  emit errorOccurred(errorMessage);
}

/**
 * @brief 发送心跳包
 * 
 * 定时向服务器发送ping消息：
 * 1. 检查连接状态
 * 2. 构建ping消息
 * 3. 发送ping消息
 * 4. 启动pong超时定时器
 */
void OKXWebSocket::sendPing() {
  qDebug() << Q_FUNC_INFO;
  qDebug() << "准备发送ping消息";
  if (isConnected) {
    // 发送ping消息
    QString message = "ping";
    qDebug() << "发送ping消息:" << message;
    webSocket.sendTextMessage(message);
    qDebug() << "启动pong超时定时器:" << PONG_TIMEOUT << "ms";
    pongTimer->start(PONG_TIMEOUT);
  }
}

/**
 * @brief 处理pong超时
 * 
 * 当超过指定时间未收到服务器的pong响应：
 * 1. 认为连接已断开
 * 2. 更新连接状态
 * 3. 停止定时器
 * 4. 关闭连接
 */
void OKXWebSocket::onPongTimeout() {
  qDebug() << Q_FUNC_INFO;  
  if (isConnected) {
    qDebug() << Q_FUNC_INFO;
    isConnected = false;
    pingTimer->stop();
    pongTimer->stop();
    webSocket.close();
    emit errorOccurred("心跳超时，连接已断开");
    emit disconnected();
  }
}

/**
 * @brief 处理ping超时
 * 
 * 当超过指定时间未收到服务器的ping响应：
 * 1. 认为连接已断开
 * 2. 更新连接状态
 * 3. 停止定时器
 * 4. 关闭连接
 */
void OKXWebSocket::onPingTimeout() {
  qDebug() << Q_FUNC_INFO;  
  if (isConnected) {
    qDebug() << Q_FUNC_INFO;
    isConnected = false;
    pingTimer->stop();
    pongTimer->stop();
    webSocket.close();
    emit errorOccurred("ping超时，连接已断开");
    emit disconnected();
  }
}
