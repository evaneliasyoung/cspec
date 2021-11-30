/**
 *  @file      json.cpp
 *  @brief     JSON casting and decasting for memory.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-29
 *  @date      2021-11-29
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"

void cspec::memory::to_json(json &j, const cspec::memory::voltages_t &vlt)
{
  j = json{{"configured", vlt.configured}, {"max", vlt.max}, {"min", vlt.min}};
}

void cspec::memory::from_json(const json &j, cspec::memory::voltages_t &vlt)
{
  j.at("configured").get_to(vlt.configured);
  j.at("max").get_to(vlt.max);
  j.at("min").get_to(vlt.min);
}

void cspec::memory::to_json(json &j, const cspec::memory::memory_t &mem)
{
  j = json{{"voltage", mem.voltage},
           {"form_factor", mem.form_factor},
           {"size", mem.size},
           {"speed", mem.speed},
           {"manufacturer", mem.manufacturer},
           {"model", mem.model},
           {"serial", mem.serial},
           {"bank", mem.bank}};
}

void cspec::memory::from_json(const json &j, cspec::memory::memory_t &mem)
{
  j.at("voltage").get_to(mem.voltage);
  j.at("form_factor").get_to(mem.form_factor);
  j.at("size").get_to(mem.size);
  j.at("speed").get_to(mem.speed);
  j.at("manufacturer").get_to(mem.manufacturer);
  j.at("model").get_to(mem.model);
  j.at("serial").get_to(mem.serial);
  j.at("bank").get_to(mem.bank);
}
