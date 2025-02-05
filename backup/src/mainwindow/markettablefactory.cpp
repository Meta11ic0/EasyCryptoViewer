#include "markettablefactory.h"
#include <QHeaderView>

/**
 * @brief 根据导航状态创建对应的表格
 * @param state 当前导航状态
 * @return 创建好的表格控件
 * 
 * 根据主导航ID选择不同类型的表格：
 * - 币种(0): 创建币种表格
 * - 现货(1): 创建现货交易表格
 * - 合约(2): 创建合约交易表格
 */
QTableWidget* MarketTableFactory::createTable(const NavState& state) {
  // 根据主导航ID选择表格类型
  int mainNavId = static_cast<int>(state.mainNavId);
  switch (mainNavId) {
    case 0:  // 币种
      return createTableByType(MarketTableType::COIN);
    case 1:  // 现货交易
      return createTableByType(MarketTableType::SPOT);
    case 2:  // 合约交易
      return createTableByType(MarketTableType::FUTURES);
    default:
      return createTableByType(MarketTableType::COIN);  // 默认返回币种表格
  }
}

/**
 * @brief 根据表格类型创建具体的表格
 * @param type 表格类型(币种/现货/合约)
 * @return 创建好的表格控件
 * 
 * 创建表格的步骤：
 * 1. 创建表格实例
 * 2. 设置通用属性
 * 3. 根据类型设置特定属性
 */
QTableWidget* MarketTableFactory::createTableByType(MarketTableType type) {
  auto table = new QTableWidget();
  // 设置通用属性
  setupCommonProperties(table);
  // 根据类型设置特定属性
  switch (type) {
    case MarketTableType::COIN:
      setupCoinTable(table);
      break;
    case MarketTableType::SPOT:
      setupSpotTable(table);
      break;
    case MarketTableType::FUTURES:
      setupFuturesTable(table);
      break;
  } 
  return table;
}

/**
 * @brief 设置表格的通用属性
 * @param table 要设置的表格控件
 * 
 * 设置表格的基本属性：
 * 1. 外观设置：无边框、网格线、行高等
 * 2. 交互设置：不可编辑、行选择等
 * 3. 表头设置：对齐方式、可点击等
 * 4. 性能优化：交替行颜色等
 */
void MarketTableFactory::setupCommonProperties(QTableWidget* table) {
  // 设置表格基本属性
  table->setFrameShape(QFrame::NoFrame);
  table->setEditTriggers(QAbstractItemView::NoEditTriggers);
  table->setSelectionBehavior(QAbstractItemView::SelectRows);
  table->setSelectionMode(QAbstractItemView::SingleSelection);
  table->verticalHeader()->setVisible(false);
  table->setShowGrid(false);
  table->setAlternatingRowColors(true);
  // 设置表头样式
  auto headerView = table->horizontalHeader();
  headerView->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  headerView->setStretchLastSection(true);
  headerView->setHighlightSections(false);
  headerView->setSectionsClickable(true);
  // 设置行高
  table->verticalHeader()->setDefaultSectionSize(40);
} 

/**
 * @brief 设置币种表格的列和属性
 * @param table 要设置的表格控件
 * 
 * 币种表格的列包括：
 * 1. 名称
 * 2. 最新价
 * 3. 涨跌幅
 */
void MarketTableFactory::setupCoinTable(QTableWidget* table) {
  // 设置列数和表头
  const int columnCount = 3;
  table->setColumnCount(columnCount);
  table->setHorizontalHeaderLabels(marketTableHeaders);
  // 设置列宽
  table->setColumnWidth(0, 200);  // 名称
  table->setColumnWidth(1, 150);  // 最新价
  table->setColumnWidth(2, 100);  // 涨跌幅
} 

/**
 * @brief 设置现货交易表格的列和属性
 * @param table 要设置的表格控件
 * 
 * 现货交易表格的列包括：
 * 1. 名称
 * 2. 最新价
 * 3. 涨跌幅
 * 4. 24h最高
 * 5. 24h最低
 * 6. 24h成交额
 */
void MarketTableFactory::setupSpotTable(QTableWidget* table) {
  // 设置列数和表头
  const int columnCount = 6;
  table->setColumnCount(columnCount);
  table->setHorizontalHeaderLabels(spotTableHeaders);
  
  // 设置列宽
  table->setColumnWidth(0, 200);  // 名称
  table->setColumnWidth(1, 150);  // 最新价
  table->setColumnWidth(2, 100);  // 涨跌幅
  table->setColumnWidth(3, 120);  // 24h最高
  table->setColumnWidth(4, 120);  // 24h最低
  table->setColumnWidth(5, 150);  // 24h成交额
}

/**
 * @brief 设置合约交易表格的列和属性
 * @param table 要设置的表格控件
 * 
 * 合约交易表格的列包括：
 * 1. 名称
 * 2. 最新价
 * 3. 涨跌幅
 * 4. 24h最高
 * 5. 24h最低
 * 6. 持仓量
 * 7. 24h成交量
 * 8. 24h成交额
 */
void MarketTableFactory::setupFuturesTable(QTableWidget* table) {
  // 设置列数和表头
  const int columnCount = 8;
  table->setColumnCount(columnCount);
  table->setHorizontalHeaderLabels(futuresTableHeaders);

  // 设置列宽
  table->setColumnWidth(0, 200);  // 名称
  table->setColumnWidth(1, 150);  // 最新价
  table->setColumnWidth(2, 100);  // 涨跌幅
  table->setColumnWidth(3, 120);  // 24h最高
  table->setColumnWidth(4, 120);  // 24h最低
  table->setColumnWidth(5, 100);  // 持仓量
  table->setColumnWidth(6, 100);  // 24h成交量
  table->setColumnWidth(7, 150);  // 24h成交额
} 
