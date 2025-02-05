#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLayout>
#include <QButtonGroup>
#include <QScrollArea>
#include <QScrollBar>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QTableWidget>
#include <QFile>
#include "markettablefactory.h"
#include "../constant/constants.h"
#include "../network/okxwebsocket.h"


class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
private:
  // === 主布局相关 ===
  QWidget* centralWidget = nullptr;        // 中心部件
  QVBoxLayout* centralLayout = nullptr;       // 主垂直布局
  // === 主导航区域 ===
  QWidget* mainNavWidget = nullptr;        // 主导航容器
  QHBoxLayout* mainNavLayout = nullptr;    // 主导航布局
  QButtonGroup* mainNavButtonGroup = nullptr;    // 主导航按钮组
  QLineEdit* mainNavSearchBox = nullptr;   // 搜索框
  // === 子导航区域 ===
  QWidget* subNavWidget = nullptr;                // 子导航容器
  QHBoxLayout* subNavLayout = nullptr;            // 子导航布局
  // 子导航币种区域
  QWidget* subNavCoinWidget = nullptr;            // 币种区域容器
  QHBoxLayout* subNavCoinLayout = nullptr;        // 币种区域布局
  QPushButton* leftArrowButton = nullptr;         // 左箭头按钮
  QPushButton* rightArrowButton = nullptr;        // 右箭头按钮
  QScrollArea* subNavCoinScrollArea = nullptr;    // 可滚动区域
  QWidget* subNavCoinScrollContent = nullptr;    // 可滚动区域内容
  QHBoxLayout* subNavCoinScrollLayout = nullptr;        // 可滚动区域布局
  QScrollBar* subNavCoinScrollBar = nullptr;                      // 水平滚动条
  QButtonGroup* subNavCoinGroup = nullptr;        // 币种按钮组
  // 子导航计价货币区域
  QWidget* subNavQuoteWidget = nullptr;           // 计价货币容器
  QHBoxLayout* subNavQuoteLayout = nullptr;       // 计价货币布局
  QLabel* subNavQuoteLabel = nullptr;            // "计价货币"标签
  QComboBox* subNavQuoteCombo = nullptr;         // 计价货币下拉框
  // 子导航保证金区域
  QWidget* subNavMarginWidget = nullptr;          // 保证金容器
  QHBoxLayout* subNavMarginLayout = nullptr;      // 保证金布局
  QButtonGroup* subNavMarginGroup = nullptr;      // 保证金按钮组
  // 子导航合约类型区域
  QWidget* subNavFuturesWidget = nullptr;         // 合约类型容器
  QHBoxLayout* subNavFuturesLayout = nullptr;     // 合约类型布局
  QButtonGroup* subNavFuturesGroup = nullptr;     // 合约类型按钮组
  // === 市场数据区域 ===
  QWidget* marketWidget = nullptr;         // 市场数据容器
  QVBoxLayout* marketLayout = nullptr;     // 市场数据布局
  QTableWidget* marketTable = nullptr;     // 市场数据表格
  // === 网络相关 ===
  OKXWebSocket* okxWebSocket = nullptr; // OKX WebSocket 对象
  // === 初始化函数 ===
  void InitMainWindow();        // 设置UI
  void InitMainNavArea();       // 创建主导航区域
  void InitSubNavArea();        // 创建子导航区域
  void InitSubNavCoinArea();    // 初始化币种区域
  void InitSubNavQuoteArea();   // 初始化计价货币区域
  void InitSubNavMarginArea();  // 初始化保证金区域
  void InitSubNavFuturesArea(); // 初始化合约类型区域
  void InitMarketArea();        // 初始化市场数据区域
  void ConnectSignals();        // 连接信号槽
  // === 辅助函数 ===
  void UpdateArrowButtonsState();  // 更新箭头按钮状态
signals:
  void SubNavChanged(const NavState& state);
private slots:
  void OnMainNavButtonClicked(QAbstractButton* button); // 处理主导航按钮点击
  void OnSubNavButtonClicked();  // 处理子导航按钮点击
  void OnLeftArrowClicked();     // 处理币种区域左箭头点击
  void OnRightArrowClicked();    // 处理币种区域右箭头点击
  void UpdateMarketTable(const NavState& state);  // 更新市场数据表格
  void InitWebSocket(); // 初始化 WebSocket
  void OnWebSocketConnected(); // 处理 WebSocket 连接成功
  void OnWebSocketDisconnected(); // 处理 WebSocket 连接断开
  void OnWebSocketError(const QString& errorMessage); // 处理 WebSocket 错误
  void OnWebSocketMessageReceived(const QString& message); // 处理 WebSocket 消息接收
};
#endif // MAINWINDOW_H
