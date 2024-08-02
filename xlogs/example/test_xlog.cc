#include"../include/xlog.h"


void Test_Global(){ 
  log::Logger::s_ptr logger = log::LoggerManager::getInstance().getLogger("global_logger");
  size_t count = 0;
  logger->debug(__FILE__, __LINE__, "%s-%d", "打开文件失败", count);
  logger->info(__FILE__, __LINE__, "%s-%d", "打开文件失败", count);
  logger->warn(__FILE__, __LINE__, "%s-%d", "打开文件失败", count);
  logger->error(__FILE__, __LINE__, "%s-%d", "打开文件失败", count);
  while (count<50000) {
    logger->fatal(__FILE__, __LINE__, "%s-%d", "打开文件失败", count++);
  }
}

void test(){

  std::unique_ptr<log::LoggerBuilder> builder (new log::GlobalLoggerBuilder());
  builder->buildLoggerName("global_logger");
  //builder->buildLoggerType(log::LoggerType::LOGGER_ASYNC);
  // builder->buildFormatter();
  // builder->buildLoggerLevel();
  //builder->buildSink<RollbyTimeSink>("logsByTime/roll-", TimeGap::SECOND);
  //builder->buildSink<log::RollBySizeSink>("logsBySzie/roll-", 1024 * 1024);
  //builder->buildEnableUnsafeAsync();
  //builder->buildSink<log::FileSink>("logsByfile/async.log");
  builder->buildSink<log::StdoutSink>();
  builder->build();
  Test_Global();
}

int main(){
  DEBUG("%s","hello xlog!");
  INFO("%s","hello xlog!");
  WARN("%s","hello xlog!");
  ERROR("%s","hello xlog!");
  FATAL("%s","hello xlog!");

  std::unique_ptr<log::LoggerBuilder> builder(new log::GlobalLoggerBuilder());
  builder->buildLoggerName("g_sync_logger");
  builder->build();

  log::getLogger("g_sync_logger")->debug("%s","hello xlog!");

  size_t count = 0;
  while(count<50000){
    log::getLogger("g_sync_logger")->debug("%s-%d","hello xlog!",count++);
  }
  return 0;
}
