#ifndef CONFIG_H  
#define CONFIG_H

const int PING_INTERVAL = 20000; // 心跳间隔（毫秒）
const int PONG_TIMEOUT = 5000; // 心跳超时时间（毫秒）

const char* const REST_PUBLIC_URL = "https://www.okx.com"; // REST频道
const char* const WS_PUBLIC_URL = "wss://wspap.okx.com:8443/ws/v5/public"; // 模拟盘WebSocket公共频道
const char* const WS_PRIVATE_URL = "wss://wspap.okx.com:8443/ws/v5/private"; // 模拟盘WebSocket私有频道
const char* const WS_BUSINESS_URL = "wss://wspap.okx.com:8443/ws/v5/business"; // 模拟盘WebSocket业务频道

const bool IS_SIMULATION = true; // 是否为模拟盘
#endif // CONFIG_H
