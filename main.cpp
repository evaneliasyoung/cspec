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

void print_header(const std::string &name)
{
  const umax pad_char_amt = 79 - (name.length() + 2);
  const bool even = pad_char_amt % 2 == 0;
  const umax l_amt = even ? pad_char_amt / 2 : (pad_char_amt + 1) / 2;
  const umax r_amt = even ? l_amt : l_amt - 1;

  std::cout << std::string(l_amt, '=') << "  " << name << "  " << std::string(r_amt, '=') << '\n';
}

void print_subheader(const std::string &name)
{
  std::cout << ">>> " << name << '\n';
}

void print_cpu()
{
  const auto arch = cspec::cpu::architecture();
  const auto arch_map =
    std::map<cspec::cpu::architecture_t, std::string>({{cspec::cpu::architecture_t::x64, "x64"},
                                                       {cspec::cpu::architecture_t::arm, "arm"},
                                                       {cspec::cpu::architecture_t::itanium, "itanium"},
                                                       {cspec::cpu::architecture_t::x86, "x86"},
                                                       {cspec::cpu::architecture_t::unknown, "unknown"}});

  print_subheader("Meta");
  std::cout << "Arch: " << arch_map.at(arch) << '\n'
            << "Endian: " << (cspec::cpu::endian() == cspec::cpu::endian_t::big ? "big" : "little") << '\n'
            << "Vendor: " << cspec::cpu::vendor() << '\n'
            << "Name: " << cspec::cpu::name() << '\n'
            << "Clock: " << cspec::cpu::clock() << '\n'
            << '\n';

  print_subheader("Amounts");
  const auto amounts = cspec::cpu::amounts();
  std::cout << "Cores: " << amounts.cores << '\n'
            << "Threads: " << amounts.threads << '\n'
            << "Packages: " << amounts.packages << '\n'
            << '\n';

  print_subheader("Cache");
  for (u8 i = 0; i < 4; ++i)
  {
    const auto cache = cspec::cpu::cache(i);
    print_subheader("L" + std::to_string(i) + " Cache");
    std::cout << "Size: " << cache.size << '\n'
              << "Line Size: " << cache.line_size << '\n'
              << "Association: " << (u16)cache.association << '\n'
              << '\n';
  }
}

void print_system()
{
  const auto kernel = cspec::system::kernel();
  const auto kernel_map =
    std::map<cspec::system::kernel_t, std::string>({{cspec::system::kernel_t::nt, "nt"},
                                                    {cspec::system::kernel_t::linux, "linux"},
                                                    {cspec::system::kernel_t::darwin, "darwin"},
                                                    {cspec::system::kernel_t::unknown, "unknown"}});
  print_subheader("Kernel");
  std::cout << "Kernel Type: " << kernel_map.at(kernel.type) << '\n'
            << "Kernel Major: " << kernel.major << '\n'
            << "Kernel Minor: " << kernel.minor << '\n'
            << "Kernel Patch: " << kernel.patch << '\n'
            << "Kernel Build: " << kernel.build << '\n'
            << '\n';

  print_subheader("OS");
  const auto os = cspec::system::os();
  std::cout << "OS Name: " << os.name << " (" << os.full_name << ')' << '\n'
            << "OS Major: " << os.major << '\n'
            << "OS Minor: " << os.minor << '\n'
            << "OS Patch: " << os.patch << '\n'
            << "OS Build: " << os.build << '\n'
            << '\n';

  print_subheader("Times");
  const auto times = cspec::system::times();
  std::cout << "Current Time: " << serialize_time_point(times.current) << '\n'
            << "Boot Time: " << serialize_time_point(times.boot) << '\n'
            << '\n';
}

int main(int argc, char const *argv[])
{
  print_header("CPU");
  print_cpu();

  print_header("System");
  print_system();

  return 0;
}
