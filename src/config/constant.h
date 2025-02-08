#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <QStringList>

// 主导航类型
enum class MainNavType {
  COIN = 0,       // 币种
  SPOT = 1,       // 现货交易
  FUTURES = 2,    // 合约交易
  /*
  OPTION = 3,     // 期权
  PRE_MARKET = 4, // 盘前交易
  TRADE_DATA = 5, // 交易数据
  ARBITRAGE = 6   // 套利交易
  */
};

// 币种类型
enum class CoinType {
  ALL = 0,        // 全部
  TOP = 1,        // Top
  NEW = 2,        // New
  AI = 3,         // AI
  MEME = 4,       // Meme
  DEFI = 5,       // DeFi
  LAYER1 = 6,     // Layer 1
  GAMING = 7,     // Gaming
  LAYER2 = 8,     // Layer 2
  INSCRIPTIONS = 9, // Inscriptions
  POW = 10,       // Proof of Work
  STORAGE = 11,   // Storage
  FAN_TOKENS = 12, // Fan Tokens
  NFT = 13,       // NFT
  DEPIN = 14,     // DePIN
  DEX = 15,       // DEX
  GOVERNANCE = 16  // Governance
};

// 计价货币类型
enum class QuoteType {
  USDT = 0,
  USDC = 1,
  CRYPTO = 2
};

// 保证金类型
enum class MarginType {
  USDT = 0,
  USDC = 1,
  CRYPTO = 2
};

// 合约类型
enum class FuturesType {
  ALL = 0,
  PERPETUAL = 1,
  EXPIRY = 2
};

// NavState 结构体
struct NavState {
  MainNavType mainNavId;    // 主导航类型
  CoinType coinId;          // 币种类型
  QuoteType quoteIndex;     // 计价货币类型
  MarginType marginId;      // 保证金类型
  FuturesType futuresId;    // 合约类型
  NavState(
    MainNavType main = MainNavType::COIN,
    CoinType coin = CoinType::AI,
    QuoteType quote = QuoteType::USDT,
    MarginType margin = MarginType::USDT,
    FuturesType futures = FuturesType::ALL
  ) : mainNavId(main), coinId(coin), quoteIndex(quote),
      marginId(margin), futuresId(futures) {}
};

// 表格类型枚举
enum class MarketTableType {
  COIN = 0,       // 币种表格
  SPOT = 1,       // 现货交易表格
  FUTURES = 2     // 合约交易表格
};

// 主导航按钮文本
const QStringList mainNavButtonTexts = { 
  "币种", 
  "现货交易", 
  "合约交易", 
  "期权", 
  "盘前交易", 
  "交易数据", 
  "套利交易"
};

const char* const NavQuoteLabel = "计价货币";

// 计价货币按钮文本
const QStringList quoteButtonTexts = { 
  "USDT", 
  "USDC", 
  "CRYPTO"
};

// 保证金按钮文本
const QStringList marginButtonTexts = { 
  "USDT本位", 
  "USDC本位", 
  "币本位"
};

// 合约按钮文本
const QStringList futuresButtonTexts = { 
  "全部", 
  "永续合约", 
  "到期合约"
};

// 币种按钮文本
const QStringList coinButtonTexts = { 
  "全部", 
  "主流币", 
  "新币种", 
  "人工智能", 
  "Meme", 
  "DeFi", 
  "Layer 1", 
  "游戏代币", 
  "Layer 2", 
  "铭文", 
  "PoW", 
  "存储项目", 
  "粉丝代币", 
  "NFT生态", 
  "DePIN", 
  "DEX", 
  "治理代币", 
  "基础设施", 
  "借贷项目", 
  "元宇宙", 
  "流动性挖矿", 
  "其他分类"
};

// 市场数据表格表头
const QStringList marketTableHeaders = {
  "名称",
  "最新价",
  "涨跌幅"
};

// 现货交易表格表头
const QStringList spotTableHeaders = {
  "名称",
  "最新价",
  "涨跌幅",
  "24h最高",
  "24h最低",
  "24h成交额"
};

// 合约交易表格表头
const QStringList futuresTableHeaders = {
  "名称",
  "最新价",
  "涨跌幅",
  "24h最高",
  "24h最低",
  "持仓量",
  "24h成交量",
  "24h成交额"
};

// 子导航按钮名称
const QStringList subNavButtonName = {
  "quotebutton",
  "marginbutton",
  "coinbutton",
  "futuresbutton"
};
#endif // CONSTANTS_H
