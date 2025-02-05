#include "mainwindow/mainwindow.h"
#include <QApplication>

/**
 * 自定义消息处理器
 * 用于处理Qt的调试输出，确保在控制台正确显示中文
 *
 * @param type: 消息类型（调试、信息、警告、错误、致命错误）
 * @param context: 消息上下文（文件、行号、函数等信息）
 * @param msg: 消息内容
 */
void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
  // 将 UTF-8 转换为系统本地编码（Windows中文版下为GBK）
  QByteArray localMsg = msg.toLocal8Bit();
  // 根据消息类型添加不同的前缀
  switch (type) {
    case QtDebugMsg:     // 调试信息
      fprintf(stdout, "Debug: %s\n", localMsg.constData());
      fflush(stdout);  // 立即刷新缓冲区
      break;
    case QtInfoMsg:      // 一般信息
      fprintf(stdout, "Info: %s\n", localMsg.constData());
      fflush(stdout);  // 立即刷新缓冲区
      break;
    case QtWarningMsg:   // 警告信息
      fprintf(stdout, "Warning: %s\n", localMsg.constData());
      fflush(stdout);  // 立即刷新缓冲区
      break;
    case QtCriticalMsg:  // 严重错误
      fprintf(stdout, "Critical: %s\n", localMsg.constData());
      fflush(stdout);  // 立即刷新缓冲区
      break;
    case QtFatalMsg:     // 致命错误
      fprintf(stdout, "Fatal: %s\n", localMsg.constData());
      fflush(stdout);  // 立即刷新缓冲区
      abort();  // 终止程序
    }
}

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  qInstallMessageHandler(customMessageHandler);
  MainWindow w;
  w.show();
  return a.exec();
}
