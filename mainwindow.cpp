#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , tradeMode(-1)
    , ui(new Ui::MainWindow) {
  ui->setupUi(this);
  initResources();
  connectSignals();
  onTradeModeChanged(1);
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::initResources() {
  //导航栏
  tradeModeButtonGroup.addButton(ui->spotButton, 0);
  tradeModeButtonGroup.addButton(ui->futureButton, 1);
  tradeModeButtonGroup.setExclusive(true);
  //默认选中现货按钮
  ui->spotButton->click();
 //现货表格模型
  spotModel = new QStandardItemModel(this);
  spotModel->setHorizontalHeaderLabels({tr("产品"), tr("最新价"), tr("涨跌幅"), tr("24h最高"), tr("24h最低"), tr("成交量")});
 //合约表格模型
  futureModel = new QStandardItemModel(this);
  futureModel->setHorizontalHeaderLabels({tr("产品"), tr("最新价"), tr("涨跌幅"), tr("24h最高"), tr("24h最低"),tr("持仓量"), tr("成交量")});
 // 一开始默认显示现货表格
  ui->tableView->setModel(spotModel);
  ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
 // 初始化交易对列表
  tradingPairs = {"BTC-USDT", "ETH-USDT", "SOL-USDT", "DOGE-USDT"};
  // 初始化网络请求管理器
  networkManager = new QNetworkAccessManager(this);
  // 初始化计时器
  updateTimer = new QTimer(this);
}

void MainWindow::connectSignals() {
  connect(&tradeModeButtonGroup, &QButtonGroup::idClicked, this, &MainWindow::onTradeModeChanged);
  connect(updateTimer, &QTimer::timeout, this, &MainWindow::fetchData);
  connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onNetworkReply);
}

void MainWindow::onTradeModeChanged(int mode) {
  if(tradeMode == mode)
    return;
  tradeMode = mode;
  //停止定时器
  updateTimer->stop();
  ui->tableView->model()->removeRows(0, ui->tableView->model()->rowCount());
  if(tradeMode == 0)  { //现货交易
    qDebug() << "交易模式切换至现货交易" ;
    ui->statusBar->showMessage(tr("交易模式切换至现货交易"), 2000);
    ui->tableView->setModel(spotModel);
  }  else if( tradeMode == 1) { //合约交易
    qDebug() << "交易模式切换至合约交易";
    ui->statusBar->showMessage(tr("交易模式切换至合约交易"), 2000);
    ui->tableView->setModel(futureModel);
  } else { //未定义
    qDebug() << QString("交易模式切换至未定义 :%1").arg(tradeMode);
    ui->statusBar->showMessage(tr("交易模式切换至未定义: %1").arg(tradeMode), 2000);
  }
  //触发第一次获取数据
  fetchData();
  updateTimer->start(5000);
}

void MainWindow::fetchData() {
  // 遍历交易对发送请求
  for(const QString & pair : tradingPairs) {
    // 构建基础请求数据
    QVariantMap baseRequestData;
    baseRequestData["pair"] = pair;
    baseRequestData["tradeMode"] = tradeMode;
    if(tradeMode == 0) { //现货交易
      // 现货交易只需要获取ticker数据
      QVariantMap tickerData = baseRequestData;
      tickerData["type"] = "ticker";
      QNetworkRequest tickerRequest;
      QUrl tickerUrl = QUrl("https://www.okx.com/api/v5/market/ticker?instId=" + pair);
      tickerRequest.setUrl(tickerUrl);
      tickerRequest.setAttribute(QNetworkRequest::User, tickerData);
      networkManager->get(tickerRequest);
      qDebug() << "发送请求 " + tickerUrl.toString() ;
      ui->statusBar->showMessage(tr("发送请求 ") + tickerUrl.toString(), 2000);
    } else if(tradeMode == 1) { //合约交易
      // 合约交易需要获取ticker和持仓量数据
      QVariantMap tickerData = baseRequestData;
      tickerData["type"] = "ticker";
      QNetworkRequest tickerRequest;
      QUrl tickerUrl = QUrl("https://www.okx.com/api/v5/market/ticker?instId=" + pair + "-SWAP");
      tickerRequest.setUrl(tickerUrl);
      tickerRequest.setAttribute(QNetworkRequest::User, tickerData);
      networkManager->get(tickerRequest);
      qDebug() << "发送请求 " + tickerUrl.toString() ;
      ui->statusBar->showMessage(tr("发送请求 ") + tickerUrl.toString() , 2000);
      //持仓量查询
      QVariantMap openInterestData = baseRequestData;
      openInterestData["type"] = "openInterest";
      QNetworkRequest openInterestRequest;
      QUrl openInterestUrl = QUrl("https://www.okx.com/api/v5/public/open-interest?instId=" + pair + "-SWAP");
      openInterestRequest.setUrl(openInterestUrl);
      openInterestRequest.setAttribute(QNetworkRequest::User, openInterestData);
      networkManager->get(openInterestRequest);
      qDebug() << "发送请求 " + openInterestUrl.toString() ;
      ui->statusBar->showMessage(tr("发送请求 ") + openInterestUrl.toString() , 2000);
    } else { //未定义
      qDebug() << "tradeMode未定义 "  ;
      ui->statusBar->showMessage(tr("tradeMode未定义 ") , 2000);
    }
  }
}

