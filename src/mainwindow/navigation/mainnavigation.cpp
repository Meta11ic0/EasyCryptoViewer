#include "mainnavigation.h"
#include "../../config/constant.h"

/**
 * @brief 主导航栏构造函数
 * @param parent 父窗口部件指针
 * @details 
 * 1. 初始化整个主导航栏的UI界面
 * 2. 连接所有的信号和槽函数
 */
MainNavigation::MainNavigation(QWidget *parent)
    : QWidget(parent) {
  initUI();                    // 初始化UI界面元素
  connectSignalsAndSlots();    // 连接所有信号和槽函数
}

/**
 * @brief 初始化UI界面
 * @details 
 * 1. 设置主导航栏的基本属性(大小、样式等)
 * 2. 创建水平布局作为主布局
 * 3. 创建导航按钮
 * 4. 创建搜索框
 */
void MainNavigation::initUI() {
  // 设置主导航容器的基本属性
  setObjectName("mainNavWidget");  // 设置对象名，用于QSS样式表
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);  // 固定大小策略
  setFixedSize(1200, 50);  // 设置固定大小：宽1200px，高50px
  
  // 创建并设置主布局
  layout = new QHBoxLayout(this);
  layout->setContentsMargins(10, 5, 10, 5);  // 设置外边距：左10px 上5px 右10px 下5px
  layout->setSpacing(5);                     // 设置部件之间的间距为5px
  
  // 创建导航按钮
  createNavigationButtons();
  
  // 添加弹性空间，使搜索框靠右显示
  layout->addStretch();
  
  // 创建搜索框
  searchBox = new QLineEdit(this);
  searchBox->setObjectName("searchBox");     // 设置对象名用于样式表
  searchBox->setPlaceholderText("搜索币种/合约");     // 设置占位符文本
  searchBox->setFixedWidth(200);            // 设置固定宽度200px
  searchBox->setProperty("class", "searchbox");  // 设置类名，用于QSS样式表
  layout->addWidget(searchBox);             // 添加到布局
}

/**
 * @brief 创建导航按钮
 * @details 
 * 1. 创建按钮组并设置互斥
 * 2. 创建所有导航按钮并设置属性
 * 3. 添加到布局和按钮组
 * 4. 默认选中第一个按钮
 */
void MainNavigation::createNavigationButtons() {
  // 创建按钮组
  buttonGroup = new QButtonGroup(this);
  buttonGroup->setExclusive(true);  // 设置按钮组互斥
  
  // 创建导航按钮
  for (int i = 0; i < mainNavButtonTexts.size(); i++) {
    auto text = mainNavButtonTexts[i];
    auto btn = new QPushButton(text, this);
    btn->setCheckable(true);        // 设置按钮可选中
    btn->setObjectName(QString("mainNavButton_%1").arg(i));  // 设置对象名
    btn->setProperty("buttonId", QVariant::fromValue(i));    // 设置按钮ID属性
    btn->setProperty("class", "mainnavbutton");              // 设置class属性用于样式表
    btn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    btn->setFixedHeight(30);        // 设置按钮高度30px
    
    // 添加到布局和按钮组
    layout->addWidget(btn);
    buttonGroup->addButton(btn, i);
    navigationButtons.append(btn);
  }
  
  // 默认选中第一个按钮
  if (!navigationButtons.isEmpty()) {
    navigationButtons.first()->setChecked(true);
  }
}

/**
 * @brief 连接所有信号和槽
 * @details 
 * 1. 连接按钮组的点击信号
 * 2. 连接搜索框的文本变化信号
 */
void MainNavigation::connectSignalsAndSlots() {
  // 连接按钮组的点击信号
  connect(buttonGroup, &QButtonGroup::idClicked, 
          this, &MainNavigation::handleButtonClicked);
  
  // 连接搜索框的文本变化信号
  connect(searchBox, &QLineEdit::textChanged, 
          this, &MainNavigation::handleSearchTextChanged);
}

/**
 * @brief 处理按钮点击事件
 * @param id 被点击按钮的ID
 */
void MainNavigation::handleButtonClicked(int id) {
  // 记录日志
  if (id >= 0 && id < navigationButtons.size()) {
    QPushButton* clickedButton = navigationButtons[id];
    qDebug() << "点击了导航按钮:" << clickedButton->text() << "ID:" << id;
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
 * @brief 设置当前选中的按钮
 * @param index 要选中的按钮索引
 */
void MainNavigation::setCurrentButton(int index) {
  if (index >= 0 && index < navigationButtons.size()) {
    navigationButtons[index]->setChecked(true);
    emit navigationChanged(index);
  }
}

/**
 * @brief 设置搜索框的文本
 * @param text 要设置的文本
 */
void MainNavigation::setSearchText(const QString& text) {
  if (searchBox) {
    searchBox->setText(text);
    emit searchTextChanged(text);
  }
}


/**
 * @brief 析构函数
 */
MainNavigation::~MainNavigation() {
  // Qt会自动删除子对象，所以这里不需要手动释放内存
}

