#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  InitMainWindow();
}

MainWindow::~MainWindow() {
  delete marketTable;
  // 其他 new 出来的对象会由 Qt 的父子关系自动处理
}
// === 初始化 ===
/**
 * @brief 设置主窗口
 *
 * 初始化主窗口的基本布局和组件：
 * 1. 创建并设置中心部件
 * 2. 创建主布局（垂直布局）
 * 3. 加载样式表
 * 4. 创建三个主要区域：主导航、子导航、市场数据
 * 5. 连接信号槽
 * 6. 设置窗口基本属性
 */
void MainWindow::InitMainWindow() {
  // 创建中心部件
  centralWidget = new QWidget(this);
  setCentralWidget(centralWidget);
  // 创建主布局
  centralLayout = new QVBoxLayout(centralWidget);
  centralLayout->setSpacing(5);
  centralLayout->setContentsMargins(0, 0, 0, 0);
  // 创建组件
  InitMainNavArea();
  InitSubNavArea();
  InitMarketArea();
  // 初始化 WebSocket
  InitWebSocket();
  // 连接信号槽
  ConnectSignals();
  // 设置窗口属性
  setWindowTitle(tr("Crypto Market Viewer"));
  setMinimumSize(1200, 800);  // 设置最小尺寸
  setMaximumSize(1200, 800);  // 设置最大尺寸
}

/**
 * @brief 创建主导航区域
 *
 * 创建窗口顶部的主导航栏：
 * 1. 创建容器和水平布局
 * 2. 创建按钮组（互斥）
 * 3. 创建导航按钮（包括特殊的 New 标签）
 * 4. 添加弹性空间
 * 5. 创建搜索框
 * 6. 添加到主布局
 */
void MainWindow::InitMainNavArea() {
  // 创建主导航容器和布局
  mainNavWidget = new QWidget(this);
  mainNavWidget->setObjectName("mainNavWidget");
  mainNavLayout = new QHBoxLayout(mainNavWidget);
  mainNavLayout->setContentsMargins(10, 5, 10, 5);
  mainNavLayout->setSpacing(5);
  // 创建主导航按钮组
  mainNavButtonGroup = new QButtonGroup(this);
  mainNavButtonGroup->setExclusive(true);
  for (int  i = 0; i< 7; i++) {
    auto text = mainNavButtonTexts[i];
    auto btn = new QPushButton(text, mainNavWidget);
    btn->setCheckable(true);
    btn->setObjectName(QString("mainNavButton_%1").arg(static_cast<int>(i)));
    btn->setProperty("buttonId", QVariant::fromValue(static_cast<int>(i)));
    btn->setProperty("class", "mainnavbutton");
    mainNavLayout->addWidget(btn);
    mainNavButtonGroup->addButton(btn);
    // 设置按钮的大小策略
    btn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    btn->setFixedHeight(30);
  }
  // 设置默认选中第一个按钮
  if (auto firstBtn = mainNavButtonGroup->buttons().first()) {
    firstBtn->setChecked(true);
  }
  // 添加弹性空间
  mainNavLayout->addStretch();
  // 创建搜索框
  mainNavSearchBox = new QLineEdit(mainNavWidget);
  mainNavSearchBox->setObjectName("searchBox");
  mainNavSearchBox->setPlaceholderText("搜索币种/合约");
  mainNavSearchBox->setFixedWidth(200);
  mainNavSearchBox->setProperty("class", "searchbox");
  mainNavLayout->addWidget(mainNavSearchBox);
  // 将主导航添加到主布局
  centralLayout->addWidget(mainNavWidget);
  // 设置主导航容器的大小策略
  mainNavWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  mainNavWidget->setFixedSize(1200, 50);
}

/**
 * @brief 创建子导航区域
 *
 * 创建主导航下方的子导航栏：
 * 1. 创建容器和水平布局
 * 2. 创建按钮组（互斥）
 * 3. 创建默认的子导航按钮
 * 4. 设置默认选中状态
 * 5. 添加到主布局
 */
