#include"../include/xlog.h"
#include"../extend/my_sink.h"

void Test_Util()
{
  time_t t = log::util::DateUtil::getCurTime();
  std::cout << ctime(&t) << "\n";
  std::string pathname = "abc/def/g/test.txt";
  log::util::FileUtil::createDirectory(log::util::FileUtil::getPath(pathname));
}

void Test_LogLevel()
{
  std::cout << log::LogLevel::toString(log::LogLevel::Value::DEBUG) << "\n";
  std::cout << log::LogLevel::toString(log::LogLevel::Value::INFO) << "\n";
  std::cout << log::LogLevel::toString(log::LogLevel::Value::WARN) << "\n";
  std::cout << log::LogLevel::toString(log::LogLevel::Value::ERROR) << "\n";
  std::cout << log::LogLevel::toString(log::LogLevel::Value::FATAL) << "\n";
  std::cout << log::LogLevel::toString(log::LogLevel::Value::OFF) << "\n";
  std::cout << log::LogLevel::toString(log::LogLevel::Value::UNKNOW) << "\n";
}

//日志输出格式化类与日志消息类测试
void Test_Formatter()
{
  //消息类测试:组织消息类
  log::LogMsg msg(log::LogLevel::Value::DEBUG, "test.cc", 12, "root", "创建文件失败");
  //输出格式化类测试:
  //1. 指定输出规则
  log::Formatter fmt("abc[%d{%H:%M:%S}]%%[%t][%p][%c][%f:%l]%T %m%n");
  //2. 验证:将格式化字符流喂给标准输出流
  std::cout << fmt.format(msg);
}

void Test_LogSink()
{
   std::shared_ptr<log::LogSink> lsp1 = log::SinkFactory::create<log::StdoutSink>();
   auto lsp2 = log::SinkFactory::create<log::FileSink>("logs/log");
   auto lsp3 = log::SinkFactory::create<log::RollBySizeSink>("logs/rollbySize/log-",1024*1024);
  std::shared_ptr<log::LogSink> lsp4 = log::SinkFactory::create<RollbyTimeSink>("logs/rollbyTime/log-", TimeGap::SECOND);

  //使用默认规则
  log::Formatter fmt;

  //line_Test:输出指定行数的日志
  int count = 50000;
  for(int i  = 0;i<=count;i++){
    log::LogMsg msg(log::LogLevel::Value::DEBUG,"test.cc",12,"root","创建文件失败");
    std::string str = fmt.format(msg);
    std::string tmp = std::to_string(i)+str;
    lsp1->log(tmp.c_str(),tmp.size());
    lsp2->log(tmp.c_str(),tmp.size());
    lsp3->log(tmp.c_str(),tmp.size());
    lsp4->log(tmp.c_str(),tmp.size());
  }

  // time_Test:输出5秒的日志
  //time_t start_time = log::util::DateUtil::getCurTime();
  //while (log::util::DateUtil::getCurTime() < start_time + 5)
  //{
  //  log::LogMsg msg(log::LogLevel::Value::DEBUG, "test.cc", 12, "root", "创建文件失败");
  //  std::string str = fmt.format(msg);
  //  std::string tmp = str + std::to_string(count++);
  //  //落地
  //  lsp->log(tmp.c_str(), tmp.size());
  //}
}

