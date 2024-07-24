

#include"util.hpp"

int main(){
  time_t t = log::util::DateUtil::getCurTime();
  std::cout<<ctime(&t)<<"\n";
  std::string pathname = "abc/def/g/test.txt";
  log::util::FileUtil::createDirectory(log::util::FileUtil::getPath(pathname));

  return 0;
}
