#ifndef SUBNAVIGATION_H
#define SUBNAVIGATION_H

/**
 * @file subnavigation.h
 * @brief 子导航栏类的头文件
 * @details 包含了币种、计价货币、保证金类型、合约类型等子导航功能
 */

#include <QWidget>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QScrollArea>
#include <QScrollBar>
#include <QDebug>

/**
 * @class SubNavigation
 * @brief 子导航栏类，负责管理各个子导航区域
 * @details 包含计价货币、保证金类型、币种、合约类型四个主要区域
 *         每个区域都可以独立显示/隐藏，币种区域支持滚动
 */
class SubNavigation : public QWidget
{
  Q_OBJECT
public:
  /**
   * @brief 构造函数
   * @param parent 父窗口部件
   */
  explicit SubNavigation(QWidget* parent = nullptr);
  ~SubNavigation();

  // 获取当前选中状态（带空指针检查）
  QString currentQuote() const { return quoteCombo ? quoteCombo->currentText() : QString(); }  // 获取当前计价货币
  int currentMarginType() const { return marginGroup ? marginGroup->checkedId() : -1; }        // 获取当前保证金类型
  int currentCoinType() const { return coinGroup ? coinGroup->checkedId() : -1; }              // 获取当前币种
  int currentFuturesType() const { return futuresGroup ? futuresGroup->checkedId() : -1; }     // 获取当前合约类型

public slots:
  // 控制区域显示/隐藏
  void showQuoteArea(bool show = true);    // 显示/隐藏计价货币区域
  void showMarginArea(bool show = true);   // 显示/隐藏保证金区域
  void showFuturesArea(bool show = true);  // 显示/隐藏合约类型区域

  // 设置当前选中状态
  void setCurrentQuote(const QString& quote);  // 设置当前计价货币
  void setCurrentMargin(int index);            // 设置当前保证金类型
  void setCurrentCoin(int index);              // 设置当前币种
  void setCurrentFutures(int index);           // 设置当前合约类型

signals:
  // 各个区域的变化信号
  void quoteChanged(const QString& quote);  // 计价货币变化信号
  void marginTypeChanged(int index);        // 保证金类型变化信号
  void coinTypeChanged(int index);          // 币种变化信号
  void futuresTypeChanged(int index);       // 合约类型变化信号

private slots:
  // 处理各个区域的事件
  void handleMarginButtonClicked(int id);   // 处理保证金按钮点击
  void handleCoinButtonClicked(int id);     // 处理币种按钮点击
  void handleFuturesButtonClicked(int id);  // 处理合约类型按钮点击
  void handleQuoteChanged(const QString& text);  // 处理计价货币变化
  void handleLeftArrowClicked();            // 处理左箭头点击
  void handleRightArrowClicked();           // 处理右箭头点击

private:
  // UI初始化相关函数
  void initUI();                           // 初始化整体UI
  void initCoinArea();                     // 初始化币种区域
  void initQuoteArea();                    // 初始化计价货币区域
  void initMarginArea();                   // 初始化保证金区域
  void initFuturesArea();                  // 初始化合约类型区域
  
  /**
   * @brief 创建导航按钮组
   * @param buttonGroup 按钮组指针的引用
   * @param buttons 按钮列表的引用
   * @param buttonTexts 按钮文本列表
   * @param layout 按钮容器的布局
   * @param buttonName 按钮的基础名称
   */
  void createNavigationButtons(QButtonGroup*& buttonGroup, 
                             QList<QPushButton*>& buttons, 
                             const QStringList& buttonTexts, 
                             QHBoxLayout* layout,
                             const QString& buttonName);
  
  void updateArrowButtonsState();          // 更新箭头按钮状态
  void connectSignalsAndSlots();           // 连接信号和槽

private:
  // 布局相关
  QHBoxLayout* layout = nullptr;           // 子导航总布局

  // 计价货币区域
  QWidget* quoteWidget = nullptr;          // 计价货币容器
  QHBoxLayout* quoteLayout = nullptr;      // 计价货币布局
  QLabel* quoteLabel = nullptr;            // "计价货币"标签
  QComboBox* quoteCombo = nullptr;         // 计价货币下拉框

  // 保证金区域
  QWidget* marginWidget = nullptr;         // 保证金容器
  QHBoxLayout* marginLayout = nullptr;     // 保证金布局
  QButtonGroup* marginGroup = nullptr;     // 保证金按钮组
  QList<QPushButton*> marginButtons;       // 保证金按钮列表

  // 币种区域（可滚动）
  QWidget* coinWidget = nullptr;           // 币种区域容器
  QHBoxLayout* coinLayout = nullptr;       // 币种区域布局
  QPushButton* leftArrowButton = nullptr;  // 左箭头按钮
  QPushButton* rightArrowButton = nullptr; // 右箭头按钮
  QScrollArea* coinScrollArea = nullptr;   // 可滚动区域
  QWidget* coinScrollContent = nullptr;    // 可滚动区域内容
  QHBoxLayout* coinScrollLayout = nullptr; // 可滚动区域布局
  QScrollBar* coinScrollBar = nullptr;     // 水平滚动条
  QButtonGroup* coinGroup = nullptr;       // 币种按钮组
  QList<QPushButton*> coinButtons;         // 币种按钮列表

  // 合约类型区域
  QWidget* futuresWidget = nullptr;        // 合约类型容器
  QHBoxLayout* futuresLayout = nullptr;    // 合约类型布局
  QButtonGroup* futuresGroup = nullptr;    // 合约类型按钮组
  QList<QPushButton*> futuresButtons;      // 合约类型按钮列表
};

#endif // SUBNAVIGATION_H
