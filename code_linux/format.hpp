#ifndef FORMAT_H
#define FORMAT_H

/*
  日志格式化模块
  : 对日志消息进行格式化,组织成指定格式的字符串

pattern成员：保存⽇志输出的格式字符串。 
 %d ⽇期 
 %t 线程id 
 %p ⽇志优先级/级别 -- DEBUG,ERROR...
 %c ⽇志器名称      -- [root]
 %f ⽂件名 
 %l ⾏号 
 %m ⽇志消息 
 %T 缩进 
 %n 换⾏ 

格式化字符串分解:
Exam: 
[%d{%H:%M:%S}][%f:%l]%p%m%n
  1. 非格式化字符子项 [
  2. 格式化字符子项   %p
  3. 日期子项         {%H:%M:%S}
  4. 非格式化字符子项 ]
  5. 非格式化字符子项 [
  6. 文件名子项       %f 
  7. 非格式化字符子项 :
  8. 行号子项         %l
  9. 非格式化字符子项 ]
  10.消息主体子项     %m
  11.换行子项         %n

*/

#include<iostream>
#include<string>
#include<sstream>
#include<ctime>

#include<unistd.h>


#include"util.hpp"
#include"message.hpp"
#include"level.hpp"


namespace log{
  class Format{
    public:
      virtual void format(std::ostream& out,const LogMsg& msg) = 0;
  };

  class TimeFormat :public Format{
    public:
      TimeFormat(const std::string& format = "%T"):_time_fmt(format){ }
      void format(std::ostream &out,const LogMsg& msg) override{
        struct tm t;
        localtime_r(&msg._time,&t);
        char tmp[32] = {0};
        strftime(tmp,31,_time_fmt.c_str(),&t);
        out<<tmp;
      }
    private:
      std::string _time_fmt;
  };

  class TidFormat:public Format{
    public:
      void format(std::ostream &out,const LogMsg& msg){
          out<<msg._tid;    
      }
  };


  //priority level = PRI
  class LevelFormat:public Format{
    public:
      void format(std::ostream &out,const LogMsg& msg){
       	out<<LogLevel::toString(msg._level);
      }
  };

  //LoggerName
  class LoggerFormat:public Format{
    public:
      void format(std::ostream &out,const LogMsg& msg){
        out<<msg._loggername;  
      }
  };

  //FileName
  class FileFormat:public Format{
    public:
      void format(std::ostream &out,const LogMsg& msg){
        out<<msg._filename;
      }
  };
  class LineFormat:public Format{
    public:
      void format(std::ostream &out,const LogMsg& msg){
        out<<msg._line; 
      }
  };
  class MsgFormat:public Format{
    public:
      void format(std::ostream &out,const LogMsg& msg){
        out<<msg._payload; 
      }
  };

  //NewLine
  class NLineFormat:public Format{
    public:
      void format(std::ostream &out,const LogMsg& msg){
        out<<"\n";
      }
  };
  class TabFormat:public Format{
    public:
      void format(std::ostream &out,const LogMsg& msg){
        out<<"\t"; 
      }
  };

  class OtherFormat:public Format{
    public:
      OtherFormat(const std::string& str):_otherstr(str){ }
      void format(std::ostream &out,const LogMsg&msg){
        out<<_otherstr;
      }
    private:
      std::string _otherstr;
  };

} //namespace_log_END

#endif
