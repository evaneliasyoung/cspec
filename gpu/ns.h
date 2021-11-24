/**
 *  @file      ns.h
 *  @brief     Namspace extensions for cspec GPU.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-20
 *  @date      2021-11-24
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
    vendor_t stovnd(const string &vendor);
    string vndtos(const vendor_t &vendor);

    struct gpu_info_t
    {
      vendor_t vendor;
      string name;
      umax memory;
    };
    void to_json(json &j, const gpu_info_t &gpu);
    void from_json(const json &j, gpu_info_t &gpu);

    vector<gpu_info_t> devices();
  } // namespace gpu
} // namespace cspec
