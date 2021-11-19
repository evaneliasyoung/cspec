/**
 *  @file      main.cpp
 *  @brief     Main for cspec.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-11
 *  @date      2021-11-19
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "main.h"

std::string serialize_time_point(const std::chrono::system_clock::time_point &time,
                                 const std::string &format = "%Y-%m-%d %H:%M:%S")
{
  std::time_t tt = std::chrono::system_clock::to_time_t(time);
  std::tm tm = *std::gmtime(&tt);
  std::stringstream ss;
  ss << std::put_time(&tm, format.c_str());
  return ss.str();
}

int main(int argc, char const *argv[])
{
  const auto arch = cspec::cpu::architecture();
  const auto arch_map =
    std::map<cspec::cpu::architecture_t, std::string>({{cspec::cpu::architecture_t::x64, "x64"},
                                                       {cspec::cpu::architecture_t::arm, "arm"},
                                                       {cspec::cpu::architecture_t::itanium, "itanium"},
                                                       {cspec::cpu::architecture_t::x86, "x86"},
                                                       {cspec::cpu::architecture_t::unknown, "unknown"}});

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

  const auto kernel = cspec::system::kernel();
  const auto kernel_map =
    std::map<cspec::system::kernel_t, std::string>({{cspec::system::kernel_t::nt, "nt"},
                                                    {cspec::system::kernel_t::linux, "linux"},
                                                    {cspec::system::kernel_t::darwin, "darwin"},
                                                    {cspec::system::kernel_t::unknown, "unknown"}});
  std::cout << "Kernel Type: " << kernel_map.at(kernel.type) << '\n'
            << "Kernel Major: " << kernel.major << '\n'
            << "Kernel Minor: " << kernel.minor << '\n'
            << "Kernel Patch: " << kernel.patch << '\n'
            << "Kernel Build: " << kernel.build << '\n'
            << '\n';

  const auto os = cspec::system::os();
  std::cout << "OS Name: " << os.name << " (" << os.full_name << ')' << '\n'
            << "OS Major: " << os.major << '\n'
            << "OS Minor: " << os.minor << '\n'
            << "OS Patch: " << os.patch << '\n'
            << "OS Build: " << os.build << '\n'
            << '\n';

  const auto times = cspec::system::times();
  std::cout << "Current Time: " << serialize_time_point(times.current) << '\n'
            << "Boot Time: " << serialize_time_point(times.boot) << '\n'
            << '\n';

  return 0;
}