void MainWindow::onNetworkReply(QNetworkReply *reply) {
  if(reply->error() != QNetworkReply::NoError) {
    qDebug() << "网络错误: " << reply->errorString();
    ui->statusBar->showMessage(tr("网络错误: ") + reply->errorString(), 2000);
    reply->deleteLater();
    return;
  }
  QVariantMap requestData = reply->request().attribute(QNetworkRequest::User).toMap();
  QString pair = requestData["pair"].toString();
  int mode = requestData["tradeMode"].toInt();
  QString type = requestData["type"].toString();
  // 确保数据是当前交易模式
  if(mode != tradeMode) {
    qDebug() << "数据与当前交易模式不符合，不做处理";
    ui->statusBar->showMessage(tr("数据与当前交易模式不符合，不做处理"), 2000);
    reply->deleteLater();
    return;
  }
  QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
  QJsonObject obj = doc.object();
  qDebug() << "返回数据：" << obj;
  QString erroMess;
  if (!obj.contains("data")) {
      erroMess = tr("未知错误，当前数据没有data域 : ") + obj["code"].toString(" ");
      qDebug() << erroMess;
      ui->statusBar->showMessage(erroMess, 2000);
      reply->deleteLater();
      return;
  }
  QJsonArray dataArray = obj["data"].toArray();
  if(dataArray.isEmpty()) {
    erroMess = tr("data域为空 ");
    qDebug() << erroMess;
    ui->statusBar->showMessage(erroMess, 2000);
    reply->deleteLater();
    return;
  }
  updateTableArea(mode, pair, type, dataArray[0].toObject());
  reply->deleteLater();
}

void MainWindow::updateTableArea(int mode, const QString &pair, const QString &type, const QJsonObject &data) {
  QStandardItemModel *model = qobject_cast<QStandardItemModel*>(ui->tableView->model());
  if(!model) return;
  // 查找行号
  int row = -1;
  for (int i = 0; i < model->rowCount(); ++i) {
    if (model->item(i, 0)->text().contains(pair)) {
      row = i;
      break;
    }
  }
  // 计算涨跌幅
  double last = data.value("last").toString().toDouble();
  double open = data.value("open24h").toString().toDouble();
  QString changeStr = open != 0 ? QString::asprintf("%.2f%%", (last - open) / open * 100.0) : "error";
  // 数据字段
  QString lastStr = data.value("last").toString("error");
  QString highStr = data.value("high24h").toString("error");
  QString lowStr = data.value("low24h").toString("error");
  QString volStr = data.value("volCcy24h").toString("error");
  QString openInterestStr = data.value("oiCcy").toString("error"); // 仅合约模式才有
  if (row == -1) {
    // 新行
    QList<QStandardItem *> newRow;
    newRow << new QStandardItem(pair)               // 产品
                 << new QStandardItem(lastStr)            // 最新价
                 << new QStandardItem(changeStr)          // 涨跌幅
                 << new QStandardItem(highStr)            // 24h最高
                 << new QStandardItem(lowStr);            // 24h最低
    if (mode == 0) {
      newRow << new QStandardItem(volStr);        // 成交量（现货）
    } else if (mode == 1) {
      newRow << new QStandardItem(openInterestStr) // 持仓量
                   << new QStandardItem(volStr);         // 成交量（合约）
    }
    model->appendRow(newRow);
  } else {
    // 更新已有行
    if (type == "ticker") {
        model->item(row, 1)->setText(lastStr);
        model->item(row, 2)->setText(changeStr);
        model->item(row, 3)->setText(highStr);
        model->item(row, 4)->setText(lowStr);
        if (mode == 0) {
            model->item(row, 5)->setText(volStr);
        } else if (mode == 1) {
            model->item(row, 6)->setText(volStr);
        }
    } else if (type == "openInterest" && mode == 1) {
        // 合约持仓量单独更新
        model->item(row, 5)->setText(openInterestStr);
    }
  }
  QString modeStr = (mode == 0) ? "现货" : "合约";
  qDebug() << modeStr << "交易表格更新数据：" << pair << " 类型：" << type;
  ui->statusBar->showMessage(tr("%1交易表格更新数据: %2").arg(modeStr, pair), 2000);
}
