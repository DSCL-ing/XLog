
#include"level.hpp"
#include"util.hpp"
#include"message.hpp"
#include"format.hpp"
#include"sink.hpp"
#include"my_sink.h"

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

int main(){
  //Test_LogLevel();
  //Test_Formatter();
  Test_LogSink();
  return 0;
}
