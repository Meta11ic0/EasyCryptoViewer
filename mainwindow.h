#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include <QStandardItem>
#include <QTimer>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void onTradeModeChanged(int mode);
  void fetchData();
  void onNetworkReply(QNetworkReply *reply);

private:
  void initResources();
  void connectSignals();
  void updateTableArea(int mode, const QString &pair, const QString &type, const QJsonObject &data);

private:
  Ui::MainWindow *ui;
  QButtonGroup tradeModeButtonGroup;
  QStringList tradingPairs;
  QNetworkAccessManager *networkManager;
  QTimer *updateTimer;
  QStandardItemModel *spotModel;
  QStandardItemModel *futureModel;
  int tradeMode;
};
#endif // MAINWINDOW_H
