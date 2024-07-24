
#include<iostream>
#include<fstream>
#include<thread>


int main(){
  while(1){
    std::ifstream ifs("test.cc");
    if(ifs.is_open()){
      std::cout<<"true1"<<std::endl;
    }
    else{
      std::cout<<"false1"<<std::endl;
    }
    if(ifs.good()){
      std::cout<<"true2"<<std::endl;
    }
    else{
      std::cout<<"false2"<<std::endl;
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

}


