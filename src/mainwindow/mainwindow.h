#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "navigation/mainnavigation.h"
#include "navigation/subnavigation.h"
#include "markettable/markettable.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
};
#endif // MAINWINDOW_H
