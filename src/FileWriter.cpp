#include "../inc/FileWriter.h"

#include <fstream>

namespace bulk {

void FileWriter::write(const std::time_t& time, const std::vector<std::string>& bulk) {
  std::string file_name = "bulk" + std::to_string(time) + ".log";
  std::fstream fs{file_name, std::ios::app};

  if(fs.is_open()) {
    fs << "bulk: ";
    for(const auto& it : bulk) {
      fs << it << (&it != &bulk.back() ? ", " : "\n");
    }
    fs.close();
  }
}

} // namespace bulk.
