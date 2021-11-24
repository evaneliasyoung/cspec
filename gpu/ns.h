/**
 *  @file      ns.h
 *  @brief     Namspace extensions for cspec GPU.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-20
 *  @date      2021-11-23
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#pragma once

#include "../core.h"

#include <vector>

namespace cspec
{
  namespace gpu
  {
    enum class vendor_t
    {
      nvidia,
      amd,
      intel,
      microsoft,
      qualcomm,
      vmware,
      unknown
    };

    struct gpu_info_t
    {
      vendor_t vendor;
      string name;
      umax memory;
    };

    vector<gpu_info_t> devices();
    vendor_t string_to_vendor(const string &vendor);
    string vendor_to_string(const vendor_t &vendor);
  } // namespace gpu
} // namespace cspec
