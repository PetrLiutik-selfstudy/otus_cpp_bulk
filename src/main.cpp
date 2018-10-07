#include <iostream>

#include "../inc/ver.h"

#include <string>
#include <memory>
#include <vector>
#include <functional>

class IStreamWriter {
  public:
    virtual void write(const std::string&) = 0;
};

class ConsoleWriter : public IStreamWriter {
  public:
    virtual void write(const std::string& str) final {
      std::cout << "ConsoleWriter: " << str << std::endl;
    }
};

class FileWriter : public IStreamWriter {
  public:
    virtual void write(const std::string& str) final {
      std::cout << "FileWriter: " << str << std::endl;
    }
};


class IObservable {
  public:
    virtual void subscribe(std::unique_ptr<IStreamWriter> observer) = 0;
};


class BulkPool {

  public:

    void push(std::string& str) {
      pool_.emplace_back(str);
    }

    template <typename F>
    void flush(F pred) {
      for(auto it: pool_)
        pred(it);
      pool_.clear();
    }

    size_t size() {
      return pool_.size();
    }

  private:
    std::vector<std::string> pool_{};
};

class OpenBlockInterpreter {
  public:
    void interpret(std::string& cmd) {
    }

  private:
      size_t open_braces_{};
};


class CloseBlockInterpreter {
  public:
    void interpret(std::string& cmd) {
    }

  private:
      size_t open_braces_{};
};

class CmdInterpreter {
  public:
    void interpret(std::string& cmd) {
    }

  private:
      size_t open_braces_{};
};

class EofInterpreter {
  public:
    void interpret(std::string& cmd) {
    }

  private:
      size_t open_braces_{};
};

class CmdProcessor : public IObservable  {
  public:
    CmdProcessor(size_t bulk_size) : bulk_size_(bulk_size) {
    }

    void subscribe(std::unique_ptr<IStreamWriter> observer) final {
      stream_writers_.emplace_back(std::move(observer));
    }



    void interpret(std::string& cmd) {
      if(cmd == "{") {
        bulk_pool_.flush(publish);
        ++open_braces_;
      } else if(cmd == "}") {
        if(open_braces_) {
          --open_braces_;
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

    std::function<void(const std::string& str)> publish = [&](const std::string& str) {
      for(auto& it: stream_writers_)
        it->write(str);
    };


    std::vector<std::unique_ptr<IStreamWriter>> stream_writers_{};
    BulkPool bulk_pool_;
    bool is_block_{};
    size_t bulk_size_{};

    size_t open_braces_{};
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

  cmd_processor.interpret(cmd1);
  cmd_processor.interpret(cmd2);
  cmd_processor.interpret(openBrace);
  cmd_processor.interpret(cmd3);
  cmd_processor.interpret(closeBrace);

  std::cout << "bulk version: "
            << ver_major() << "."
            << ver_minor() << "."
            << ver_patch() << std::endl;

  return 0;
}
