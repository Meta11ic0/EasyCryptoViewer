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
  void onTradeModeButtonClicked(QAbstractButton *button);
  void updateTableArea(int mode);
  void fetchMarketData();
  void onNetworkReply(QNetworkReply *reply);

private:
  void updateSpotTable(const QString &pair, const QJsonArray &dataArray);
  void updateFutureTable(const QString &pair, const QJsonArray &dataArray, const QString &type);
  void initUI();
  void connectSignals();

private:
  Ui::MainWindow *ui;
  QButtonGroup tradeModeButtonGroup;
  QStringList tradingPairsSpot, tradingPairsFuture;
  QNetworkAccessManager *networkManager;
  QTimer *updateTimer;
  QStandardItemModel *spotModel;
  QStandardItemModel *futureModel;
  int tradeMode;
};
#endif // MAINWINDOW_H
