#ifndef MAINNAVIGATION_H
#define MAINNAVIGATION_H

#include <QWidget>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QLineEdit>
#include <QPushButton>
#include <QDebug>

class MainNavigation : public QWidget
{
  Q_OBJECT
public:
  explicit MainNavigation(QWidget* parent = nullptr);
  ~MainNavigation();
signals:
  // 导航按钮点击信号
  void navigationChanged(int index);
  // 搜索内容变化信号
  void searchTextChanged(const QString& text);
private slots:
  // 处理按钮点击
  void handleButtonClicked(int id);
  // 处理搜索框文本变化
  void handleSearchTextChanged(const QString& text);
private:
  // 初始化UI组件
  void initUI();
  // 创建导航按钮
  void createNavigationButtons();
  // 连接信号和槽
  void connectSignalsAndSlots();  
private:
  QHBoxLayout* layout;       
  QButtonGroup* buttonGroup; 
  QList<QPushButton*> navigationButtons;
  QLineEdit* searchBox;
};

#endif // MAINNAVIGATION_H
