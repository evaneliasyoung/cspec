/**
 *  @file      main.cpp
 *  @brief     Main for cspec.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-11
 *  @date      2021-11-23
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "main.h"

string serialize_time_point(const std::chrono::system_clock::time_point &time,
                            const string &format = "%Y-%m-%d %H:%M:%S")
{
  std::time_t tt = std::chrono::system_clock::to_time_t(time);
  std::tm tm = *std::gmtime(&tt);
  std::stringstream ss;
  ss << std::put_time(&tm, format.c_str());
  return ss.str();
}

void print_header(const string &name)
{
  const umax pad_char_amt = 79 - (name.length() + 2);
  const bool even = pad_char_amt % 2 == 0;
  const umax l_amt = even ? pad_char_amt / 2 : (pad_char_amt + 1) / 2;
  const umax r_amt = even ? l_amt : l_amt - 1;

  std::cout << string(l_amt, '=') << "  " << name << "  " << string(r_amt, '=') << '\n';
}

void print_subheader(const string &name)
{
  std::cout << ">>> " << name << '\n';
}

void print_cpu()
{
  const auto arch = cspec::cpu::architecture();

  print_subheader("Meta");
  std::cout << "Arch: " << cspec::cpu::architecture_to_string(arch) << '\n'
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

  print_subheader("Group");
  const auto group = cspec::cpu::group();
  std::cout << "Family: " << group.family << '\n'
            << "Model: " << group.model << '\n'
            << "Stepping: " << group.stepping << '\n'
            << '\n';
}

void print_system()
{
  const auto kernel = cspec::system::kernel();

  print_subheader("Kernel");
  std::cout << "Kernel Type: " << cspec::system::kernel_to_string(kernel.type) << '\n'
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
            << "Install Time: " << serialize_time_point(times.install) << '\n'
            << "Boot Time: " << serialize_time_point(times.boot) << '\n'
            << '\n';
}

void print_gpu()
{
  const auto gpus = cspec::gpu::devices();
  for (const auto gpu: gpus)
  {
    std::cout << "Vendor: " << cspec::gpu::vendor_to_string(gpu.vendor) << '\n'
              << "Name: " << gpu.name << '\n'
              << "Memory: " << gpu.memory << '\n'
              << '\n';
  }
}

void print_memory()
{
  const auto chips = cspec::memory::devices();
  for (const auto chip: chips)
  {
    print_subheader("New Memory");
    std::cout << "Min Voltage: " << chip.voltage.min << '\n'
              << "Max Voltage: " << chip.voltage.max << '\n'
              << "Configured Voltage: " << chip.voltage.configured << '\n'
              << "Size: " << chip.size << '\n'
              << "Speed: " << chip.speed << '\n'
              << "Manufacturer: " << chip.manufacturer << '\n'
              << "Form Factor: " << chip.form_factor << '\n'
              << "Model: " << chip.model << '\n'
              << "Serial: " << chip.serial << '\n'
              << "Bank: " << chip.bank << '\n'
              << '\n';
  }
}

void print_filesystem()
{
  const auto fss = cspec::filesystem::systems();
  for (const auto fs: fss)
  {
    print_subheader("New System");
    std::cout << "Name: " << fs.name << '\n'
              << "Type: " << cspec::filesystem::filesystem_type_to_string(fs.type) << '\n'
              << "Mount: " << fs.mount << '\n'
              << "Size: " << fs.size << '\n'
              << "Used: " << fs.used << '\n'
              << "Avaiable: " << fs.available << '\n'
              << '\n';
  }
}

int main(int argc, char const *argv[])
{
  print_header("CPU");
  print_cpu();

  print_header("System");
  print_system();

  print_header("GPU");
  print_gpu();

  print_header("Memory");
  print_memory();

  print_header("Filesystem");
  print_filesystem();

  return 0;
}