void MainWindow::InitSubNavArea() {
  // 1. 创建容器和布局
  subNavWidget = new QWidget(this);
  subNavWidget->setObjectName("subNavWidget");
  subNavLayout = new QHBoxLayout(subNavWidget);
  subNavLayout->setContentsMargins(10, 0, 10, 0);  // 垂直边距设为0
  subNavLayout->setSpacing(5);                     // 使用统一的间距
  // 2. 初始化各个区域
  InitSubNavQuoteArea();    // 计价货币区域
  InitSubNavMarginArea();   // 保证金区域
  InitSubNavCoinArea();     // 币种区域
  InitSubNavFuturesArea();  // 合约类型区域
  // 3. 添加到主布局
  centralLayout->addWidget(subNavWidget);
  // 4. 默认全部隐藏
  subNavQuoteWidget->hide();
  subNavMarginWidget->hide();
  subNavFuturesWidget->hide();
  // 设置子导航容器的大小策略
  subNavWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  subNavWidget->setFixedSize(1200, 50);
}

/**
 * @brief 初始化计价货币区域
 *
 * 创建计价货币区域的所有组件：
 * 1. 创建容器和布局
 * 2. 创建"计价货币"标签
 * 3. 创建下拉框并添加选项
 * 4. 添加到主布局
 */
void MainWindow::InitSubNavQuoteArea() {
  // 创建计价货币区域容器和布局
  subNavQuoteWidget = new QWidget(this);
  subNavQuoteWidget->setObjectName("subNavQuoteWidget");
  subNavQuoteLayout = new QHBoxLayout(subNavQuoteWidget);
  subNavQuoteLayout->setContentsMargins(0, 0, 0, 0);
  subNavQuoteLayout->setSpacing(5);
  // 创建标签
  subNavQuoteLabel = new QLabel(NavQuoteLabel, subNavQuoteWidget);
  subNavQuoteLabel->setObjectName("subNavQuoteLabel");
  subNavQuoteLayout->addWidget(subNavQuoteLabel);
  // 创建下拉框
  subNavQuoteCombo = new QComboBox(subNavQuoteWidget);
  subNavQuoteCombo->setObjectName("subNavQuoteCombo");
  subNavQuoteCombo->setProperty("class", "subnavquotecombo");
  // 添加计价货币选项
  for (int i = 0; i < 3; i++) {subNavQuoteCombo->addItem(quoteButtonTexts[i]); }
  // 设置默认选中第一项
  subNavQuoteCombo->setCurrentIndex(0);
  subNavQuoteLayout->addWidget(subNavQuoteCombo);
  // 添加到子导航布局
  subNavLayout->addWidget(subNavQuoteWidget);
  // 设置固定宽度
  subNavQuoteWidget->setFixedWidth(150);
  subNavQuoteWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  // 设置下拉框宽度
  subNavQuoteCombo->setFixedWidth(100);
}

/**
 * @brief 初始化保证金类型区域
 *
 * 创建保证金类型区域的所有组件：
 * 1. 创建容器和布局
 * 2. 创建按钮组（互斥）
 * 3. 创建保证金类型按钮
 * 4. 添加到主布局
 */
