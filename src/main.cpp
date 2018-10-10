#include <iostream>

#include "../inc/ver.h"

#include <string>
#include <memory>
#include <vector>
#include <list>
#include <functional>
#include <sstream>
#include <fstream>
#include <ctime>
#include <algorithm>


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
    virtual void subscribe(const std::shared_ptr<IStreamWriter>& observer) = 0;
    virtual void unsubscribe(const std::shared_ptr<IStreamWriter>& observer) = 0;
};

class BulkPool {
  public:
    BulkPool() = default;
    ~BulkPool() = default;

    void push(const std::string& cmd) {
      if(pool_.empty())
        start_bulk_time_ = std::time(nullptr);
      if(!cmd.empty())
        pool_.emplace_back(cmd);
    }

    template <typename F>
    void flush(F pred) {
      if(pred != nullptr)
        pred(start_bulk_time_, pool_);
      pool_.clear();
    }

  private:
    std::vector<std::string> pool_{};
    std::time_t start_bulk_time_{};
};

class CmdInterpreter {
  public:
    CmdInterpreter(size_t bulk_size) : bulk_size_(bulk_size) {
    }

    ~CmdInterpreter() = default;

    bool interpret(const std::string& input, std::string& cmd) {
      if(input.empty()) {
        /// Завершение по EOF.
        if((tokens_ == 0) && (size_ > 0)) {
          size_ = 0;
          return true;
        }
      } else if(input == "{") {
        /// Начало блока.
        size_ = 0;
        if(tokens_++ == 0)
          return true;
      } else if(input == "}") {
        /// Конец блока.
        if(tokens_ > 0)
          if(--tokens_ == 0)
            return true;
      } else {
        /// Команда.
        cmd = input;
        if((tokens_ == 0) && (++size_ == bulk_size_)) {
          size_ = 0;
          return true;
        }
      }
      return false;
    }

  private:

    size_t bulk_size_{};
    size_t size_{};
    size_t tokens_{};
};

class CmdProcessor : public IObservable  {
    using observer_t = std::weak_ptr<IStreamWriter>;
  public:
    CmdProcessor(size_t bulk_size) : interpreter_(bulk_size) {
    }

    virtual ~CmdProcessor() = default;

    void subscribe(const std::shared_ptr<IStreamWriter>& observer) final {
      auto it = std::find_if(observers_.begin(), observers_.end(), [&observer](observer_t& p) {
          return p.lock() == observer;
      });
      if(it == observers_.end())
        observers_.emplace_back(observer);
    }

    void unsubscribe(const std::shared_ptr<IStreamWriter>& observer) final {
      auto it = std::find_if(observers_.begin(), observers_.end(), [&observer](observer_t& p) {
        return p.lock() == observer;
      });
      if(it != observers_.end())
        observers_.erase(it);
    }

    void process(std::istream& is) {
      for(bool is_eof = false; !is_eof;) {
        std::string input;
        is_eof = !std::getline(is, input);

        std::string cmd;
        bool is_bulk_end = interpreter_.interpret(input, cmd);
        bulk_pool_.push(cmd);
        if(is_bulk_end)
          bulk_pool_.flush(publish);
      }
    }

  private:

    using publish_t = std::function<void(const bulk_time_t& bulk_time,
                                         const bulk_t& cmds)>;
    publish_t publish = [&](const bulk_time_t& bulk_time, const bulk_t& bulk) {
      for(auto& it: observers_) {
        if(!it.expired()) {
          auto p = it.lock();
          p->write(bulk_time, bulk);
        }
      }
    };


    std::list<observer_t> observers_{};
    BulkPool bulk_pool_;
    CmdInterpreter interpreter_;
};





int main(int argc, char const *argv[])
{
  std::cout << "bulk version: "
            << ver_major() << "."
            << ver_minor() << "."
            << ver_patch() << std::endl;

  if(argc < 2) {
    std::cerr << "Wrong number of arguments.\n" << std::endl;
    return EXIT_FAILURE;
  }

  auto bulk_size = std::strtoll(argv[1], nullptr, 0);
  if (bulk_size <= 0) {
    std::cerr << "Bulk size must be greater than 0.\n";
    return EXIT_FAILURE;
  }

  std::stringstream ss;
  ss << "cmd1\n";
  ss << "cmd2\n";
  ss << "cmd3\n";
  ss << "cmd4\n";
  ss << "cmd5\n";
  ss << "{\n";
  ss << "cmd1\n";
  ss << "{\n";
  ss << "cmd2\n";
  ss << "{\n";
  ss << "cmd3\n";
  ss << "cmd4\n";
  ss << "}\n";
  ss << "cmd5\n";
  ss << "}\n";
  ss << "cmd6\n";
  ss << "}\n";
  ss << "cmd1\n";
  ss << "cmd2\n";


//  std::fstream fs;
//  fs.open("input.log", std::ios::in);

  CmdProcessor cmd_processor(static_cast<size_t>(bulk_size));

  auto fileWriter = std::make_shared<FileWriter>();
  auto consoleWriter = std::make_shared<ConsoleWriter>();

//  cmd_processor.subscribe(fileWriter);
  cmd_processor.subscribe(consoleWriter);

  cmd_processor.process(ss);

  cmd_processor.unsubscribe(fileWriter);

  std::stringstream ss1;
  ss1 << "cmd1\n";
  ss1 << "cmd2\n";
  ss1 << "cmd3\n";
  cmd_processor.process(ss1);

  return 0;
}
