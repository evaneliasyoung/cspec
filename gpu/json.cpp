/**
 *  @file      json.cpp
 *  @brief     JSON casting and decasting for gpu.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-29
 *  @date      2021-11-29
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"

void cspec::gpu::to_json(json &j, const cspec::gpu::gpu_info_t &gpu)
{
  j = json{{"memory", gpu.memory}, {"bus", gpu.bus}, {"name", gpu.name}, {"vendor", cspec::gpu::vndtos(gpu.vendor)}};
}

void cspec::gpu::from_json(const json &j, cspec::gpu::gpu_info_t &gpu)
{
  j.at("memory").get_to(gpu.memory);
  j.at("name").get_to(gpu.name);
  j.at("bus").get_to(gpu.bus);
  gpu.vendor = cspec::gpu::stovnd(j.at("vendor"));
}
