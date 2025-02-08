#include "subnavigation.h"
#include "../../config/constant.h"

/**
 * @brief 子导航栏构造函数
 * @param parent 父窗口部件指针
 */
SubNavigation::SubNavigation(QWidget *parent)
    : QWidget(parent) {
  initUI();                    // 初始化UI
  connectSignalsAndSlots();    // 连接信号和槽
}

/**
 * @brief 初始化整体UI布局
 * @details 设置基本属性并初始化各个子区域
 */
void SubNavigation::initUI() {
  // 设置子导航容器的基本属性
  setObjectName("subNavWidget");
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  setFixedSize(1200, 50);  // 设置固定大小：宽1200px，高50px
  
  // 创建并设置主布局
  layout = new QHBoxLayout(this);
  layout->setContentsMargins(10, 5, 10, 5);  // 设置外边距：左10px 上5px 右10px 下5px
  layout->setSpacing(5);                     // 设置部件间距为5px
  
  // 初始化各个功能区域
  initQuoteArea();    // 初始化计价货币区域
  initMarginArea();   // 初始化保证金区域
  initCoinArea();     // 初始化币种区域
  initFuturesArea();  // 初始化合约类型区域
}

/**
 * @brief 初始化计价货币区域
 * @details 创建并设置计价货币标签和下拉框
 */
void SubNavigation::initQuoteArea() {
  // 创建计价货币区域容器和布局
  quoteWidget = new QWidget(this);
  quoteWidget->setObjectName("quoteWidget");
  quoteWidget->setFixedWidth(150);          // 设置固定宽度150px
  quoteWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  
  // 创建并设置布局
  quoteLayout = new QHBoxLayout(quoteWidget);
  quoteLayout->setContentsMargins(0, 0, 0, 0);
  quoteLayout->setSpacing(5);
  
  // 创建"计价货币"标签
  quoteLabel = new QLabel(NavQuoteLabel, quoteWidget);
  quoteLabel->setObjectName("quoteLabel");
  quoteLayout->addWidget(quoteLabel);
  
  // 创建计价货币下拉框
  quoteCombo = new QComboBox(quoteWidget);
  quoteCombo->setObjectName(subNavButtonName[0]);
  quoteCombo->setProperty("class", subNavButtonName[0]);
  quoteCombo->setFixedWidth(100);           // 设置下拉框宽度100px
  
  // 添加计价货币选项
  for (int i = 0; i < 3; i++) {
    quoteCombo->addItem(quoteButtonTexts[i]);
  }
  quoteCombo->setCurrentIndex(0);           // 设置默认选中第一项
  quoteLayout->addWidget(quoteCombo);
  
  // 添加到主布局并默认隐藏
  layout->addWidget(quoteWidget);
  quoteWidget->hide();
}

/**
 * @brief 初始化保证金区域
 * @details 创建保证金类型选择按钮组
 */
void SubNavigation::initMarginArea() {
  // 创建保证金区域容器和布局
  marginWidget = new QWidget(this);
  marginWidget->setObjectName("marginWidget");
  marginWidget->setFixedWidth(200);         // 设置固定宽度200px
  marginWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  
  // 创建并设置布局
  marginLayout = new QHBoxLayout(marginWidget);
  marginLayout->setContentsMargins(0, 0, 0, 0);
  marginLayout->setSpacing(5);
  
  // 创建保证金类型按钮组
  createNavigationButtons(marginGroup, marginButtons, marginButtonTexts, marginLayout, subNavButtonName[1]);
  
  // 添加到主布局并默认隐藏
  layout->addWidget(marginWidget);
  marginWidget->hide();
}

/**
 * @brief 初始化币种区域
 * @details 创建可滚动的币种选择区域，包含左右箭头按钮
 */
void SubNavigation::initCoinArea() {
  // 创建币种区域容器和布局
  coinWidget = new QWidget(this);
  coinWidget->setObjectName("coinWidget");
  coinWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  
  // 创建并设置布局
  coinLayout = new QHBoxLayout(coinWidget);
  coinLayout->setContentsMargins(0, 0, 0, 0);
  coinLayout->setSpacing(0);
  
  // 创建左箭头按钮
  leftArrowButton = new QPushButton("<", coinWidget);
  leftArrowButton->setObjectName("leftArrowButton");
  leftArrowButton->setFixedSize(20, 30);    // 设置按钮大小20x30px
  leftArrowButton->hide();                  // 默认隐藏
  coinLayout->addWidget(leftArrowButton);
  
  // 创建滚动区域
  coinScrollArea = new QScrollArea(coinWidget);
  coinScrollArea->setObjectName("coinScrollArea");
  coinScrollArea->setWidgetResizable(true);
  coinScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  coinScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  coinScrollBar = coinScrollArea->horizontalScrollBar();
  
  // 创建滚动区域的内容容器
  coinScrollContent = new QWidget(coinScrollArea);
  coinScrollContent->setObjectName("coinScrollContent");
  coinScrollLayout = new QHBoxLayout(coinScrollContent);
  coinScrollLayout->setContentsMargins(0, 0, 0, 0);
  coinScrollLayout->setSpacing(0);
  coinScrollLayout->setAlignment(Qt::AlignVCenter);
  
  // 设置滚动区域的大小策略
  coinScrollArea->setFixedHeight(30);
  coinScrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  coinScrollArea->setAlignment(Qt::AlignVCenter);
  coinScrollContent->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
  
  // 创建币种按钮组
  createNavigationButtons(coinGroup, coinButtons, coinButtonTexts, coinScrollLayout, subNavButtonName[2]);
  
  // 设置滚动区域的内容
  coinScrollArea->setWidget(coinScrollContent);
  coinLayout->addWidget(coinScrollArea);
  
  // 创建右箭头按钮
  rightArrowButton = new QPushButton(">", coinWidget);
  rightArrowButton->setObjectName("rightArrowButton");
  rightArrowButton->setFixedSize(20, 30);   // 设置按钮大小20x30px
  rightArrowButton->hide();                 // 默认隐藏
  coinLayout->addWidget(rightArrowButton);
  
  // 添加到主布局
  layout->addWidget(coinWidget);
}

