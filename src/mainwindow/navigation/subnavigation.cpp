#include "subnavigation.h"
#include "../../config/constant.h"

/**
 * @brief 子导航栏构造函数
 * @param parent 父窗口部件指针
 * @details 
 * 1. 初始化整个子导航栏的UI界面
 * 2. 连接所有的信号和槽函数
 */
SubNavigation::SubNavigation(QWidget *parent)
    : QWidget(parent) {
  initUI();                    // 初始化UI界面元素
  connectSignalsAndSlots();    // 连接所有信号和槽函数
}

/**
 * @brief 初始化整体UI布局
 * @details 
 * 1. 设置子导航栏的基本属性(大小、样式等)
 * 2. 创建水平布局作为主布局
 * 3. 依次初始化四个主要区域:
 *    - 计价货币区域(USDT等)
 *    - 保证金区域(全仓、逐仓)
 *    - 币种选择区域(BTC、ETH等)
 *    - 合约类型区域(永续、季度等)
 */
void SubNavigation::initUI() {
  // 设置子导航容器的基本属性
  setObjectName("subNavWidget");  // 设置对象名，用于QSS样式表
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);  // 固定大小策略
  setFixedSize(1200, 50);  // 设置固定大小：宽1200px，高50px
  
  // 创建并设置主布局
  layout = new QHBoxLayout(this);
  layout->setContentsMargins(10, 5, 10, 5);  // 设置外边距：左10px 上5px 右10px 下5px
  layout->setSpacing(5);                     // 设置部件之间的间距为5px
  
  // 初始化各个功能区域
  initQuoteArea();    // 初始化计价货币区域(USDT等)
  initMarginArea();   // 初始化保证金区域
  initCoinArea();     // 初始化币种区域(BTC、ETH等)
  initFuturesArea();  // 初始化合约类型区域(永续、季度等)
}

/**
 * @brief 初始化计价货币区域
 * @details 
 * 1. 创建计价货币区域的容器和布局
 * 2. 添加"计价货币"标签
 * 3. 创建下拉框并添加货币选项(USDT等)
 * 4. 设置默认选中项
 */
void SubNavigation::initQuoteArea() {
  // 创建计价货币区域容器和布局
  quoteWidget = new QWidget(this);
  quoteWidget->setObjectName("quoteWidget");  // 设置对象名用于样式表
  quoteWidget->setFixedWidth(150);           // 固定宽度为150px
  quoteWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  
  // 创建水平布局
  quoteLayout = new QHBoxLayout(quoteWidget);
  quoteLayout->setContentsMargins(0, 0, 0, 0);  // 无边距
  quoteLayout->setSpacing(5);                   // 组件间距5px
  
  // 创建"计价货币"标签
  quoteLabel = new QLabel(NavQuoteLabel, quoteWidget);
  quoteLabel->setObjectName("quoteLabel");
  quoteLayout->addWidget(quoteLabel);
  
  // 创建计价货币下拉框
  quoteCombo = new QComboBox(quoteWidget);
  quoteCombo->setObjectName(subNavButtonName[0]);
  quoteCombo->setProperty("class", subNavButtonName[0]);  // 设置class属性用于样式表
  quoteCombo->setFixedWidth(100);                        // 下拉框宽度100px
  
  // 添加计价货币选项(如USDT、USD等)
  for (int i = 0; i < 3; i++) {
    quoteCombo->addItem(quoteButtonTexts[i]);
  }
  quoteCombo->setCurrentIndex(0);  // 默认选中第一项
  quoteLayout->addWidget(quoteCombo);
  
  // 添加到主布局并默认隐藏
  layout->addWidget(quoteWidget);
  quoteWidget->hide();
}

/**
 * @brief 初始化保证金区域
 * @details 
 * 1. 创建保证金区域容器和布局
 * 2. 创建按钮组
 * 3. 设置按钮组的样式和属性
 */
void SubNavigation::initMarginArea() {
  // 创建保证金区域容器和布局
  marginWidget = new QWidget(this);
  marginWidget->setObjectName("marginWidget");
  marginWidget->setFixedWidth(200);          // 固定宽度200px
  marginWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  
  // 创建水平布局
  marginLayout = new QHBoxLayout(marginWidget);
  marginLayout->setContentsMargins(0, 0, 0, 0);
  marginLayout->setSpacing(5);
  
  // 创建保证金类型按钮组
  createNavigationButtons(marginGroup, marginButtons, marginButtonTexts, 
                         marginLayout, subNavButtonName[1]);
  
  // 添加到主布局并默认隐藏
  layout->addWidget(marginWidget);
  marginWidget->hide();
}

