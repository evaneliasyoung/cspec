/**
 *  @file      devices.cpp
 *  @brief     Determines memory information.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-23
 *  @date      2021-11-24
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

#if defined(WIN)
#include "../utils/win/wmi.hpp"

vector<cspec::memory::memory_t> cspec::memory::devices()
{
  vector<cspec::memory::memory_t> ret{};

  WMI wmi;
  if (!wmi.prepare())
    return ret;

  const auto chips = wmi.query_and_retrieve_multiple<string>(
    "Win32_PhysicalMemory",
    {"Capacity", "BankLabel", "MemoryType", "ConfiguredClockSpeed", "DeviceLocator", "Speed", "FormFactor",
     "Manufacturer", "PartNumber", "SerialNumber", "ConfiguredVoltage", "MinVoltage", "MaxVoltage"});

  const auto form_factors =
    array{"Unknown", "Other", "SIP", "DIP",  "ZIP", "SOJ",  "Proprietary", "SIMM", "DIMM", "TSOP", "PGA",   "RIMM",
          "SODIMM",  "SRIMM", "SMD", "SSMP", "QFP", "TQFP", "SOIC",        "LCC",  "PLCC", "BGA",  "FPBGA", "LGA"};

  for (const auto chip: chips)
  {
    cspec::memory::memory_t memory{};
    const auto raw_speed =
      !chip.at("ConfiguredClockSpeed").empty() ? chip.at("ConfiguredClockSpeed") : chip.at("Speed");

    memory.bank = chip.at("BankLabel").empty() ? chip.at("DeviceLocator") : chip.at("BankLabel");
    memory.serial = chip.at("SerialNumber");
    memory.model = chip.at("PartNumber");
    memory.manufacturer = !chip.at("Manufacturer").empty() ? chip.at("Manufacturer") : "Unknown";
    memory.size = !chip.at("Capacity").empty() ? std::stoull(chip.at("Capacity")) : 0;
    memory.speed = std::stoul(raw_speed) * 1000 * 1000;
    memory.voltage.configured =
      !chip.at("ConfiguredVoltage").empty() ? std::stod(chip.at("ConfiguredVoltage")) / 1000 : 0;
    memory.form_factor = form_factors[!chip.at("FormFactor").empty() ? std::stoi(chip.at("FormFactor")) : 0];
    memory.voltage.min = !chip.at("MinVoltage").empty() ? std::stod(chip.at("MinVoltage")) / 1000 : 0;
    memory.voltage.max = !chip.at("MaxVoltage").empty() ? std::stod(chip.at("MaxVoltage")) / 1000 : 0;

    ret.push_back(memory);
  }

  return ret;
}
#elif defined(MAC)
#else
#endif
