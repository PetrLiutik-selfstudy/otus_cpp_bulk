#include <iostream>

#include "../inc/ver.h"

#include <string>
#include <memory>
#include <vector>
#include <functional>
#include <fstream>
#include <ctime>

using bulk_time_t = std::time_t;
using bulk_t = std::vector<std::string>;

class IStreamWriter {
  public:
    virtual ~IStreamWriter() = default;
    virtual void write(const bulk_time_t&, const bulk_t&) = 0;
};

class ConsoleWriter : public IStreamWriter {
  public:
    ConsoleWriter() = default;
    ~ConsoleWriter() = default;

    virtual void write(const bulk_time_t&, const bulk_t& bulk) final {
      std::cout << "bulk: ";
      for(const auto &it : bulk) {
        std::cout << it << (&it != &bulk.back() ? ", " : "\n");
      }
    }
};

class FileWriter : public IStreamWriter {
  public:
    FileWriter() = default;
    ~FileWriter() = default;

    virtual void write(const bulk_time_t& time, const bulk_t& bulk) final {
      std::string file_name = "bulk" + std::to_string(time) + ".log";
      std::fstream fs{file_name, std::ios::app};

      fs << "bulk: ";
      for(const auto& it : bulk) {
        fs << it << (&it != &bulk.back() ? ", " : "\n");
      }
      fs.close();
    }
};


class IObservable {
  public:
    virtual ~IObservable() = default;
    virtual void subscribe(std::unique_ptr<IStreamWriter> observer) = 0;
};

class BulkPool {
  public:
    BulkPool() = default;
    ~BulkPool() = default;

    void push(const std::string& cmd) {
      if(pool_.empty())
        start_bulk_time_ = std::time(nullptr);
      pool_.emplace_back(cmd);
    }

    template <typename F>
    void flush(F pred) {
      if(pred != nullptr)
        pred(start_bulk_time_, pool_);
      pool_.clear();
    }

    size_t size() {
      return pool_.size();
    }

  private:
    std::vector<std::string> pool_{};
    std::time_t start_bulk_time_{};
};


class CmdProcessor : public IObservable  {
  public:
    CmdProcessor(size_t bulk_size) : bulk_size_(bulk_size) {
    }

    virtual ~CmdProcessor() = default;

    void subscribe(std::unique_ptr<IStreamWriter> observer) final {
      stream_writers_.emplace_back(std::move(observer));
    }

    void process(const std::string& cmd) {
      if(cmd == "{") {
        if(open_braces_ == 0)
          bulk_pool_.flush(publish);
        ++open_braces_;
      } else if(cmd == "}") {
        if(open_braces_) {
          --open_braces_;
          if(open_braces_ == 0)
            bulk_pool_.flush(publish);
        }
      } else {
        bulk_pool_.push(cmd);
        if((open_braces_ == 0) && (bulk_pool_.size() == bulk_size_)) {
          bulk_pool_.flush(publish);
        }
      }
    }

  private:

    using publish_t = std::function<void(const std::time_t& bulk_time,
                                         const std::vector<std::string>& cmds)>;
    publish_t publish = [&](const std::time_t& bulk_time, const std::vector<std::string>& bulk) {
      for(auto& it: stream_writers_)
        it->write(bulk_time, bulk);
    };


    std::vector<std::unique_ptr<IStreamWriter>> stream_writers_{};
    BulkPool bulk_pool_;
    size_t   bulk_size_{};
    size_t   open_braces_{};
};





int main()
{
  CmdProcessor cmd_processor(3);

  cmd_processor.subscribe(std::make_unique<FileWriter>());
  cmd_processor.subscribe(std::make_unique<ConsoleWriter>());

  std::string cmd1 = "cmd1";
  std::string cmd2 = "cmd2";
  std::string cmd3 = "cmd3";
  std::string openBrace = "{";
  std::string closeBrace = "}";

  cmd_processor.process(cmd1);
  cmd_processor.process(cmd2);
  cmd_processor.process(openBrace);
  cmd_processor.process(cmd3);
  cmd_processor.process(cmd3);
  cmd_processor.process(cmd3);
  cmd_processor.process(cmd3);
  cmd_processor.process(closeBrace);

  std::cout << "bulk version: "
            << ver_major() << "."
            << ver_minor() << "."
            << ver_patch() << std::endl;

  return 0;
}
