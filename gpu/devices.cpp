/**
 *  @file      devices.cpp
 *  @brief     Determines gpu information.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-20
 *  @date      2021-11-29
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "../shared/ns.h"
#include "ns.h"

#if defined(WIN)
vector<cspec::gpu::gpu_info_t> cspec::gpu::devices()
{
  vector<cspec::gpu::gpu_info_t> ret{};
  const auto gpu_path = R"(SYSTEM\ControlSet001\Control\Class\{4d36e968-e325-11ce-bfc1-08002be10318}\0000)";

  cspec::gpu::vendor_t vendor =
    cspec::gpu::stovnd(cspec::shared::read_registry_sz<13>(HKEY_LOCAL_MACHINE, gpu_path, "ProviderName"));
  const string name =
    cspec::shared::read_registry_sz<80>(HKEY_LOCAL_MACHINE, gpu_path, "HardwareInformation.AdapterString");
  const auto memory = cspec::shared::depow2(
    cspec::shared::read_registry_qw(HKEY_LOCAL_MACHINE, gpu_path, "HardwareInformation.qwMemorySize"));
  bool dynamic = false;
  string bus = "";

  cspec::shared::WMI wmi(true);
  if (!wmi.failed)
  {
    const auto datamap = wmi.query_and_retrieve<string>("Win32_VideoController", {"VideoMemoryType", "PNPDeviceID"});
    dynamic = datamap.at("VideoMemoryType") == "2";
    if (datamap.at("PNPDeviceID").substr(0, 3) == "PCI")
      bus = "PCIe";
  }

  ret.push_back({vendor, name, memory, bus, dynamic});

  return ret;
}
#elif defined(MAC)
vector<cspec::gpu::gpu_info_t> cspec::gpu::devices()
{
  vector<cspec::gpu::gpu_info_t> ret{};
  std::stringstream ss(cspec::shared::exec("system_profiler SPDisplaysDataType | grep -A5 'Chipset Model:'"));

  cspec::gpu::gpu_info_t gpu;
  for (string line; std::getline(ss, line, '\n');)
  {
    std::smatch mt;
    if (line == "--")
      ret.push_back(gpu);
    else if (line.find("Chipset Model") != string::npos)
      gpu.name = line.substr(line.find(':') + 2);
    else if (std::regex_search(line, mt, R"(Vendor: (.*) \()"_regex))
      gpu.vendor = cspec::gpu::stovnd(mt[1].str());
    else if (std::regex_search(line, mt, R"(VRAM.*: (\d+) ([GMK]))"_regex))
    {
      gpu.dynamic = line.find("Dynamic") != string::npos;
      gpu.memory = std::stoull(mt[1]);
      switch (mt[2].str()[0])
      {
        case 'G':
          gpu.memory *= 1000;
          [[fallthrough]];
        case 'M':
          gpu.memory *= 1000;
          [[fallthrough]];
        case 'K':
          gpu.memory *= 1000;
      }
    }
    else if (line.find("Bus") != string::npos)
      gpu.bus = line.substr(line.find(':') + 2);
  }
  ret.push_back(gpu);

  return ret;
}
#else
#include <iostream>
#include <sstream>

vector<cspec::gpu::gpu_info_t> cspec::gpu::devices()
{
  vector<cspec::gpu::gpu_info_t> ret{};

  auto lspci_out = std::stringstream(cspec::shared::exec("lspci -vnn"));
  for (string line; std::getline(lspci_out, line, '\n');)
  {
    if (line.find(" VGA ") != string::npos)
    {
      gpu_info_t info{};
      for (string vga_line; std::getline(lspci_out, vga_line, '\n') && vga_line.size() > 0;)
      {
        std::smatch match;
        if (std::regex_search(vga_line, match, R"(Subsystem: ((\w+).+(?=\s\[)))"_regex))
        {
          info.name = match[1];
          info.vendor = cspec::gpu::stovnd(match[2]);
        }
        else if (std::regex_search(vga_line, match, R"(Mem.* pre.*size=(\d+)([GMK]))"_regex))
        {
          info.memory = std::stoul(match[1]);
          switch (string(match[2])[0])
          {
            case 'G':
              info.memory *= 1024;
              [[fallthrough]];
            case 'M':
              info.memory *= 1024;
              [[fallthrough]];
            case 'K':
              info.memory *= 1024;
          }
          info.memory = cspec::shared::depow2(info.memory);
        }
      }
      ret.push_back(info);
    }
  }

  return ret;
}
#endif
