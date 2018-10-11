#include <iostream>

#include "CmdProcessor.h"
#include "ConsoleWriter.h"
#include "FileWriter.h"
#include "ver.h"

#include <sstream>
#include <fstream>


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

  bulk::CmdProcessor cmd_processor(static_cast<size_t>(bulk_size));

  auto fileWriter = std::make_shared<bulk::FileWriter>();
  auto consoleWriter = std::make_shared<bulk::ConsoleWriter>();

  cmd_processor.subscribe(fileWriter);
  cmd_processor.subscribe(consoleWriter);

  cmd_processor.process(ss);

//  cmd_processor.unsubscribe(fileWriter);

  std::stringstream ss1;
  ss1 << "cmd1\n";
  ss1 << "cmd2\n";
  ss1 << "cmd3\n";
  cmd_processor.process(ss1);

  return 0;
}
