
#include"level.hpp"
#include"util.hpp"
#include"message.hpp"
#include"format.hpp"

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
  log::Formatter fmt("abc[%d{%H:%M:%S}]%%%[%t][%p][%c][%f:%l]%T %m%n"); 
  std::cout<<fmt.format(msg);
}

int main(){
  //Test_LogLevel();
  Test_Formatter();
  return 0;
}