/**
 * @brief 初始化币种区域
 * @details 
 * 1. 创建可滚动的币种选择区域
 * 2. 添加左右箭头按钮用于滚动
 * 3. 创建币种按钮组(BTC、ETH等)
 * 4. 设置滚动区域的属性和行为
 */
void SubNavigation::initCoinArea() {
  // 创建币种区域容器和布局
  coinWidget = new QWidget(this);
  coinWidget->setObjectName("coinWidget");
  coinWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  
  // 创建水平布局
  coinLayout = new QHBoxLayout(coinWidget);
  coinLayout->setContentsMargins(0, 0, 0, 0);
  coinLayout->setSpacing(0);
  
  // 创建左箭头按钮
  leftArrowButton = new QPushButton("<", coinWidget);
  leftArrowButton->setObjectName("leftArrowButton");
  leftArrowButton->setFixedSize(20, 30);    // 按钮大小20x30px
  leftArrowButton->hide();                  // 默认隐藏
  coinLayout->addWidget(leftArrowButton);
  
  // 创建滚动区域
  coinScrollArea = new QScrollArea(coinWidget);
  coinScrollArea->setObjectName("coinScrollArea");
  coinScrollArea->setWidgetResizable(true);  // 允许内容区域调整大小
  coinScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  // 隐藏水平滚动条
  coinScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);    // 隐藏垂直滚动条
  coinScrollBar = coinScrollArea->horizontalScrollBar();  // 获取水平滚动条引用
  
  // 创建滚动区域的内容容器
  coinScrollContent = new QWidget(coinScrollArea);
  coinScrollContent->setObjectName("coinScrollContent");
  coinScrollLayout = new QHBoxLayout(coinScrollContent);
  coinScrollLayout->setContentsMargins(0, 0, 0, 0);
  coinScrollLayout->setSpacing(0);
  coinScrollLayout->setAlignment(Qt::AlignVCenter);  // 垂直居中对齐
  
  // 设置滚动区域的大小策略
  coinScrollArea->setFixedHeight(30);  // 固定高度30px
  coinScrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  coinScrollArea->setAlignment(Qt::AlignVCenter);
  coinScrollContent->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
  
  // 创建币种按钮组
  createNavigationButtons(coinGroup, coinButtons, coinButtonTexts, 
                         coinScrollLayout, subNavButtonName[2]);
  
  // 设置滚动区域的内容
  coinScrollArea->setWidget(coinScrollContent);
  coinLayout->addWidget(coinScrollArea);
  
  // 创建右箭头按钮
  rightArrowButton = new QPushButton(">", coinWidget);
  rightArrowButton->setObjectName("rightArrowButton");
  rightArrowButton->setFixedSize(20, 30);   // 按钮大小20x30px
  rightArrowButton->hide();                 // 默认隐藏
  coinLayout->addWidget(rightArrowButton);
  
  // 添加到主布局
  layout->addWidget(coinWidget);
}

/**
 * @brief 初始化合约类型区域
 * @details 
 * 1. 创建合约类型区域容器和布局
 * 2. 创建合约类型按钮组(永续、季度等)
 * 3. 设置按钮组的样式和属性
 */
void SubNavigation::initFuturesArea() {
  // 创建合约类型区域容器和布局
  futuresWidget = new QWidget(this);
  futuresWidget->setObjectName("futuresWidget");
  futuresWidget->setFixedWidth(200);        // 固定宽度200px
  futuresWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  
  // 创建水平布局
  futuresLayout = new QHBoxLayout(futuresWidget);
  futuresLayout->setContentsMargins(0, 0, 0, 0);
  futuresLayout->setSpacing(5);
  
  // 创建合约类型按钮组
  createNavigationButtons(futuresGroup, futuresButtons, futuresButtonTexts, 
                         futuresLayout, subNavButtonName[3]);
  
  // 添加到主布局并默认隐藏
  layout->addWidget(futuresWidget);
  futuresWidget->hide();
}

/**
 * @brief 创建导航按钮组的通用函数
 * @param buttonGroup 按钮组指针
 * @param buttons 按钮列表
 * @param buttonTexts 按钮文本列表
 * @param layout 按钮容器布局
 * @param buttonName 按钮基础名称
 * @details 
 * 1. 创建按钮组并设置互斥
 * 2. 根据文本列表创建按钮
 * 3. 设置按钮的属性和样式
 * 4. 将按钮添加到布局和按钮组中
 */
