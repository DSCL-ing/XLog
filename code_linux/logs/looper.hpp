#ifndef LOOPER_HPP
#define LOOPER_HPP

#include<iostream>
#include<thread>
#include<condition_variable>
#include<atomic>
#include<functional>
#include"buffer.hpp"

namespace log{
//异步工作器 looper:双缓冲循环
/*
    启动 -- 停止
    两个缓冲区
    写入(push) --- 输出(异步 + 回调)

*/

using Functor = std::function<void(Buffer&)>; //处理缓冲区的任务
    
    
    class AsyncLooper{
    public:
        AsyncLooper(const Functor& callback)
        :_stop(false),
        _callback(callback),
        _thread(&AsyncLooper::threadEntry,this)
        {}
        ~AsyncLooper(){
            
        }
        
        void push(const char* data, size_t len){
          /*
          容量足够,写入,否则阻塞
          写入数据后,通知消费者 --- 只有生产者知道有没有数据
          */
          std::unique_lock<std::mutex> lock(_mutex);
          _cond_pro.wait(lock,[&](){return len>_buf_pro.writeAbleSize()?false:true;});
          _buf_pro.push(data,len);
          _cond_con.notify_all();
        }

        //异步任务线程入口
        void threadEntry(){
          while(1){
            {
              std::unique_lock<std::mutex> lock(_mutex);
              _cond_con.wait(lock,[&](){return !_buf_con.empty()?true:false;}); //捕获this
              _buf_con.swap(_buf_pro);
            }
            _cond_pro.notify_all();
              _callback(_buf_con);
              _buf_con.reset();

          }
        }

    private:
        std::atomic<bool> _stop; //启停标记

        std::mutex _mutex; 
        std::condition_variable _cond_pro; //producer
        std::condition_variable _cond_con; //consumer

        Functor _callback; //输出任务
        std::thread _thread;    //异步输出任务线程

        Buffer _buf_pro; 
        Buffer _buf_con; //资源自动释放
    };

}

#endif

