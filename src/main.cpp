#include <iostream>

#include "../inc/CmdProcessor.h"
#include "../inc/ConsoleWriter.h"
#include "../inc/FileWriter.h"
#include "../inc/ver.h"

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

  auto file_writer = std::make_shared<bulk::FileWriter>();
  auto console_writer = std::make_shared<bulk::ConsoleWriter>();

  cmd_processor.subscribe(file_writer);
  cmd_processor.subscribe(console_writer);

  cmd_processor.process(std::cin);

  cmd_processor.unsubscribe(console_writer);
  cmd_processor.unsubscribe(file_writer);

  return 0;
}
