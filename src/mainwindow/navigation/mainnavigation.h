/**
 * @file mainnavigation.h
 * @brief 主导航栏类的头文件
 * @details 包含了现货、合约等主要导航功能和搜索功能
 */

#ifndef MAINNAVIGATION_H
#define MAINNAVIGATION_H

#include <QWidget>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QLineEdit>
#include <QPushButton>
#include <QDebug>

/**
 * @class MainNavigation
 * @brief 主导航栏类，负责管理主要导航区域
 * @details 包含现货、合约等主导航按钮和搜索框
 */
class MainNavigation : public QWidget
{
  Q_OBJECT
public:
  /**
   * @brief 构造函数
   * @param parent 父窗口部件
   */
  explicit MainNavigation(QWidget* parent = nullptr);
  
  /**
   * @brief 析构函数
   */
  ~MainNavigation();

  /**
   * @brief 获取当前导航类型
   * @return 当前选中的导航按钮ID，如果无效返回-1
   */
  int currentNavType() const { return buttonGroup ? buttonGroup->checkedId() : -1; }

  /**
   * @brief 获取当前搜索文本
   * @return 搜索框中的文本，如果无效返回空字符串
   */
  QString currentSearchText() const { return searchBox ? searchBox->text() : QString(); }

signals:
  /**
   * @brief 导航类型变化信号
   * @param index 新选中的导航按钮ID
   */
  void navigationChanged(int index);

  /**
   * @brief 搜索文本变化信号
   * @param text 新的搜索文本
   */
  void searchTextChanged(const QString& text);

public slots:
  /**
   * @brief 设置当前选中的按钮
   * @param index 要选中的按钮索引
   */
  void setCurrentButton(int index);

  /**
   * @brief 设置搜索框的文本
   * @param text 要设置的文本
   */
  void setSearchText(const QString& text);

private slots:
  /**
   * @brief 处理导航按钮点击
   * @param id 被点击按钮的ID
   */
  void handleButtonClicked(int id);

  /**
   * @brief 处理搜索框文本变化
   * @param text 搜索框的新文本
   */
  void handleSearchTextChanged(const QString& text);

private:
  /**
   * @brief 初始化UI组件
   * @details 设置布局、创建按钮和搜索框
   */
  void initUI();

  /**
   * @brief 创建导航按钮
   * @details 创建现货、合约等主导航按钮，设置按钮属性和样式
   */
  void createNavigationButtons();

  /**
   * @brief 连接信号和槽
   * @details 建立所有需要的信号槽连接
   */
  void connectSignalsAndSlots();

private:
  QHBoxLayout* layout = nullptr;            // 主导航总布局
  QButtonGroup* buttonGroup = nullptr;      // 导航按钮组
  QList<QPushButton*> navigationButtons;    // 导航按钮列表
  QLineEdit* searchBox = nullptr;           // 搜索框
};

#endif // MAINNAVIGATION_H
