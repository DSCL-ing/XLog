#ifndef LOGGER_HPP
#define LOGGER_HPP

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <iostream>
#include <string>
#include <atomic>
#include <mutex>
#include <cstdio>
#include <cstdarg>
#include "format.hpp"
#include "sink.hpp"
#include "level.hpp"

namespace log
{

  /*
    功能整合
    向外提供接口,完成不同等级的日志输出

    不同等级日志落地功能


    1.日志优先级
    2.日志器名(唯一标识)
      消息对象
      不定参解析 (解析可变参数列表和输出格式,实现和printf相同的方式)
    3.格式化
    4.日志落地
      互斥输出(同步/异步)
      互斥锁
      落地模块数组(多落地位置)

   */

  // 日志器基类
  class Logger
  {
  public:
    Logger(const std::string &logger_name,
           LogLevel::Value level,
           std::shared_ptr<Formatter> formatter,
           std::vector<std::shared_ptr<LogSink>> &sinks)
        : _logger_name(logger_name), _limit_level(level), _formatter_sp(formatter), _sinks(sinks.begin(), sinks.end())
    {
    }

    // 构造对应等级的日志消息对象并格式化成日志消息字符串,然后进行落地输出
    //  是否满足等级 -> 解析不定参 -> serialize(封装,可略){ LogMsg msg -> formatter(pattern).format(msg)-> data -> sink }
    void debug(const std::string &file, size_t line, const char *fmt, ...)
    {
      // 判断等级
      if (_limit_level > LogLevel::Value::DEBUG)
      {
        return;
      }

      // 解析不定参
      va_list arg; // al,arg,ap,arg_ptr,char*
      va_start(arg, fmt);
      char *buf;
      vasprintf(&buf, fmt, arg); // 解析不定参,转换成字符串 --GNU,自动计算并malloc
      va_end(arg);
      serialize(LogLevel::Value::DEBUG, file, line, buf);
    }
    void info(const std::string &file, size_t line, const char *fmt, ...)
    {
      // 判断等级
      if (_limit_level > LogLevel::Value::INFO)
      {
        return;
      }

      // 解析不定参
      va_list arg; // al,arg,ap,arg_ptr,char*
      va_start(arg, fmt);
      char *buf;
      vasprintf(&buf, fmt, arg); // 解析不定参,转换成字符串 --GNU,自动计算并malloc
      va_end(arg);
      serialize(LogLevel::Value::INFO, file, line, buf);
    }
    void warn(const std::string &file, size_t line, const char *fmt, ...)
    {
      // 判断等级
      if (_limit_level > LogLevel::Value::WARN)
      {
        return;
      }
      // 解析不定参
      va_list arg; // al,arg,ap,arg_ptr,char*
      va_start(arg, fmt);
      char *buf;
      vasprintf(&buf, fmt, arg); // 解析不定参,转换成字符串 --GNU,自动计算并malloc
      va_end(arg);
      serialize(LogLevel::Value::WARN, file, line, buf);
    }
    void error(const std::string &file, size_t line, const char *fmt, ...)
    {
      // 判断等级
      if (_limit_level > LogLevel::Value::ERROR)
      {
        return;
      }

      // 解析不定参
      va_list arg; // al,arg,ap,arg_ptr,char*
      va_start(arg, fmt);
      char *buf;
      vasprintf(&buf, fmt, arg); // 解析不定参,转换成字符串 --GNU,自动计算并malloc
      va_end(arg);
      serialize(LogLevel::Value::ERROR, file, line, buf);
    }
    void fatal(const std::string &file, size_t line, const char *fmt, ...)
    {
      // 判断等级
      if (_limit_level > LogLevel::Value::FATAL)
      {
        return;
      }

      // 解析不定参
      va_list arg; // al,arg,ap,arg_ptr,char*
      va_start(arg, fmt);
      char *buf;
      vasprintf(&buf, fmt, arg); // 解析不定参,转换成字符串 --GNU,自动计算并malloc
      va_end(arg);
      serialize(LogLevel::Value::FATAL, file, line, buf);
    }

  protected:
    void serialize(LogLevel::Value level, const std::string &file, size_t line, const std::string &buf)
    {
      // 构造消息对象
      LogMsg msg(level, file, line, _logger_name, buf);

      // 格式化
      std::string str = _formatter_sp->format(msg);

      // 日志:w
      log(str.c_str(), str.size());
    }
    virtual void log(const char *data, size_t len) = 0;

  protected:
    std::string _logger_name;
    std::atomic<LogLevel::Value> _limit_level; // 枚举成员本质属整型类 -- 多线程输出日志,频繁访问,竞态
    std::shared_ptr<Formatter> _formatter_sp;
    std::vector<std::shared_ptr<LogSink>> _sinks;
    std::mutex _mutex; // 防止出现竞态条件

  }; // class logger  __END__

  
 
//与任务同步进行
  class SyncLogger : public Logger
  {
  public:
    SyncLogger(const std::string &logger_name,
               LogLevel::Value level,
               std::shared_ptr<Formatter> formatter,
               std::vector<std::shared_ptr<LogSink>> &sinks)
        : Logger(logger_name, level, formatter, sinks)
    {
    }

