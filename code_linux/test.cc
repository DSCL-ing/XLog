
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

int main(){
  Test_LogLevel();
  return 0;
}
