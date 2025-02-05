#ifndef MAINNAVIGATION_H
#define MAINNAVIGATION_H

#include <QWidget>

class MainNavigation : public QWidget
{
   Q_OBJECT
public:
  MainNavigation(QWidget *parent = nullptr);
  ~MainNavigation();
private:
  QHBoxLayout layout;       // 主导航布局
  QButtonGroup buttonGroup; // 主导航按钮组
  QLineEdit searchBox ;     // 搜索框
};

#endif // MAINNAVIGATION_H
