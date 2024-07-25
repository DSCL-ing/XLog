
#include"level.hpp"
#include"util.hpp"

void Test_Util(){
  time_t t = log::DateUtil::getCurTime();
  std::cout<<ctime(&t)<<"\n";
  std::string pathname = "abc/def/g/test.txt";
  log::FileUtil::createDirectory(log::FileUtil::getPath(pathname));
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
