/**
 *  @file      strconv.cpp
 *  @brief     Handles gpu related string conversions.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-29
 *  @date      2021-11-29
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "../shared/ns.h"
#include "ns.h"

cspec::gpu::vendor_t cspec::gpu::stovnd(const string &vendor)
{
  if (cspec::shared::icaseis(vendor, "NVIDIA"))
    return cspec::gpu::vendor_t::nvidia;
  else if (cspec::shared::icaseis(vendor, "AMD") || cspec::shared::icaseis(vendor, "ATi")
           || cspec::shared::icaseis(vendor, "Advanced Micro Devices"))
    return cspec::gpu::vendor_t::amd;
  else if (cspec::shared::icaseis(vendor, "Intel"))
    return cspec::gpu::vendor_t::intel;
  else if (cspec::shared::icaseis(vendor, "Microsoft"))
    return cspec::gpu::vendor_t::microsoft;
  else if (cspec::shared::icaseis(vendor, "Qualcomm"))
    return cspec::gpu::vendor_t::qualcomm;
  else if (cspec::shared::icaseis(vendor, "VMware"))
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