void SubNavigation::createNavigationButtons(QButtonGroup*& buttonGroup, 
                                          QList<QPushButton*>& buttons, 
                                          const QStringList& buttonTexts, 
                                          QHBoxLayout* layout,
                                          const QString& buttonName) {
  // 创建按钮组
  buttonGroup = new QButtonGroup(this);
  buttonGroup->setExclusive(true);  // 设置互斥模式(同一时间只能选中一个按钮)
  
  // 创建按钮
  for (int i = 0; i < buttonTexts.size(); i++) {
    auto text = buttonTexts[i];
    auto btn = new QPushButton(text, this);
    btn->setCheckable(true);        // 设置按钮可选中
    btn->setObjectName(QString("%1_%2").arg(buttonName).arg(i));  // 设置对象名
    btn->setProperty("buttonId", QVariant::fromValue(i));  // 设置按钮ID属性
    btn->setProperty("class", buttonName);  // 设置class属性用于样式表
    btn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    btn->setFixedHeight(30);        // 设置按钮高度30px
    
    // 添加到布局和按钮组
    layout->addWidget(btn);
    buttonGroup->addButton(btn, i);
    buttons.append(btn);
  }
  
  // 默认选中第一个按钮
  if (!buttons.isEmpty()) {
    buttons.first()->setChecked(true);
  }
}

/**
 * @brief 连接所有信号和槽
 * @details 
 * 1. 连接计价货币下拉框的信号
 * 2. 连接各个按钮组的点击信号
 * 3. 连接左右箭头按钮的点击信号
 * 4. 连接滚动条值变化的信号
 */
void SubNavigation::connectSignalsAndSlots() {
  // 连接计价货币下拉框信号
  connect(quoteCombo, &QComboBox::currentTextChanged, 
          this, &SubNavigation::handleQuoteChanged);
  
  // 连接保证金按钮组信号
  connect(marginGroup, &QButtonGroup::idClicked, 
          this, &SubNavigation::handleMarginButtonClicked);
  
  // 连接币种按钮组信号
  connect(coinGroup, &QButtonGroup::idClicked, 
          this, &SubNavigation::handleCoinButtonClicked);
  
  // 连接合约类型按钮组信号
  connect(futuresGroup, &QButtonGroup::idClicked, 
          this, &SubNavigation::handleFuturesButtonClicked);
  
  // 连接左右箭头按钮信号
  connect(leftArrowButton, &QPushButton::clicked, 
          this, &SubNavigation::handleLeftArrowClicked);
  connect(rightArrowButton, &QPushButton::clicked, 
          this, &SubNavigation::handleRightArrowClicked);
  
  // 监听滚动条值变化，用于更新箭头按钮状态
  connect(coinScrollBar, &QScrollBar::valueChanged, 
          this, &SubNavigation::updateArrowButtonsState);
}

/**
 * @brief 处理计价货币变化的槽函数
 * @param text 新选择的计价货币文本
 */
void SubNavigation::handleQuoteChanged(const QString& text) {
  qDebug() << "计价货币变化:" << text;
  emit quoteChanged(text);  // 发射信号通知外部
}

/**
 * @brief 处理保证金按钮点击的槽函数
 * @param id 被点击按钮的ID
 */
void SubNavigation::handleMarginButtonClicked(int id) {
  if (id >= 0 && id < marginButtons.size()) {
    QPushButton* clickedButton = marginButtons[id];
    qDebug() << "点击了保证金按钮:" << clickedButton->text() << "ID:" << id;
    emit marginTypeChanged(id);  // 发射信号通知外部
  }
}

/**
 * @brief 处理币种按钮点击的槽函数
 * @param id 被点击按钮的ID
 */
void SubNavigation::handleCoinButtonClicked(int id) {
  if (id >= 0 && id < coinButtons.size()) {
    QPushButton* clickedButton = coinButtons[id];
    qDebug() << "点击了币种按钮:" << clickedButton->text() << "ID:" << id;
    emit coinTypeChanged(id);  // 发射信号通知外部
  }
}

/**
 * @brief 处理合约类型按钮点击的槽函数
 * @param id 被点击按钮的ID
 */
void SubNavigation::handleFuturesButtonClicked(int id) {
  if (id >= 0 && id < futuresButtons.size()) {
    QPushButton* clickedButton = futuresButtons[id];
    qDebug() << "点击了合约类型按钮:" << clickedButton->text() << "ID:" << id;
    emit futuresTypeChanged(id);  // 发射信号通知外部
  }
}

