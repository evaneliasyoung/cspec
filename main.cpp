/**
 *  @file      main.cpp
 *  @brief     Main for cspec.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-11
 *  @date      2021-11-16
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "main.h"

int main(int argc, char const *argv[])
{
  cspec::cpu::architecture_t arch = cspec::cpu::architecture();
  std::map<cspec::cpu::architecture_t, std::string> arch_map = {{cspec::cpu::architecture_t::x64, "x64"},
                                                                {cspec::cpu::architecture_t::arm, "arm"},
                                                                {cspec::cpu::architecture_t::itanium, "itanium"},
                                                                {cspec::cpu::architecture_t::x86, "x86"},
                                                                {cspec::cpu::architecture_t::unknown, "unknown"}};

  std::cout << "Arch: " << arch_map.at(arch) << '\n'
            << "Endian: " << (cspec::cpu::endian() == cspec::cpu::endian_t::big ? "big" : "little") << '\n'
            << '\n';

  std::cout << "Vendor: " << cspec::cpu::vendor() << '\n'
            << "Name: " << cspec::cpu::name() << '\n'
            << "Clock: " << cspec::cpu::clock() << '\n'
            << '\n';

  const auto amounts = cspec::cpu::amounts();
  std::cout << "Cores: " << amounts.cores << '\n'
            << "Threads: " << amounts.threads << '\n'
            << "Packages: " << amounts.packages << '\n'
            << '\n';

  for (u8 i = 0; i < 4; ++i)
  {
    const auto cache = cspec::cpu::cache(i);
    std::cout << "L" << (u16)i << " Cache" << '\n'
              << "Size: " << cache.size << '\n'
              << "Line Size: " << cache.line_size << '\n'
              << "Association: " << (u16)cache.association << '\n'
              << '\n';
  }

  return 0;
}
