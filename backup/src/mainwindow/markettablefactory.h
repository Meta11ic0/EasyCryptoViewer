#ifndef MARKETTABLEFACTORY_H
#define MARKETTABLEFACTORY_H

#include <QTableWidget>
#include <QString>
#include "../constant/constants.h"


class MarketTableFactory {
public:
    // 根据主导航和子导航ID创建表格
    static QTableWidget* createTable(const NavState& state);
    // 根据类型创建表格
    static QTableWidget* createTableByType(MarketTableType type);
private:
    // 设置不同类型表格的列
    static void setupCoinTable(QTableWidget* table);
    static void setupSpotTable(QTableWidget* table);
    static void setupFuturesTable(QTableWidget* table);
    // 设置表格的通用属性
    static void setupCommonProperties(QTableWidget* table);
};

#endif // MARKETTABLEFACTORY_H 