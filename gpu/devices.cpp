/**
 *  @file      devices.cpp
 *  @brief     Determines gpu information.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-20
 *  @date      2021-11-20
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"

cspec::gpu::vendor_t cspec::gpu::string_to_vendor(const string &vendor)
{
  if (strcasecmp(vendor.c_str(), "NVIDIA") == 0)
    return cspec::gpu::vendor_t::nvidia;
  else if (strcasecmp(vendor.c_str(), "AMD") == 0 || strcasecmp(vendor.c_str(), "ATi") == 0
           || strcasecmp(vendor.c_str(), "Advanced Micro Devices") == 0)
    return cspec::gpu::vendor_t::amd;
  else if (strcasecmp(vendor.c_str(), "Intel") == 0)
    return cspec::gpu::vendor_t::intel;
  else if (strcasecmp(vendor.c_str(), "Microsoft") == 0)
    return cspec::gpu::vendor_t::microsoft;
  else if (strcasecmp(vendor.c_str(), "Qualcomm") == 0)
    return cspec::gpu::vendor_t::qualcomm;
  else if (strcasecmp(vendor.c_str(), "VMware") == 0)
    return cspec::gpu::vendor_t::vmware;
  else
    return cspec::gpu::vendor_t::unknown;
}

string cspec::gpu::vendor_to_string(const cspec::gpu::vendor_t &vendor)
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

#if defined(WIN)
std::vector<cspec::gpu::gpu_info_t> cspec::gpu::devices()
{
  std::vector<cspec::gpu::gpu_info_t> ret{};
  const auto gpu_path = R"(SYSTEM\ControlSet001\Control\Class\{4d36e968-e325-11ce-bfc1-08002be10318}\0000)";

  cspec::gpu::vendor_t vendor =
    cspec::gpu::string_to_vendor(read_registry_sz<13>(HKEY_LOCAL_MACHINE, gpu_path, "ProviderName"));
  const string name = read_registry_sz<80>(HKEY_LOCAL_MACHINE, gpu_path, "HardwareInformation.AdapterString");
  const auto memory = read_registry_qw(HKEY_LOCAL_MACHINE, gpu_path, "HardwareInformation.qwMemorySize");

  ret.push_back({vendor, name, memory});

  return ret;
}
#elif defined(MAC)
#else
#include "../utils/shell.hpp"

#include <regex>
#include <sstream>

std::vector<cspec::gpu::gpu_info_t> cspec::gpu::devices()
{
  std::vector<cspec::gpu::gpu_info_t> ret{};

  auto lspci_out = std::stringstream(exec("lspci -vnn"));
  for (string line; std::getline(lspci_out, line, '\n');)
  {
    if (line.find(" VGA ") != string::npos)
    {
      gpu_info_t info{};
      for (string vga_line; std::getline(lspci_out, vga_line, '\n') && vga_line.size() > 0;)
      {
        std::smatch match;
        if (std::regex_search(vga_line, match, std::regex(R"(Subsystem: ((\w+).+(?=\s\[)))")))
        {
          info.name = match[1];
          info.vendor = cspec::gpu::string_to_vendor(match[2]);
        }
      }
      ret.push_back(info);
    }
  }

  return ret;
}
#endif
