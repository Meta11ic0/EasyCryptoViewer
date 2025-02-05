#include "mainnavigation.h"
#include "../../config/constant.h"

MainNavigation::MainNavigation(QWidget *parent)
    : QWidget(parent) {
  // 设置主导航容器的大小策略
  setObjectName("mainNavWidget");
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  setFixedSize(1200, 50);
  layout = new QHBoxLayout(this);
  layout->setContentsMargins(10, 5, 10, 5);
  layout->setSpacing(5);
  // 创建主导航按钮组
  buttonGroup = new QButtonGroup(this);
  buttonGroup->setExclusive(true);
  for (int  i = 0; i< 7; i++) {
    auto text = mainNavButtonTexts[i];
    auto btn = new QPushButton(text, this);
    btn->setCheckable(true);
    btn->setObjectName(QString("mainNavButton_%1").arg(static_cast<int>(i)));
    btn->setProperty("buttonId", QVariant::fromValue(static_cast<int>(i)));
    btn->setProperty("class", "mainnavbutton");
    layout->addWidget(btn);
    buttonGroup->addButton(btn);
    // 设置按钮的大小策略
    btn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    btn->setFixedHeight(30);
  }
  // 设置默认选中第一个按钮
  if (auto firstBtn = buttonGroup->buttons().first()) {
    firstBtn->setChecked(true);
  }
  // 添加弹性空间
  layout->addStretch();
  // 创建搜索框
  searchBox = new QLineEdit(this);
  searchBox->setObjectName("searchBox");
  searchBox->setPlaceholderText("搜索币种/合约");
  searchBox->setFixedWidth(200);
  searchBox->setProperty("class", "searchbox");
  layout->addWidget(searchBox);
}

MainNavigation::~MainNavigation() {
  delete layout;
  delete buttonGroup;
  delete searchBox;
}

