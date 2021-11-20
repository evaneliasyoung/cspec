/**
 *  @file      ns.h
 *  @brief     Namspace extensions for cspec GPU.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-20
 *  @date      2021-11-20
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#pragma once

#include "../pch.h"

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
      std::string name;
      umax memory;
    };

    std::vector<gpu_info_t> devices();
    vendor_t string_to_vendor(const std::string &vendor);
    std::string vendor_to_string(const vendor_t &vendor);
  } // namespace gpu
} // namespace cspec
