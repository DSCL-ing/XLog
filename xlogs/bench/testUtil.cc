#include"../include/xlog.h"

#include<iostream>
#include<thread>
#include<vector>
#include<chrono>

void benchMark(const std::string&logger_name,size_t thr_count,size_t msg_count,size_t msg_len){
  //获取日志器
  //组织指定长度的日志消息
  //创建指定数量的线程
  //计时
  //写日志
  //结束计时
  //计算总耗时
  //整理输出

  std::shared_ptr<log::Logger> logger = log::getLogger(logger_name); 
  if(logger.get() == nullptr){
    return ;
  }

  std::string str(msg_count-1,'a'); //末尾留一个,自动填充'\0'

  std::vector<std::thread> threads;
  std::vector<double> costs;


  for(int i = 0;i<thr_count;i++){
    threads.emplace_back([&,i](){
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
        for(int j = 0;j<msg_count;j++){
          logger->debug("%s",str.c_str());
        }
        std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> cost = end - start;
        costs.push_back(cost.count());
    });
  }



}


int main(){

  return 0;
}


/*
   测试环境
   测试方法
   测试结果
   */

/* 测试环境:
CPU:
RAM:
ROM:
OS:
*/

/*测试方法:
  测试方式:平均每秒能打印多少条日志到文件
  原理:每秒打印的日志数 = 打印的日志条数/总的打印日志消耗时间
  测试要素:同步/异步 & 单线程/多线程

//其他
- 100w条指定长度的日志输出所耗时间(耗时)
- 每秒可以输出多少条日志     (条数)
- 每秒可以输出多少大小的日志 (大小)
*/

//接口设计
/*
   输入:可控变量 = 参数 =  日志器名称, 线程数量, 日志数量, 单条日志长度, 同步/异步 ;
   {
   计时start
   线程负责平均处理日志输出
   计时end


   输出:
   每秒输出量 = 总条数/时间
   每秒输出大小 = 总条数*大小/时间
   } 

//注:异步不考虑写入磁盘,只考虑写入到缓冲区(纯内存写入)

*/ 
