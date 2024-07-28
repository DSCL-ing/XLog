
#include"level.hpp"
#include"util.hpp"
#include"message.hpp"
#include"format.hpp"
#include"sink.hpp"
#include"my_sink.h"
#include"logger.hpp"

void Test_Util(){
  time_t t = log::util::DateUtil::getCurTime();
  std::cout<<ctime(&t)<<"\n";
  std::string pathname = "abc/def/g/test.txt";
  log::util::FileUtil::createDirectory(log::util::FileUtil::getPath(pathname));
}

void Test_LogLevel(){
  std::cout<<log::LogLevel::toString(log::LogLevel::Value::DEBUG)<<"\n";
  std::cout<<log::LogLevel::toString(log::LogLevel::Value::INFO)<<"\n";
  std::cout<<log::LogLevel::toString(log::LogLevel::Value::WARN)<<"\n";
  std::cout<<log::LogLevel::toString(log::LogLevel::Value::ERROR)<<"\n";
  std::cout<<log::LogLevel::toString(log::LogLevel::Value::FATAL)<<"\n";
  std::cout<<log::LogLevel::toString(log::LogLevel::Value::OFF)<<"\n";
  std::cout<<log::LogLevel::toString(log::LogLevel::Value::UNKNOW)<<"\n";
}

void Test_Formatter(){
  log::LogMsg msg(log::LogLevel::Value::DEBUG,"test.cc",12,"root","创建文件失败");
  log::Formatter fmt("abc[%d{%H:%M:%S}]%%[%t][%p][%c][%f:%l]%T %m%n"); 
  std::cout<<fmt.format(msg);
}

void  Test_LogSink(){
  //std::shared_ptr<log::LogSink> lsp = log::SinkFactory::create<log::StdoutSink>();
  //auto lsp = log::SinkFactory::create<log::FileSink>("logs/log");
  //auto lsp = log::SinkFactory::create<log::RollBySizeSink>("logs/roll-",1024*1024);
  std::shared_ptr<log::LogSink> lsp = log::SinkFactory::create<RollbyTimeSink>("logs/roll-",TimeGap::SECOND);

  log::Formatter fmt;
  int count = 0;

  //size_Test
  //for(int i  = 0;i<1024*1024*100;){
  //log::LogMsg msg(log::LogLevel::Value::DEBUG,"test.cc",12,"root","创建文件失败");
  //std::string str = fmt.format(msg);
  //  std::string tmp = str+std::to_string(count++);
  //  lsp->log(tmp.c_str(),tmp.size());
  //  i+=str.size();
  //}
  
  //time_Test
  time_t start_time = log::util::DateUtil::getCurTime();
  while(log::util::DateUtil::getCurTime()<start_time+5){
    log::LogMsg msg(log::LogLevel::Value::DEBUG,"test.cc",12,"root","创建文件失败");
    std::string str = fmt.format(msg);
    std::string tmp = str+std::to_string(count++);
    lsp->log(tmp.c_str(),tmp.size());

  }

}

void Test_Logger(){
  //直接零部件构造太麻烦 --> 建造者模式,简化使用复杂度
  std::shared_ptr<log::LogSink> lsp1 = log::SinkFactory::create<log::StdoutSink>();
  auto lsp2 = log::SinkFactory::create<log::FileSink>("logsByFile/test.log");
  auto lsp3 = log::SinkFactory::create<log::RollBySizeSink>("logsBySize/roll-",1024*1024);
  std::shared_ptr<log::LogSink> lsp4 = log::SinkFactory::create<RollbyTimeSink>("logsByTime/roll-",TimeGap::SECOND);
  std::vector<std::shared_ptr<log::LogSink>> sinks{lsp1,lsp2,lsp3,lsp4};
  log::SyncLogger sl("root",log::LogLevel::Value::INFO,std::make_shared<log::Formatter>("[%d{%H:%M:%S}][%t][%p][%c][%f:%l] %m%n"),sinks);

  time_t start_time = log::util::DateUtil::getCurTime();
  size_t count = 0;
  while(log::util::DateUtil::getCurTime()<start_time+5){
    sl.debug(__FILE__,__LINE__,"%s-%d","打开文件失败",count++);
    sl.info(__FILE__,__LINE__,"%s-%d","打开文件失败",count);
    sl.warn(__FILE__,__LINE__,"%s-%d","打开文件失败",count);
    sl.error(__FILE__,__LINE__,"%s-%d","打开文件失败",count);
    sl.fatal(__FILE__,__LINE__,"%s-%d","打开文件失败",count);
  }
}

void Test_Builder(){
  std::unique_ptr<log::LoggerBuilder> builder (new log::LocalLoggerBuilder());
  builder->buildFormatter();
  // builder->buildLoggerLevel();
  builder->buildLoggerName("root");
  builder->buildSink<RollbyTimeSink>("logsByTime/roll-",TimeGap::SECOND);
  builder->buildSink<log::RollBySizeSink>("logsBySzie/roll-",1024*1024);
  builder->buildSink<log::FileSink>("logsByfile");
  // builder->buildLoggerType();
  //builder->buildSink();
  auto  sl= builder->build();
  time_t start_time = log::util::DateUtil::getCurTime();
  size_t count = 0;
  while(log::util::DateUtil::getCurTime()<start_time+2){
    sl->debug(__FILE__,__LINE__,"%s-%d","打开文件失败",count++);
    sl->info(__FILE__,__LINE__,"%s-%d","打开文件失败",count);
    sl->warn(__FILE__,__LINE__,"%s-%d","打开文件失败",count);
    sl->error(__FILE__,__LINE__,"%s-%d","打开文件失败",count);
    sl->fatal(__FILE__,__LINE__,"%s-%d","打开文件失败",count);
}
}

int main(){
  //Test_LogLevel();
  //Test_Formatter();
  //Test_LogSink();
  //Test_Logger();
  Test_Builder();
  return 0;
}
