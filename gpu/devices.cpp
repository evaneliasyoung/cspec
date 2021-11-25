/**
 *  @file      devices.cpp
 *  @brief     Determines gpu information.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-20
 *  @date      2021-11-25
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "../utils/strcmp.hpp"
#include "ns.h"

cspec::gpu::vendor_t cspec::gpu::stovnd(const string &vendor)
{
  if (icaseis(vendor, "NVIDIA"))
    return cspec::gpu::vendor_t::nvidia;
  else if (icaseis(vendor, "AMD") || icaseis(vendor, "ATi") || icaseis(vendor, "Advanced Micro Devices"))
    return cspec::gpu::vendor_t::amd;
  else if (icaseis(vendor, "Intel"))
    return cspec::gpu::vendor_t::intel;
  else if (icaseis(vendor, "Microsoft"))
    return cspec::gpu::vendor_t::microsoft;
  else if (icaseis(vendor, "Qualcomm"))
    return cspec::gpu::vendor_t::qualcomm;
  else if (icaseis(vendor, "VMware"))
    return cspec::gpu::vendor_t::vmware;
  else
    return cspec::gpu::vendor_t::unknown;
}

string cspec::gpu::vndtos(const cspec::gpu::vendor_t &vendor)
{
  switch (vendor)
  {
    case cspec::gpu::vendor_t::nvidia:
      return "NVIDIA";
    case cspec::gpu::vendor_t::amd:
      return "AMD";
    case cspec::gpu::vendor_t::intel:
      return "Intel";
    case cspec::gpu::vendor_t::microsoft:
      return "Microsoft";
    case cspec::gpu::vendor_t::qualcomm:
      return "Qualcomm";
    case cspec::gpu::vendor_t::vmware:
      return "VMware";
    default:
      return "Unknown";
  }
}

void cspec::gpu::to_json(json &j, const cspec::gpu::gpu_info_t &gpu)
{
  j = json{{"memory", gpu.memory}, {"name", gpu.name}, {"vendor", cspec::gpu::vndtos(gpu.vendor)}};
}

void cspec::gpu::from_json(const json &j, cspec::gpu::gpu_info_t &gpu)
{
  j.at("memory").get_to(gpu.memory);
  j.at("name").get_to(gpu.name);
  gpu.vendor = cspec::gpu::stovnd(j.at("vendor"));
}

#if defined(WIN)
#include "../utils/win/registry.hpp"

vector<cspec::gpu::gpu_info_t> cspec::gpu::devices()
{
  vector<cspec::gpu::gpu_info_t> ret{};
  const auto gpu_path = R"(SYSTEM\ControlSet001\Control\Class\{4d36e968-e325-11ce-bfc1-08002be10318}\0000)";

  cspec::gpu::vendor_t vendor = cspec::gpu::stovnd(read_registry_sz<13>(HKEY_LOCAL_MACHINE, gpu_path, "ProviderName"));
  const string name = read_registry_sz<80>(HKEY_LOCAL_MACHINE, gpu_path, "HardwareInformation.AdapterString");
  const auto memory = read_registry_qw(HKEY_LOCAL_MACHINE, gpu_path, "HardwareInformation.qwMemorySize");

  ret.push_back({vendor, name, memory});

  return ret;
}
#elif defined(MAC)
#else
#include "../utils/shell.hpp"

#include <iostream>
#include <sstream>

vector<cspec::gpu::gpu_info_t> cspec::gpu::devices()
{
  vector<cspec::gpu::gpu_info_t> ret{};

  auto lspci_out = std::stringstream(exec("lspci -vnn"));
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
        }
      }
      ret.push_back(info);
    }
  }

  return ret;
}
#endif