/**
 * @brief 初始化合约类型区域
 * @details 创建合约类型选择按钮组
 */
void SubNavigation::initFuturesArea() {
  // 创建合约类型区域容器和布局
  futuresWidget = new QWidget(this);
  futuresWidget->setObjectName("futuresWidget");
  futuresWidget->setFixedWidth(200);        // 设置固定宽度200px
  futuresWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  
  // 创建并设置布局
  futuresLayout = new QHBoxLayout(futuresWidget);
  futuresLayout->setContentsMargins(0, 0, 0, 0);
  futuresLayout->setSpacing(5);
  
  // 创建合约类型按钮组
  createNavigationButtons(futuresGroup, futuresButtons, futuresButtonTexts, futuresLayout, subNavButtonName[3]);
  
  // 添加到主布局并默认隐藏
  layout->addWidget(futuresWidget);
  futuresWidget->hide();
}

/**
 * @brief 创建导航按钮组
 * @param buttonGroup 按钮组指针
 * @param buttons 按钮列表
 * @param buttonTexts 按钮文本列表
 * @param layout 按钮容器布局
 * @param buttonName 按钮基础名称
 */
void SubNavigation::createNavigationButtons(QButtonGroup*& buttonGroup, 
                                          QList<QPushButton*>& buttons, 
                                          const QStringList& buttonTexts, 
                                          QHBoxLayout* layout,
                                          const QString& buttonName) {
  // 创建按钮组
  buttonGroup = new QButtonGroup(this);
  buttonGroup->setExclusive(true);  // 设置互斥模式
  
  // 创建按钮
  for (int i = 0; i < buttonTexts.size(); i++) {
    auto text = buttonTexts[i];
    auto btn = new QPushButton(text, this);
    btn->setCheckable(true);        // 设置按钮可选中
    btn->setObjectName(QString("%1_%2").arg(buttonName).arg(i));
    btn->setProperty("buttonId", QVariant::fromValue(i));
    btn->setProperty("class", buttonName);
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
  connect(coinScrollBar, &QScrollBar::valueChanged, this, &SubNavigation::updateArrowButtonsState);
}

void SubNavigation::handleQuoteChanged(const QString& text) {
  qDebug() << "计价货币变化:" << text;
  emit quoteChanged(text);
}

void SubNavigation::handleMarginButtonClicked(int id) {
  if (id >= 0 && id < marginButtons.size()) {
    QPushButton* clickedButton = marginButtons[id];
    qDebug() << "点击了保证金按钮:" << clickedButton->text() << "ID:" << id;
    emit marginTypeChanged(id);
  }
}

void SubNavigation::handleCoinButtonClicked(int id) {
  if (id >= 0 && id < coinButtons.size()) {
    QPushButton* clickedButton = coinButtons[id];
    qDebug() << "点击了币种按钮:" << clickedButton->text() << "ID:" << id;
    emit coinTypeChanged(id);
  }
}

void SubNavigation::handleFuturesButtonClicked(int id) {
  if (id >= 0 && id < futuresButtons.size()) {
    QPushButton* clickedButton = futuresButtons[id];
    qDebug() << "点击了合约类型按钮:" << clickedButton->text() << "ID:" << id;
    emit futuresTypeChanged(id);
  }
}

void SubNavigation::handleLeftArrowClicked() {
  if (!coinScrollBar || !coinScrollContent) return;
  
  int scrollStep = calculateScrollStep();
  int oldValue = coinScrollBar->value();
  coinScrollBar->setValue(oldValue - scrollStep);
  qDebug() << "左箭头点击 - 滚动值从" << oldValue << "变为" << coinScrollBar->value();
  updateArrowButtonsState();
}

void SubNavigation::handleRightArrowClicked() {
  if (!coinScrollBar || !coinScrollContent) return;
  int scrollStep = calculateScrollStep();
  int oldValue = coinScrollBar->value();
  coinScrollBar->setValue(oldValue + scrollStep);
  qDebug() << "右箭头点击 - 滚动值从" << oldValue << "变为" << coinScrollBar->value();
  updateArrowButtonsState();
}

int SubNavigation::calculateScrollStep() const {
  // 如果有币种按钮，使用按钮的平均宽度作为步长
  if (!coinButtons.isEmpty()) {
    int totalWidth = 0;
    int count = 0;
    for (const auto& btn : coinButtons) {
      if (btn) {
        totalWidth += btn->width();
        count++;
      }
    }
    if (count > 0) {
      return totalWidth / count;  // 返回平均按钮宽度
    }
  }
  // 默认返回视口宽度的1/6
  return coinScrollArea ? coinScrollArea->viewport()->width() / 6 : 100;
}

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

void SubNavigation::showQuoteArea(bool show) {
  if (quoteWidget) {
    quoteWidget->setVisible(show);
  }
}

void SubNavigation::showMarginArea(bool show) {
  if (marginWidget) {
    marginWidget->setVisible(show);
  }
}

void SubNavigation::showFuturesArea(bool show) {
  if (futuresWidget) {
    futuresWidget->setVisible(show);
  }
}

SubNavigation::~SubNavigation() {}