/**
 * @brief 处理左箭头按钮点击的槽函数
 * @details 向左滚动一定步长
 */
void SubNavigation::handleLeftArrowClicked() {
  if (!coinScrollBar || !coinScrollContent) return;
  
  int scrollStep = coinScrollArea->viewport()->width() / 6;
  int oldValue = coinScrollBar->value();
  coinScrollBar->setValue(oldValue - scrollStep);  // 向左滚动
  qDebug() << "左箭头点击 - 滚动值从" << oldValue << "变为" << coinScrollBar->value();
  updateArrowButtonsState();  // 更新箭头按钮状态
}

/**
 * @brief 处理右箭头按钮点击的槽函数
 * @details 向右滚动一定步长
 */
void SubNavigation::handleRightArrowClicked() {
  if (!coinScrollBar || !coinScrollContent) return;
  int scrollStep = coinScrollArea->viewport()->width() / 6;
  int oldValue = coinScrollBar->value();
  coinScrollBar->setValue(oldValue + scrollStep);  // 向右滚动
  qDebug() << "右箭头点击 - 滚动值从" << oldValue << "变为" << coinScrollBar->value();
  updateArrowButtonsState();  // 更新箭头按钮状态
}


/**
 * @brief 更新箭头按钮的显示状态
 * @details 
 * 1. 根据内容宽度和视口宽度决定是否需要显示箭头
 * 2. 根据滚动条位置决定显示哪个箭头
 */
void SubNavigation::updateArrowButtonsState() {
  if (!coinScrollArea || !coinScrollContent || !leftArrowButton || !rightArrowButton) {
    return;
  }

  // 计算内容总宽度和视口宽度
  int contentWidth = coinScrollContent->sizeHint().width();
  int viewportWidth = coinScrollArea->viewport()->width();
  
  // 根据内容宽度决定是否显示箭头
  bool needArrows = contentWidth > viewportWidth;
  
  // 更新箭头按钮状态
  leftArrowButton->setVisible(needArrows && coinScrollBar->value() > 0);
  rightArrowButton->setVisible(needArrows && coinScrollBar->value() < coinScrollBar->maximum());
}

/**
 * @brief 显示/隐藏计价货币区域
 * @param show true显示，false隐藏
 */
void SubNavigation::showQuoteArea(bool show) {
  if (quoteWidget) {
    quoteWidget->setVisible(show);
  }
}

/**
 * @brief 显示/隐藏保证金区域
 * @param show true显示，false隐藏
 */
void SubNavigation::showMarginArea(bool show) {
  if (marginWidget) {
    marginWidget->setVisible(show);
  }
}

/**
 * @brief 显示/隐藏合约类型区域
 * @param show true显示，false隐藏
 */
void SubNavigation::showFuturesArea(bool show) {
  if (futuresWidget) {
    futuresWidget->setVisible(show);
  }
}

/**
 * @brief 设置当前计价货币
 * @param quote 要设置的计价货币
 */
void SubNavigation::setCurrentQuote(const QString& quote) {
  if (quoteCombo) {
    int index = quoteCombo->findText(quote);
    if (index >= 0) {
      quoteCombo->setCurrentIndex(index);
      emit quoteChanged(quote);
    }
  }
}

/**
 * @brief 设置当前保证金类型
 * @param index 要设置的保证金类型索引
 */
void SubNavigation::setCurrentMargin(int index) {
  if (marginGroup && index >= 0 && index < marginButtons.size()) {
    marginButtons[index]->setChecked(true);
    emit marginTypeChanged(index);
  }
}

/**
 * @brief 设置当前币种
 * @param index 要设置的币种索引
 */
void SubNavigation::setCurrentCoin(int index) {
  if (coinGroup && index >= 0 && index < coinButtons.size()) {
    coinButtons[index]->setChecked(true);
    emit coinTypeChanged(index);
  }
}

/**
 * @brief 设置当前合约类型
 * @param index 要设置的合约类型索引
 */
void SubNavigation::setCurrentFutures(int index) {
  if (futuresGroup && index >= 0 && index < futuresButtons.size()) {
    futuresButtons[index]->setChecked(true);
    emit futuresTypeChanged(index);
  }
}

/**
 * @brief 析构函数
 * @details Qt会自动删除子对象，所以这里不需要手动释放内存
 */
SubNavigation::~SubNavigation() {}