  protected:
    void log(const char *data, size_t len) override
    {
      std::unique_lock<std::mutex> lock(_mutex);
      // 日志落地
      for (auto &sink : _sinks)
      {
        sink->log(data, len);
      }
    }
  };


//同步写日志过程(直接落地)可能写的比较慢,写入量多等,为了避免因写日志过程阻塞带来的影响,实现异步落地日志器
//异步:(间接落地)不让业务线程进行日志的实际落地操作,而是将日志消息放到缓冲区(一块指定的内存)中,让一个专门的异步线程去将该缓冲区中的数据进行处理
 
//异步优点:避免了同步日志可能因IO问题阻塞的情况
//异步缺点:理想情况下,没有同步日志快 


//日志一般只会用一个线程 -- 主要资源用于业务
//但也要保证线程安全 -- 

  //异步缓冲区实现路程:
  /*
  1. 队列 先进先出特性 --> STL队列:链式队列,输入输出过程频繁申请与释放内存,性能浪费严重. 生产消费竞争锁,效率低
  2. 环形队列 定长,循环使用 --> 生产消费模型:竞态行为不那么严重: 主要瓶颈:高并发场景中信号量为主要性能开销,信号量涉及内核态,频繁切换开销
  3. 双缓冲 交换---> 避免生产与消费频繁的锁冲突(串行化) 优点:只在生产满时或消费完时发生生产消费一次互斥; 输出: 一次性IO输出,减少IO次数,提高效率
  */

 /*
  缓冲区设计: 一个缓冲区负责输入,另一个缓冲区负责输出; 当其中一个满足条件时,进行角色交换;两个同时满足条件时,休眠
  存储数据类型: 只存储格式化好的字符串 -- 避免Msg对象频繁创建带来的性能开销
  数据结构: std::vector -- 
  读指针rseek: 指向当前读取的位置,读到写指针位置时,表明读完,交换
  写指针wseek: 指向当前写入的位置,写到读指针位置时,表示写满,交换

  提供的缓冲区管理操作:
  1.向缓冲区中写入数据
  2.从缓冲区中读入数据(虚:读数据会再次发生拷贝,原位置不需要保留 --> 不读,直接移动 --> 几乎与同步日志效率一样)
    1.获取可读数据起始地址;
    2.获取刻度数据长度
  3.初始化
  4.交换(地址交换)
 */


  class AsyncLogger : public Logger
  {
  };

  enum class LoggerType
  {
    LOGGER_SYNC,
    LOGGER_ASYNC
  };

  // 建造者模式: 简化构造流程
  // 有两种日志器对象,分别是同步和异步日志器,直接构造logger太繁琐,使用建造者模式,简化构造流程
  // 抽象logger建造者基类,派生出同步模式与异步模式建造者
  class LoggerBuilder
  {
  public:
    LoggerBuilder()
        : _limit_level(LogLevel::Value::INFO), _logger_type(LoggerType::LOGGER_SYNC)
    {
    }

    void buildLoggerType(LoggerType logger_type = LoggerType::LOGGER_SYNC);
    void buildLoggerName(const std::string &name) { _logger_name = name; }
    void buildLoggerLevel(LogLevel::Value level = LogLevel::Value::INFO) { _limit_level = level; }
    void buildFormatter(const std::string &pattern = "")
    {
      if (pattern.empty())
      {
        _formatter_sp = std::make_shared<Formatter>();
        return;
      }
      _formatter_sp = std::make_shared<Formatter>(pattern);
    }
    template <class SinkType, class... Args>
    void buildSink(Args &&...args)
    {
      std::shared_ptr<LogSink> sink_sp = SinkFactory::create<SinkType>(std::forward<Args>(args)...);
      _sinks.push_back(sink_sp);
    }

    virtual std::shared_ptr<Logger> build() = 0;

  protected:
    LoggerType _logger_type;
    std::string _logger_name;
    std::atomic<LogLevel::Value> _limit_level;
    std::shared_ptr<Formatter> _formatter_sp;
    std::vector<std::shared_ptr<LogSink>> _sinks; // 优化:使用set,保证唯一
  };

  class LocalLoggerBuilder : public LoggerBuilder
  {
  public:
    std::shared_ptr<Logger> build() override
    {
      assert(!_logger_name.empty());
      if(_logger_name.empty()){
         std::cout<<"logger_name is empty"<<std::endl;
      }
      if (_sinks.empty())
      {
        buildSink<StdoutSink>(); // 默认为标准输出
      }
      if (_logger_type == LoggerType::LOGGER_ASYNC)
      {
      }
      return std::make_shared<SyncLogger>(_logger_name, _limit_level, _formatter_sp, _sinks);
    }
  };

} // namespace_log_END

#endif
