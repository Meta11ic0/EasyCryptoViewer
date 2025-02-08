#include "mainnavigation.h"
#include "../../config/constant.h"

/**
 * @brief 主导航栏构造函数
 * @param parent 父窗口部件指针
 */
MainNavigation::MainNavigation(QWidget *parent)
    : QWidget(parent) {
  // 初始化UI界面
  initUI();
  // 连接信号和槽
  connectSignalsAndSlots();
}

/**
 * @brief 初始化UI界面
 * 设置导航栏的基本属性、创建布局、添加按钮和搜索框
 */
void MainNavigation::initUI() {
  // 设置主导航容器的大小策略和名称
  setObjectName("mainNavWidget");  // 设置对象名，用于QSS样式表
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);  // 设置大小策略为固定大小
  setFixedSize(1200, 50);  // 设置固定宽度1200px，高度50px
  
  // 创建水平布局管理器
  layout = new QHBoxLayout(this);
  layout->setContentsMargins(10, 5, 10, 5);  // 设置布局的外边距：左10px 上5px 右10px 下5px
  layout->setSpacing(5);  // 设置部件之间的间距为5px
  
  // 创建导航按钮
  createNavigationButtons();
  
  // 添加弹性空间，使搜索框靠右显示
  layout->addStretch();
  
  // 创建搜索框
  searchBox = new QLineEdit(this);
  searchBox->setObjectName("searchBox");  // 设置对象名，用于QSS样式表
  searchBox->setPlaceholderText("搜索币种/合约");  // 设置占位符文本
  searchBox->setFixedWidth(200);  // 设置搜索框固定宽度为200px
  searchBox->setProperty("class", "searchbox");  // 设置类名，用于QSS样式表
  layout->addWidget(searchBox);  // 将搜索框添加到布局中
}

/**
 * @brief 创建导航按钮
 * 创建并初始化所有导航按钮，设置按钮组
 */
void MainNavigation::createNavigationButtons() {
  // 创建按钮组，用于管理按钮的互斥选中状态
  buttonGroup = new QButtonGroup(this);
  buttonGroup->setExclusive(true);  // 设置按钮组为互斥模式，同一时间只能有一个按钮被选中
  
  // 创建7个导航按钮
  for (int i = 0; i < 7; i++) {
    auto text = mainNavButtonTexts[i];  // 从常量数组中获取按钮文本
    auto btn = new QPushButton(text, this);
    btn->setCheckable(true);  // 设置按钮可选中
    btn->setObjectName(QString("mainNavButton_%1").arg(i));  // 设置对象名
    btn->setProperty("buttonId", QVariant::fromValue(i));  // 设置按钮ID属性
    btn->setProperty("class", "mainnavbutton");  // 设置类名，用于QSS样式表
    layout->addWidget(btn);  // 将按钮添加到布局中
    buttonGroup->addButton(btn, i);  // 将按钮添加到按钮组，并设置ID
    navigationButtons.append(btn);  // 将按钮添加到导航按钮列表
    
    // 设置按钮的大小策略
    btn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    btn->setFixedHeight(30);  // 设置按钮固定高度为30px
  }
  
  // 设置默认选中第一个按钮
  if (!navigationButtons.isEmpty()) {
    navigationButtons.first()->setChecked(true);
  }
}

/**
 * @brief 处理按钮点击事件
 * @param id 被点击按钮的ID
 */
void MainNavigation::handleButtonClicked(int id) {
  // 获取被点击的按钮并输出调试信息
  if (id >= 0 && id < navigationButtons.size()) {
    QPushButton* clickedButton = navigationButtons[id];
    qDebug() << "点击了按钮:" << clickedButton->text() << "ID:" << id;
  }
  // 发送导航变化信号
  emit navigationChanged(id);
}

/**
 * @brief 处理搜索框文本变化事件
 * @param text 搜索框的新文本
 */
void MainNavigation::handleSearchTextChanged(const QString& text) {
  qDebug() << "搜索框文本变化:" << text;
  // 发送搜索文本变化信号
  emit searchTextChanged(text);
}

/**
 * @brief 连接信号和槽
 * 建立所有需要的信号槽连接
 */
void MainNavigation::connectSignalsAndSlots() {
  // 连接按钮组的idClicked信号到处理函数
  connect(buttonGroup, &QButtonGroup::idClicked, this, &MainNavigation::handleButtonClicked);
  // 连接搜索框的textChanged信号到处理函数
  connect(searchBox, &QLineEdit::textChanged, this, &MainNavigation::handleSearchTextChanged);
}

/**
 * @brief 析构函数
 */
MainNavigation::~MainNavigation() {}

