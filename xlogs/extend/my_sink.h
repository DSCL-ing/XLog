#ifndef MY_SINK_H
#define MY_SINK_H

#include<iostream>
#include<string>
#include"../include/util.hpp"
#include"../include/sink.hpp"
#include<ctime>


//写死 -- 简易
//自定义时间秒,或者宏,或者参考chrono时间
enum class TimeGap{
  SECOND,
  MINUTE,
  HOUR,
  DAY
};

class RollbyTimeSink :public log::LogSink {
  public:
    RollbyTimeSink(std::string basename,TimeGap timegap)
      :_basename(basename),_start_timestamp(log::util::DateUtil::getCurTime())
    {
      switch(timegap){
        case TimeGap::SECOND:_gap_size = 1;break;
        case TimeGap::MINUTE:_gap_size = 60;break;
        case TimeGap::HOUR:_gap_size=3600;break;
        case TimeGap::DAY:_gap_size=3600*24;break;
      }
      log::util::FileUtil::createDirectory(log::util::FileUtil::getPath(_basename));
      std::string filename = createNewFileName();
      _ofs.open(filename,std::ios::binary|std::ios::app);
      if(!_ofs.is_open()){
        std::cout<<"RollbyTimeSink:文件打开失败"<<"\n";
        abort();
      }
      _cur_gap = (log::util::DateUtil::getCurTime()-_start_timestamp)/_gap_size;
    }


    void log(const char* data,size_t len)override{
      size_t new_gap = (log::util::DateUtil::getCurTime() - _start_timestamp)/_gap_size;

      if(new_gap>_cur_gap){
        _ofs.close();
        log::util::FileUtil::createDirectory(log::util::FileUtil::getPath(_basename));
        std::string filename = createNewFileName();
        _ofs.open(filename,std::ios::binary|std::ios::app);
        if(!_ofs.is_open()){
          std::cout<<"RollbyTimeSink:文件打开失败"<<"\n";
          abort();
        }
      }
      _ofs.write(data,len);
      if(!_ofs.good()){
        std::cout<<"RollbyTimeSink:log():文件写入异常"<<"\n";
        abort();
      }

    }

  private:
    std::string createNewFileName(){
      //根据当前时间构建 // ./logs/base-20230102030507.log
      time_t timestamp = log::util::DateUtil::getCurTime();
      struct tm tm; //或者命名lt:localtime
      localtime_r(&timestamp,&tm);

      std::stringstream ss;//自动类型推导特性
      ss<<_basename;
      ss<<tm.tm_year+1900;
      ss<<tm.tm_mon+1;
      ss<<tm.tm_mday;
      ss<<tm.tm_hour;
      ss<<tm.tm_min;
      ss<<tm.tm_sec;
      ss<<".log";

      return ss.str();
    }
  private:
    std::string _basename;//基础名
    std::ofstream _ofs;   //写入文件
    size_t _gap_size;     //时间间隔大小/周期period
    size_t _cur_gap;      //当前是第几个间隔
    time_t _start_timestamp; //起始时间

};

#endif