void Test_Logger()
{
  std::shared_ptr<log::LogSink> lsp1 = log::SinkFactory::create<log::StdoutSink>();
  auto lsp2 = log::SinkFactory::create<log::FileSink>("logsByFile/test.log");
  auto lsp3 = log::SinkFactory::create<log::RollBySizeSink>("logsBySize/roll-", 1024 * 1024);
  std::shared_ptr<log::LogSink> lsp4 = log::SinkFactory::create<RollbyTimeSink>("logsByTime/roll-", TimeGap::SECOND);
  //落地数组
  std::vector<std::shared_ptr<log::LogSink>> sinks{lsp1, lsp2, lsp3, lsp4};

  //格式化器
  std::shared_ptr<log::Formatter> fmt_sp(new log::Formatter("[%d{%H:%M:%S}][%t][%p][%c][%f:%l] %m%n"));

  //同步日志器: synclogger(日志器名,日志等级,格式化器,落地方式);
  log::SyncLogger sl("root", log::LogLevel::Value::INFO, fmt_sp, sinks);

  //输出5s日志
  time_t start_time = log::util::DateUtil::getCurTime();
  size_t count = 0;
  while (log::util::DateUtil::getCurTime() < start_time + 5)
  {
    //sl等级:INFO
    //不显示
    sl.debug(__FILE__, __LINE__, "%s-%d", "打开文件失败", count++); 
    //显式
    sl.info(__FILE__, __LINE__, "%s-%d", "打开文件失败", count);
    sl.warn(__FILE__, __LINE__, "%s-%d", "打开文件失败", count);
    sl.error(__FILE__, __LINE__, "%s-%d", "打开文件失败", count);
    sl.fatal(__FILE__, __LINE__, "%s-%d", "打开文件失败", count);
  }
}

void Test_Builder()
{
  // 直接零部件构造太麻烦 --> 建造者模式,简化使用复杂度
  std::unique_ptr<log::LoggerBuilder> builder(new log::LocalLoggerBuilder());
  //builder->buildFormatter();  //不显式构造表示使用默认解析器
  // builder->buildLoggerLevel(); //不显式构造表示使用默认等级
  builder->buildLoggerName("root");
  builder->buildSink<log::StdoutSink>();
  builder->buildSink<log::RollBySizeSink>("logsBySzie/roll-", 1024 * 1024);
  builder->buildSink<log::FileSink>("logsByfile/file.log");
  builder->buildSink<RollbyTimeSink>("logsByTime/roll-", TimeGap::SECOND);
  // builder->buildLoggerType(); //不显式构造表示使用默认日志器类型
  auto sl = builder->build();

  //输出2秒
  time_t start_time = log::util::DateUtil::getCurTime();
  size_t count = 0;
  while (log::util::DateUtil::getCurTime() < start_time + 2) {
    sl->debug(__FILE__, __LINE__, "%s-%d", "打开文件失败", count++);
    sl->info(__FILE__, __LINE__, "%s-%d", "打开文件失败", count);
    sl->warn(__FILE__, __LINE__, "%s-%d", "打开文件失败", count);
    sl->error(__FILE__, __LINE__, "%s-%d", "打开文件失败", count);
    sl->fatal(__FILE__, __LINE__, "%s-%d", "打开文件失败", count);
  }
}

void Test_Buffer(){
  //测试思路:
  /*
     提前准备好本地文件
     将本地文件读入内存 
     将内存中数据写入到buffer
     将buffer中数据输出到文件  
     对比文件是否相同 
     */


  if(access("logsByfile/file.log",F_OK)){
    std::unique_ptr<log::LoggerBuilder> builder(new log::LocalLoggerBuilder());
    builder->buildLoggerName("root");
    builder->buildSink<log::FileSink>("logsByfile/file.log");
    auto sl = builder->build();
    size_t count = 1;
    while (count<=50000) {
      sl->debug(__FILE__, __LINE__, "%s-%d", "打开文件失败", count);
      sl->info(__FILE__, __LINE__, "%s-%d", "打开文件失败", count);
      sl->warn(__FILE__, __LINE__, "%s-%d", "打开文件失败", count);
      sl->error(__FILE__, __LINE__, "%s-%d", "打开文件失败", count);
      sl->fatal(__FILE__, __LINE__, "%s-%d", "打开文件失败", count);
      count++;
    }
  }


  std::ifstream ifs("logsByfile/file.log",std::ios::binary);
  if(ifs.is_open() == false){
    std::cout<<"ifs open fail\n";
    abort();
  }
  ifs.seekg(0,std::ios::end); //文件指针跳转到相对于文件末尾+0的位置
  size_t size = ifs.tellg() ; //获取文件指针当前的位置
  ifs.seekg(std::ios::beg); //文件指针跳转到绝对位置beg,即文件起始

  //读到内存
  std::string body;
  body.resize(size);
  ifs.read(&body[0],size); //read(char* ,len); //字节流
  if(ifs.good() == false){
    std::cout<<"ifs good() is false\n";
    abort();
  }
  ifs.close();


  //写到缓冲区
  log::Buffer buffer;
  //int was = buffer.writeAbleSize();
  int was = body.size();
  for(size_t i = 0; i<body.size();i++){
    buffer.push(&body[i],1);  //逐字节拷贝
  }
  int ras = buffer.readAbleSize();

  //缓冲区写到文件 -- 尽可能测试更多的接口
  std::ofstream ofs("./logsByfile/out.log",std::ios::binary);
  if(ofs.is_open() == false){
    std::cout<<"ifs is_open fail\n";
    abort();
  }

  size_t buffersize = buffer.readAbleSize();

  // for(size_t i = 0; i<buffersize;i++){
  //   ofs.write(buffer.begin(),1);
  //   buffer.moveReader(1);
  // }
  ofs.write(buffer.begin(),buffersize);

  if(ofs.good() == false){
    std::cout<<"ifs good() is false\n";
    ofs.close();
    abort();
  }
  ofs.close();

  std::cout<<"原始数据大小"<<was<<std::endl;
  std::cout<<"可读数据大小"<<ras<<std::endl;

  //util : md5sum 比较
}