void MainWindow::InitSubNavMarginArea() {
  // 创建保证金区域容器和布局
  subNavMarginWidget = new QWidget(this);
  subNavMarginWidget->setObjectName("subNavMarginWidget");
  subNavMarginLayout = new QHBoxLayout(subNavMarginWidget);
  subNavMarginLayout->setContentsMargins(0, 0, 0, 0);
  subNavMarginLayout->setSpacing(5);
  // 创建按钮组
  subNavMarginGroup = new QButtonGroup(subNavMarginWidget);
  subNavMarginGroup->setExclusive(true);
  for (int  i = 0; i< 3; i++) {
    auto text = marginButtonTexts[i];
    auto btn = new QPushButton(text, subNavMarginWidget);
    btn->setCheckable(true);
    btn->setObjectName(QString("MarginButton_%1").arg(static_cast<int>(i)));
    btn->setProperty("buttonId", QVariant::fromValue(static_cast<int>(i)));
    btn->setProperty("class", "marginbutton");
    subNavMarginLayout->addWidget(btn);
    subNavMarginGroup->addButton(btn);
    // 设置按钮的大小策略
    btn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    btn->setFixedHeight(30);
  }
  // 设置默认选中第一个按钮
  if (auto firstBtn = subNavMarginGroup->buttons().first()) {
    firstBtn->setChecked(true);
  }
  // 添加到子导航布局
  subNavLayout->addWidget(subNavMarginWidget);
  // 设置固定宽度;
  subNavMarginWidget->setFixedWidth(200);
  subNavMarginWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

/**
 * @brief 初始化币种导航区域
 *
 * 创建币种导航区域的所有组件：
 * 1. 创建容器和布局
 * 2. 创建左箭头按钮（默认隐藏）
 * 3. 创建滚动区域
 * 4. 创建滚动内容容器
 * 5. 创建按钮组
 * 6. 创建币种按钮
 * 7. 设置滚动区域内容
 * 8. 创建右箭头按钮（默认隐藏）
 * 9. 添加到主布局
 */
void MainWindow::InitSubNavCoinArea() {
  // 创建币种区域容器和布局
  subNavCoinWidget = new QWidget(this);
  subNavCoinWidget->setObjectName("subNavCoinWidget");
  subNavCoinLayout = new QHBoxLayout(subNavCoinWidget);
  subNavCoinLayout->setContentsMargins(0, 0, 0, 0);
  subNavCoinLayout->setSpacing(0);
  // 创建左箭头按钮
  leftArrowButton = new QPushButton("<", subNavCoinWidget);
  leftArrowButton->setObjectName("leftArrowButton");
  leftArrowButton->setFixedSize(20, 30);
  leftArrowButton->show();
  subNavCoinLayout->addWidget(leftArrowButton);
  // 创建滚动区域
  subNavCoinScrollArea = new QScrollArea(subNavCoinWidget);
  subNavCoinScrollArea->setObjectName("CoinScrollArea");
  subNavCoinScrollArea->setWidgetResizable(true);
  subNavCoinScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  subNavCoinScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  // 获取滚动条
  subNavCoinScrollBar = subNavCoinScrollArea->horizontalScrollBar();
  // 创建滚动区域的内容容器
  subNavCoinScrollContent = new QWidget(subNavCoinScrollArea);
  subNavCoinScrollContent->setObjectName("CoinScrollContent");
  subNavCoinScrollLayout = new QHBoxLayout(subNavCoinScrollContent);
  subNavCoinScrollLayout->setContentsMargins(0, 0, 0, 0);
  subNavCoinScrollLayout->setSpacing(0);
  subNavCoinScrollLayout->setAlignment(Qt::AlignVCenter);  // 垂直居中对齐
  subNavCoinScrollArea->setFixedHeight(30);
  // 创建按钮组并添加按钮
  subNavCoinGroup = new QButtonGroup(this);
  subNavCoinGroup->setExclusive(true);
  // 创建币种按钮
  for (int  i = 0; i< 22; i++) {
    auto text = coinButtonTexts[i];
    auto btn = new QPushButton(text, subNavCoinScrollContent);
    btn->setCheckable(true);
    btn->setObjectName(QString("coinButton_%1").arg(static_cast<int>(i)));
    btn->setProperty("buttonId", QVariant::fromValue(static_cast<int>(i)));
    btn->setProperty("class", "coinbutton");
    subNavCoinScrollLayout->addWidget(btn);
    subNavCoinGroup->addButton(btn);
    // 设置按钮的大小策略
    btn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    btn->setFixedHeight(30);
  }
  // 设置默认选中第一个按钮
  if (auto firstBtn = subNavCoinGroup->buttons().first()) {
    firstBtn->setChecked(true);
  }
  // 设置滚动区域的内容
  subNavCoinScrollArea->setWidget(subNavCoinScrollContent);
  subNavCoinLayout->addWidget(subNavCoinScrollArea);
  // 创建右箭头按钮
  rightArrowButton = new QPushButton(">", subNavCoinWidget);
  rightArrowButton->setObjectName("rightArrowButton");
  rightArrowButton->setFixedSize(20, 30);
  rightArrowButton->show();  
  subNavCoinLayout->addWidget(rightArrowButton);
  // 添加到子导航栏布局
  subNavLayout->addWidget(subNavCoinWidget);
  // 设置币种区域为自适应
  subNavCoinWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  subNavCoinScrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  subNavCoinScrollArea->setAlignment(Qt::AlignVCenter);
  // 设置滚动内容容器的大小策略
  subNavCoinScrollContent->setSizePolicy  (QSizePolicy::Preferred, QSizePolicy::Preferred);
}


/**
 * @brief 初始化合约类型区域
 *
 * 创建合约类型区域的所有组件：
 * 1. 创建容器和布局
 * 2. 创建按钮组（互斥）
 * 3. 创建合约类型按钮
 * 4. 添加到主布局
 */
void MainWindow::InitSubNavFuturesArea() {
  // 创建合约类型区域容器和布局
  subNavFuturesWidget = new QWidget(this);
  subNavFuturesWidget->setObjectName("subNavFuturesWidget");
  subNavFuturesLayout = new QHBoxLayout(subNavFuturesWidget);
  subNavFuturesLayout->setContentsMargins(0, 0, 0, 0);
  subNavFuturesLayout->setSpacing(5);
  // 创建按钮组
  subNavFuturesGroup = new QButtonGroup(this);
  subNavFuturesGroup->setExclusive(true);
  for (int  i = 0; i< 3; i++) {
    auto text = futuresButtonTexts[i];
    auto btn = new QPushButton(text, subNavFuturesWidget);
    btn->setCheckable(true);
    btn->setObjectName(QString("futuresButton_%1").arg(static_cast<int>(i)));
    btn->setProperty("buttonId", QVariant::fromValue(static_cast<int>(i)));
    btn->setProperty("class", "futuresButton");
    subNavFuturesLayout->addWidget(btn);
    subNavFuturesGroup->addButton(btn);
    // 设置按钮的大小策略
    btn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    btn->setFixedHeight(30);
  }
  // 设置默认选中第一个按钮
  if (auto firstBtn = subNavFuturesGroup->buttons().first()) {
    firstBtn->setChecked(true);
  }
  // 添加到子导航布局
  subNavLayout->addWidget(subNavFuturesWidget);
  // 设置固定宽度
  subNavFuturesWidget->setFixedWidth(200);
  subNavFuturesWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

/**
 * @brief 创建市场数据区域
 *
 * 创建窗口主体的市场数据显示区：
 * 1. 创建容器和垂直布局
 * 2. 创建表格控件
 * 3. 设置表格基本属性和样式
 * 4. 添加到主布局
 */
void MainWindow::InitMarketArea() {
  // 创建市场数据容器和布局
  marketWidget = new QWidget(this);
  marketWidget->setObjectName("marketWidget");
  marketLayout = new QVBoxLayout(marketWidget);
  marketLayout->setContentsMargins(10, 0, 10, 0);
  marketLayout->setSpacing(0);
  // 创建默认表格（币种-全部）
  marketTable = MarketTableFactory::createTable(NavState());  
  marketLayout->addWidget(marketTable);
  // 添加到主布局
  centralLayout->addWidget(marketWidget);
  // 设置市场数据区域的大小策略
  marketWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

/**
 * @brief 初始化 WebSocket
 *
 * 创建 OKXWebSocket 对象并连接到 WebSocket 服务器:
 * 1. 创建 WebSocket 对象
 * 2. 连接到服务器
 * 3. 处理连接结果
 */
void MainWindow::InitWebSocket() {
  // 创建 WebSocket 对象
  okxWebSocket = new OKXWebSocket(this);
  // 连接到 OKX WebSocket 服务器
  while(!okxWebSocket->connectToServer(WS_PUBLIC_URL)){
    qDebug() << "WebSocket 连接失败";
  }
}

// === 信号槽连接 ===
/**
 * @brief 连接所有信号和槽
 *
 * 建立界面各组件之间的信号连接：
 * 1. 主导航按钮点击 -> 更新子导航
 * 2. 主导航变化信号 -> 更新子导航区域
 * 3. 子导航按钮点击 -> 更新表格
 * 4. 子导航变化信号 -> 更新表格数据
 */
void MainWindow::ConnectSignals() {
  // 主导航按钮点击 -> 更新子导航
  connect(mainNavButtonGroup, &QButtonGroup::buttonClicked,
          this, &MainWindow::OnMainNavButtonClicked);
  // 子导航信号连接
  connect(subNavCoinGroup, &QButtonGroup::buttonClicked,
          this, &MainWindow::OnSubNavButtonClicked);
  connect(subNavMarginGroup, &QButtonGroup::buttonClicked,
          this, &MainWindow::OnSubNavButtonClicked);  
  connect(subNavFuturesGroup, &QButtonGroup::buttonClicked,
          this, &MainWindow::OnSubNavButtonClicked);
  connect(subNavQuoteCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
          this, &MainWindow::OnSubNavButtonClicked);
  // 左右箭头点击 -> 滚动币种区域
  connect(leftArrowButton, &QPushButton::clicked,
          this, &MainWindow::OnLeftArrowClicked);
  connect(rightArrowButton, &QPushButton::clicked,
          this, &MainWindow::OnRightArrowClicked);
  // 添加子导航变化 -> 更新表格的连接
  connect(this, &MainWindow::SubNavChanged,
          this, &MainWindow::UpdateMarketTable);
  // 连接 WebSocket 信号
  connect(okxWebSocket, &OKXWebSocket::connected, this, &MainWindow::OnWebSocketConnected);
  connect(okxWebSocket, &OKXWebSocket::disconnected, this, &MainWindow::OnWebSocketDisconnected);
  connect(okxWebSocket, &OKXWebSocket::errorOccurred, this, &MainWindow::OnWebSocketError);
  connect(okxWebSocket, &OKXWebSocket::tickerDataReceived, this, &MainWindow::OnWebSocketMessageReceived);
}

/**
 * @brief 添加一个新的私有函数来更新箭头按钮状态
 *
 * 根据当前滚动条位置和按钮状态，更新箭头按钮的可见性
 */
void MainWindow::UpdateArrowButtonsState() {
  // 计算内容总宽度
  int contentWidth = subNavCoinScrollContent->sizeHint().width();
  int viewportWidth = subNavCoinScrollArea->viewport()->width();
  // 根据内容宽度决定是否显示箭头
  bool needArrows = contentWidth > viewportWidth;
  leftArrowButton->setVisible(needArrows && subNavCoinScrollBar->value() > 0);
  rightArrowButton->setVisible(needArrows && subNavCoinScrollBar->value() < subNavCoinScrollBar->maximum());
}

/**
 * @brief 更新市场数据表格
 * @param state 导航状态
 *
 * 根据当前导航状态更新表格：
 * 1. 清理现有表格
 * 2. 创建新表格
 * 3. 添加到布局
 */
void MainWindow::UpdateMarketTable(const NavState& state) {
  // 清理现有表格
  if (marketTable) {
    if (marketTable->parent()) {
      marketLayout->removeWidget(marketTable);
    }
    marketTable->deleteLater();  // 使用 deleteLater 更安全
    marketTable = nullptr;
  }
  
  // 创建新表格
  marketTable = MarketTableFactory::createTable(state);
  
  // 添加到布局
  marketLayout->addWidget(marketTable);
  
  // TODO: 加载数据
  // LoadMarketData(state);
}

// === 按钮点击相关 ===
/**
 * @brief 处理主导航按钮点击事件
 * @param button 被点击的按钮
 *
 * 当主导航按钮被点击时：
 * 1. 验证按钮有效性
 * 2. 获取导航ID并转换为枚举类型
 * 3. 隐藏所有子导航区域
 * 4. 根据主导航类型显示对应的子导航区域
 * 5. 触发子导航状态更新
 */
void MainWindow::OnMainNavButtonClicked(QAbstractButton* button) {
  if (!button || !button->property("buttonId").isValid()) {
    qDebug() << "主导航按钮点击无效";
    return;
  }
  const MainNavType mainNavId = static_cast<MainNavType>(
    button->property("buttonId").value<int>()
  );
  qDebug() << "主导航按钮点击 - ID:" << static_cast<int>(mainNavId) 
           << "文本:" << button->text();
  subNavQuoteWidget->hide();
  subNavMarginWidget->hide();
  subNavFuturesWidget->hide();
  switch (mainNavId) {
    case MainNavType::COIN:
      subNavCoinWidget->show();
      break;
    case MainNavType::SPOT:
      subNavCoinWidget->show();
      subNavQuoteWidget->show();
      break;
    case MainNavType::FUTURES:
      subNavCoinWidget->show();
      subNavMarginWidget->show();
      subNavFuturesWidget->show();
      break;
  }
  OnSubNavButtonClicked();
}

/**
 * @brief 处理币种区域左箭头按钮点击
 *
 * 当左箭头按钮被点击时：
 * 1. 计算滚动区域的新位置
 * 2. 平滑滚动到新位置
 * 3. 更新箭头按钮状态
 */
void MainWindow::OnLeftArrowClicked() {
  int scrollStep = subNavCoinScrollContent->width() / 3;
  int oldValue = subNavCoinScrollBar->value();
  subNavCoinScrollBar->setValue(oldValue - scrollStep);
  qDebug() << "左箭头点击 - 滚动值从" << oldValue << "变为" << subNavCoinScrollBar->value();
  UpdateArrowButtonsState();
}

/**
 * @brief 处理币种区域右箭头按钮点击
 *
 * 当右箭头按钮被点击时：
 * 1. 计算滚动区域的新位置
 * 2. 平滑滚动到新位置
 * 3. 更新箭头按钮状态
 */
void MainWindow::OnRightArrowClicked() {
  int scrollStep = subNavCoinScrollContent->width() / 6;
  int oldValue = subNavCoinScrollBar->value();
  subNavCoinScrollBar->setValue(oldValue + scrollStep);
  qDebug() << "右箭头点击 - 滚动值从" << oldValue << "变为" << subNavCoinScrollBar->value();
  UpdateArrowButtonsState();
}

/**
 * @brief 处理子导航变化事件
 * 
 * 当子导航区域的任何控件状态发生变化时触发此函数：
 * 1. 收集所有子导航区域的当前状态
 * 2. 构建导航状态对象
 * 3. 发送状态变化信号
 */
void MainWindow::OnSubNavButtonClicked() {
  if (!mainNavButtonGroup || !subNavCoinGroup || !subNavQuoteCombo) {
    qDebug() << "关键控件为空，无法更新导航状态";
    return;
  }
  NavState state;
  state.mainNavId = static_cast<MainNavType>(
    mainNavButtonGroup->checkedButton()->property("buttonId").value<int>()
  );
  state.coinId = static_cast<CoinType>(
    subNavCoinGroup->checkedButton()->property("buttonId").value<int>()
  );
  state.quoteIndex = static_cast<QuoteType>(
    subNavQuoteCombo->currentIndex()
  );
  state.marginId = subNavMarginGroup->checkedButton() ? 
    static_cast<MarginType>(
      subNavMarginGroup->checkedButton()->property("buttonId").value<int>()
    ) : MarginType::USDT;
  state.futuresId = subNavFuturesGroup->checkedButton() ? 
    static_cast<FuturesType>(
      subNavFuturesGroup->checkedButton()->property("buttonId").value<int>()
    ) : FuturesType::ALL;
    
  qDebug() << "子导航状态变化:";
  qDebug() << "  主导航ID:" << static_cast<int>(state.mainNavId);
  qDebug() << "  币种ID:" << static_cast<int>(state.coinId);
  qDebug() << "  计价货币索引:" << static_cast<int>(state.quoteIndex);
  qDebug() << "  保证金ID:" << static_cast<int>(state.marginId);
  qDebug() << "  合约类型ID:" << static_cast<int>(state.futuresId);
  emit SubNavChanged(state);
}


// === WebSocket 相关 ===
/**
 * @brief 处理 WebSocket 连接成功
 * 
 * WebSocket连接成功后的处理：
 * 1. 更新连接状态为已连接
 * 2. 启动心跳定时器
 * 3. 发送connected信号通知外部
 */
void MainWindow::OnWebSocketConnected() {
  qDebug() << "WebSocket 连接成功";
}

/**
 * @brief 处理 WebSocket 连接断开
 * 
 * WebSocket连接断开后的处理：
 * 1. 更新连接状态为已断开
 * 2. 停止心跳定时器
 * 3. 发送disconnected信号通知外部
 */
void MainWindow::OnWebSocketDisconnected() {
  qDebug() << "WebSocket 连接断开";
}

/**
 * @brief 处理 WebSocket 错误
 * 
 * WebSocket错误处理：
 * 1. 打印错误信息
 * 2. 发送error信号通知外部
 */
void MainWindow::OnWebSocketError(const QString& errorMessage) {
  qDebug() << "WebSocket 错误:" << errorMessage;
}

/**
 * @brief 处理 WebSocket 消息接收
 * 
 * WebSocket消息接收后的处理：
 * 1. 打印接收到的消息
 * 2. 发送messageReceived信号通知外部
 */
void MainWindow::OnWebSocketMessageReceived(const QString& message) {
  qDebug() << "WebSocket 收到消息:" << message;
}
