#include <iostream>

#include "../inc/ver.h"

int main()
{
  std::cout << "bulk version: "
            << ver_major() << "."
            << ver_minor() << "."
            << ver_patch() << std::endl;

  return 0;
}
