/**
 *  @file      main.cpp
 *  @brief     Main for cspec.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-11
 *  @date      2021-11-11
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
            << "Endian: " << (cspec::cpu::endian() == cspec::cpu::endian_t::big ? "big" : "little") << '\n';

  std::cout << "Vendor: " << cspec::cpu::vendor() << '\n' << "Name: " << cspec::cpu::name() << '\n';

  return 0;
}
