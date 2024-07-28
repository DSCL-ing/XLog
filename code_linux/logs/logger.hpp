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
