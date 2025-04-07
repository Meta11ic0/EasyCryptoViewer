#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
   : QMainWindow(parent)
    , tradeMode(-1)
    , ui(new Ui::MainWindow) {
  ui->setupUi(this);
  initUI();
 // 初始化交易对列表
  tradingPairsSpot = {"BTC-USDT", "ETH-USDT", "SOL-USDT", "DOGE-USDT"};
  tradingPairsFuture = {"BTC-USDT-SWAP", "ETH-USDT-SWAP", "SOL-USDT-SWAP", "DOGE-USDT-SWAP"};
  // 初始化网络请求管理器
  networkManager = new QNetworkAccessManager(this);
  // 初始化计时器
  updateTimer = new QTimer(this);
  //链接所有信号
  connectSignals();
  // 第一次手动触发
  onTradeModeButtonClicked(ui->spotButton);
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::initUI() {
  //导航栏按钮组
  tradeModeButtonGroup.addButton(ui->spotButton, 0);
  tradeModeButtonGroup.addButton(ui->futureButton, 1);
  tradeModeButtonGroup.setExclusive(true);
 //现货表格模型
  QStandardItemModel *spotModel = new QStandardItemModel(this);
  spotModel->setHorizontalHeaderLabels({tr("产品"), tr("最新价"), tr("涨跌幅"), tr("24h最高"), tr("24h最低"), tr("成交量")});
  ui->spotTableView->setModel(spotModel);
  ui->spotTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  //合约表格模型
  QStandardItemModel *futureModel = new QStandardItemModel(this);
  futureModel->setHorizontalHeaderLabels({tr("产品"), tr("最新价"), tr("涨跌幅"), tr("24h最高"), tr("24h最低"),tr("持仓量"), tr("成交量")});
  ui->futureTableView->setModel(futureModel);
  ui->futureTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::connectSignals() {
  connect(&tradeModeButtonGroup, &QButtonGroup::buttonClicked, this, &MainWindow::onTradeModeButtonClicked);
  connect(this, &MainWindow::tradeModeChanged, this, &MainWindow::updateTableArea);
  connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onNetworkReply);
  connect(updateTimer, &QTimer::timeout, this, &MainWindow::fetchMarketData);
}

void MainWindow::onTradeModeButtonClicked(QAbstractButton *button) {
  int mode = tradeModeButtonGroup.id(button);  
  if (mode == 0) {
    ui->spotTableView->setVisible(true);
    ui->futureTableView->setVisible(false);
    ui->statusBar->showMessage(tr("现货交易"), 2000);
  } else if (mode == 1) {
    ui->spotTableView->setVisible(false);
    ui->futureTableView->setVisible(true);
    ui->statusBar->showMessage(tr("合约交易"), 2000);
  }
  emit tradeModeChanged(mode);
}

void MainWindow::updateTableArea(int mode) {
  if(tradeMode == mode)
    return;
  tradeMode = mode;
  //停止定时器
  updateTimer->stop();
  // 清空表格数据
  if (tradeMode == 0) {
    ui->spotTableView->model()->removeRows(0, ui->spotTableView->model()->rowCount());
  } else if (tradeMode == 1) {
    ui->futureTableView->model()->removeRows(0, ui->futureTableView->model()->rowCount());
  }
  qDebug() << "交易模式切换至" << (tradeMode == 0 ? "现货" : "合约");
  // 触发第一次数据请求
  fetchMarketData();
  updateTimer->start(3000);
}

void MainWindow::fetchMarketData() {
  qDebug() << "开始获取市场数据...";
  // 根据交易模式选择不同的交易对列表
  const QStringList& tradingPairs = (tradeMode == 0) ? tradingPairsSpot : tradingPairsFuture;
  // 遍历交易对发送请求
  for (const QString &pair : tradingPairs) {
    // 构建基础请求数据
    QVariantMap baseRequestData;
    baseRequestData["pair"] = pair;
    baseRequestData["tradeMode"] = tradeMode;
    // 现货交易只需要获取ticker数据
    if (tradeMode == 0) {
      QNetworkRequest tickerRequest;
      QVariantMap tickerData = baseRequestData;
      tickerData["messageType"] = "ticker";
      tickerRequest.setUrl(QUrl("https://www.okx.com/api/v5/market/ticker?instId=" + pair));
      tickerRequest.setAttribute(QNetworkRequest::User, tickerData);
      networkManager->get(tickerRequest);
    } 
    // 合约交易需要获取ticker和持仓量数据
    else {
      // 构建ticker请求
      QNetworkRequest tickerRequest;
      QVariantMap tickerData = baseRequestData;
      tickerData["messageType"] = "ticker";
      // 构建持仓量请求
      QNetworkRequest openInterestRequest;
      QVariantMap openInterestData = baseRequestData;
      openInterestData["messageType"] = "openInterest";
      // 设置请求URL
      tickerRequest.setUrl(QUrl("https://www.okx.com/api/v5/market/ticker?instId=" + pair));
      openInterestRequest.setUrl(QUrl("https://www.okx.com/api/v5/public/open-interest?instType=SWAP&instId=" + pair));
      // 设置请求属性
      tickerRequest.setAttribute(QNetworkRequest::User, tickerData);
      openInterestRequest.setAttribute(QNetworkRequest::User, openInterestData);
      // 发送请求
      networkManager->get(tickerRequest);
      networkManager->get(openInterestRequest);
    }
  }
}

void MainWindow::onNetworkReply(QNetworkReply *reply) {
  if (reply->error() != QNetworkReply::NoError) {
    qDebug() << "网络错误: " << reply->errorString();
    reply->deleteLater();
    return;
  }
  QVariantMap requestData = reply->request().attribute(QNetworkRequest::User).toMap();
  QString pair = requestData["pair"].toString();
  int mode = requestData["tradeMode"].toInt();
  QString type = requestData["messageType"].toString();  // ticker 或 openInteresting
  if (mode != tradeMode) {  // 确保数据是当前交易模式
    reply->deleteLater();
    return;
  }
  QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
  QJsonObject obj = doc.object();
  if (!obj.contains("data")) {
    reply->deleteLater();
    return;
  }
  QJsonArray dataArray = obj["data"].toArray();
  if (dataArray.isEmpty()) {
    reply->deleteLater();
    return;
  }
  QJsonObject data = dataArray[0].toObject();
  if (mode == 0) {
    updateSpotTable(pair, dataArray);
  } else if (mode == 1) {
    updateFutureTable(pair, dataArray, type);
  }
  reply->deleteLater();
}


void MainWindow::updateSpotTable(const QString &pair, const QJsonArray &dataArray) {
  QStandardItemModel *model = qobject_cast<QStandardItemModel *>(ui->spotTableView->model());
  if (!model) return;
  QJsonObject data = dataArray[0].toObject();
  double last = data["last"].toString().toDouble();
  double open = data["open24h"].toString().toDouble();
  double change = (last - open) / open * 100.0;
  for (int i = 0; i < model->rowCount(); ++i) {
    if (model->item(i, 0)->text() == pair) {
    //  spotModel->setHorizontalHeaderLabels({tr("产品"), tr("最新价"),
    //  tr("涨跌幅"), tr("24h最高"), tr("24h最低"), tr("成交量")});
      model->item(i, 1)->setText(data["last"].toString());
      model->item(i, 2)->setText(QString::asprintf("%.2f%%", change));
      model->item(i, 3)->setText(data["high24h"].toString());
      model->item(i, 4)->setText(data["low24h"].toString());
      model->item(i, 5)->setText(data["volCcy24h"].toString());
      return;
    }
  }
  QList<QStandardItem *> row;
  row << new QStandardItem(pair);
  row << new QStandardItem(data["last"].toString());
  row << new QStandardItem(QString::asprintf("%.2f%%", change));
  row << new QStandardItem(data["high24h"].toString());
  row << new QStandardItem(data["low24h"].toString());
  row << new QStandardItem(data["volCcy24h"].toString());
  model->appendRow(row);
}

void MainWindow::updateFutureTable(const QString &pair, const QJsonArray &dataArray, const QString &type) {
  QStandardItemModel *model = qobject_cast<QStandardItemModel *>(ui->futureTableView->model());
  if (!model) return;
  QJsonObject data = dataArray[0].toObject();
  int row = -1;
  for (int i = 0; i < model->rowCount(); ++i) {
    if (model->item(i, 0)->text() == pair) {
      row = i;
      break;
    }
  }
  if (row == -1) {
      QList<QStandardItem *> newRow;
      newRow << new QStandardItem(pair);                              // 0 产品
      newRow << new QStandardItem(type == "ticker" ? data["last"].toString() : "-"); // 1 最新价
      newRow << new QStandardItem("-");                               // 2 涨跌幅（你还没处理）
      newRow << new QStandardItem(type == "ticker" ? data["high24h"].toString() : "-");  // 3 高
      newRow << new QStandardItem(type == "ticker" ? data["low24h"].toString() : "-");   // 4 低
      newRow << new QStandardItem(type == "openInterest" ? data["openInterest"].toString() : "-"); // 5 持仓量
      newRow << new QStandardItem(type == "ticker" ? data["volCcy24h"].toString() : "-");  // 6 成交量
      model->appendRow(newRow);
      return;
  }
  // 更新已有行的数据
  if (type == "ticker") {
    model->item(row, 1)->setText(data["last"].toString());  // 更新最新价格
  } else if (type == "openInterest") {
    model->item(row, 5)->setText(data["openInterest"].toString());  // 设置持仓量
  }
}

