#ifndef OKXWEBSOCKET_H
#define OKXWEBSOCKET_H

#include <QObject>      // 包含Qt基础对象类
#include <QWebSocket>   // WebSocket功能支持
#include <QString>      // Qt字符串类
#include <QTimer>       // 定时器功能
#include <QJsonDocument> // JSON文档处理
#include <QJsonObject>   // JSON对象处理
#include <QJsonArray>    // JSON数组处理
#include "../config/config.h"

class OKXWebSocket : public QObject {
  Q_OBJECT 
public:
  explicit OKXWebSocket(QObject *parent, const QString& baseUrl);
  ~OKXWebSocket();
  bool connectToServer(const QString& url);
  void disconnectFromServer();
  bool subscribeTickers(const QString& instType, const QString& instId);
  bool isConnectedToServer() const { return isConnected; }

private:
  QString m_baseUrl;  
  QWebSocket webSocket; // WebSocket连接对象
  QTimer* pingTimer = nullptr;     // 发送ping的定时器
  QTimer* pongTimer = nullptr;     // 等待pong响应的定时器  
  bool isConnected = false; // 连接状态标志

private slots:
  void onConnected();     // 连接成功时的处理
  void onDisconnected();  // 连接断开时的处理
  void onTextMessageReceived(const QString& message);  // 收到消息的处理
  void onError(QAbstractSocket::SocketError error);    // 错误处理
  void sendPing();        // 发送心跳包
  void onPongTimeout();   // 处理pong超时 
  void onPingTimeout();   // 处理ping超时

signals:
  void connected();       // 连接成功信号
  void disconnected();    // 连接断开信号
  void tickerDataReceived(const QString& data);  // 接收到行情数据信号
  void errorOccurred(const QString& errorMessage);  // 发生错误信号
  void instrumentsDataReceived(const QString& data);  // 接收到产品列表数据信号
};

#endif // OKXWEBSOCKET_H