void Test_Async(){
  std::unique_ptr<log::LoggerBuilder> builder(new log::LocalLoggerBuilder());
  builder->buildLoggerName("root");
  builder->buildLoggerType(log::LoggerType::LOGGER_ASYNC);
  // builder->buildFormatter();   //默认
  // builder->buildLoggerLevel(); //默认
  builder->buildSink<RollbyTimeSink>("logsByTime/roll-", TimeGap::SECOND);
  builder->buildSink<log::RollBySizeSink>("logsBySzie/roll-", 1024 * 1024 * 1024);
  builder->buildEnableUnsafeAsync();
  builder->buildSink<log::FileSink>("logsByfile/async.log");
  // builder->buildSink<log::StdoutSink>();
  auto sl = builder->build();
  // time_t start_time = log::util::DateUtil::getCurTime();
  size_t count = 0;
  while (count<=500000) {
    sl->debug(__FILE__, __LINE__, "%s-%d", "打开文件失败", count);
    sl->info(__FILE__, __LINE__, "%s-%d", "打开文件失败", count);
    sl->warn(__FILE__, __LINE__, "%s-%d", "打开文件失败", count);
    sl->error(__FILE__, __LINE__, "%s-%d", "打开文件失败", count);
    sl->fatal(__FILE__, __LINE__, "%s-%d", "打开文件失败", count++);
  }

}

void Test_Global(){ 
  std::shared_ptr<log::Logger> logger = log::LoggerManager::getInstance().getLogger("global_logger");
  size_t count = 0;
  logger->debug(__FILE__, __LINE__, "%s-%d", "打开文件失败", count);
  logger->info(__FILE__, __LINE__, "%s-%d", "打开文件失败", count);
  logger->warn(__FILE__, __LINE__, "%s-%d", "打开文件失败", count);
  logger->error(__FILE__, __LINE__, "%s-%d", "打开文件失败", count);
  while (count<50000) {
    logger->fatal(__FILE__, __LINE__, "%s-%d", "打开文件失败", count++);
  }

}

int main()
{
  //Test_Util();
  //Test_LogLevel();
  //Test_Formatter();
  //Test_LogSink();
  //Test_Logger();
  //Test_Builder();
  //Test_Buffer();
  //Test_Async();

  std::unique_ptr<log::LoggerBuilder> builder (new log::GlobalLoggerBuilder());
  builder->buildLoggerName("global_logger");
  builder->buildLoggerType(log::LoggerType::LOGGER_ASYNC);
  // builder->buildFormatter();
  // builder->buildLoggerLevel();
  builder->buildSink<RollbyTimeSink>("logsByTime/roll-", TimeGap::SECOND);
  builder->buildSink<log::RollBySizeSink>("logsBySzie/roll-", 1024 * 1024);
  builder->buildEnableUnsafeAsync();
  builder->buildSink<log::FileSink>("logs/async.log");
  //builder->buildSink<log::StdoutSink>();
  builder->build();
  Test_Global();

  return 0;
}
