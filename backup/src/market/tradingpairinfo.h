#ifndef TRADINGPAIRMANAGER_H
#define TRADINGPAIRMANAGER_H

#include <QObject>
#include <QMap>
#include <QString>

// 交易对信息结构体
struct TradingPairInfo {
  QString instType;      // 产品类型
  QString instId;        // 产品ID，如 BTC-USDT
  QString uly;           // 标的指数，如 BTC-USD，仅适用于 杠杆/交割/永续/期权
  QString instFamily;    // 交易品种，如 BTC-USD，仅适用于 杠杆/交割/永续/期权
  QString category;      // 币种类别（已废弃）
  QString baseCcy;       // 交易货币币种，如 BTC-USDT 中的 BTC，仅适用于 币币/币币杠杆
  QString quoteCcy;      // 计价货币币种，如 BTC-USDT 中的 USDT，仅适用于 币币/币币杠杆
  QString settleCcy;     // 盈亏结算和保证金币种，如 BTC，仅适用于 交割/永续/期权
  QString ctVal;         // 合约面值，仅适用于 交割/永续/期权
  QString ctMult;        // 合约乘数，仅适用于 交割/永续/期权
  QString ctValCcy;      // 合约面值计价币种，仅适用于 交割/永续/期权
  QString optType;       // 期权类型，C：看涨期权，P：看跌期权，仅适用于 期权
  QString stk;           // 行权价格，仅适用于 期权
  QString listTime;      // 上线时间，Unix时间戳的毫秒数格式，如 1597026383085
  QString auctionEndTime; // 竞价结束时间，Unix时间戳的毫秒数格式，如 1597026383085。仅适用于通过集合竞价方式上线的币币，其余情况返回""
  QString expTime;       // 到期时间，仅适用于 交割/期权
  QString lever;         // 最大杠杆倍数，不适用于 期权
  QString tickSz;        // 最小价格变动单位
  QString lotSz;         // 最小交易数量
  QString minSz;         // 最小下单数量
  QString ctType;        // 合约类型，linear：正向合约，inverse：反向合约。仅适用于 交割/永续
  QString alias;         // 合约日期别名，this_week：本周，next_week：次周，quarter：季度，next_quarter：次季度。仅适用于 交割
  QString state;         // 产品状态：live：交易中，suspend：暂停中，preopen：预上线，test：测试中
  QString ruleType;      // 交易规则类型，normal：普通交易 pre_market：盘前交易
  QString maxLmtSz;      // 合约或现货限价单的最大委托数量
  QString maxMktSz;      // 合约或现货市价单的最大委托数量
  QString maxTwapSz;     // 合约或现货时间加权单的最大委托数量
  QString maxIcebergSz;  // 合约或现货冰山委托的最大委托数量
  QString maxTriggerSz;  // 合约或现货计划委托的最大委托数量
  QString maxStopSz;     // 合约或现货止盈止损的最大委托数量
};

class TradingPairManager : public QObject {
  Q_OBJECT

public:
  static TradingPairManager* getInstance();
  // 获取交易对信息
  const TradingPairInfo* getTradingPair(const QString& instId) const;
  // 获取所有交易对
  QList<TradingPairInfo> getAllTradingPairs() const;
  // 获取特定类型的交易对
  QList<TradingPairInfo> getTradingPairsByType(const QString& instType) const;

public slots:
  // 更新交易对信息
  void updateTradingPair(const TradingPairInfo& info);
  // 批量更新交易对信息
  void updateTradingPairs(const QList<TradingPairInfo>& pairs);
  // 清除所有交易对信息
  void clear();

signals:
  // 交易对更新信号
  void tradingPairUpdated(const QString& instId);
  // 交易对列表更新信号
  void tradingPairsUpdated();

private:
  explicit TradingPairManager(QObject* parent = nullptr);
  ~TradingPairManager() = default;
  
  static TradingPairManager* instance;
  QMap<QString, TradingPairInfo> m_tradingPairs;  // instId -> TradingPairInfo
};

#endif // TRADINGPAIRMANAGER_H
