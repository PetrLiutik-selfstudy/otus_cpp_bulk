#include <iostream>

#include "CmdProcessor.h"
#include "ConsoleWriter.h"
#include "FileWriter.h"
#include "ver.h"

#include <iostream>


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


  bulk::CmdProcessor cmd_processor(static_cast<size_t>(bulk_size));

  auto fileWriter = std::make_shared<bulk::FileWriter>();
  auto consoleWriter = std::make_shared<bulk::ConsoleWriter>();

  cmd_processor.subscribe(fileWriter);
  cmd_processor.subscribe(consoleWriter);

  cmd_processor.process(std::cin);

  cmd_processor.unsubscribe(consoleWriter);
  cmd_processor.unsubscribe(fileWriter);

  return 0;
}
