/**
 *  @file      devices.cpp
 *  @brief     Determines memory information.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-23
 *  @date      2021-11-29
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "../shared/ns.h"
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
vector<cspec::memory::memory_t> cspec::memory::devices()
{
  vector<cspec::memory::memory_t> ret{};

  cspec::shared::WMI wmi;
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
    memory.size = !chip.at("Capacity").empty() ? cspec::shared::depow2(std::stoull(chip.at("Capacity"))) : 0;
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
vector<cspec::memory::memory_t> cspec::memory::devices()
{
  vector<cspec::memory::memory_t> ret{};
  std::map<string, string> manuf = {{"0x014F", "Transcend Information"},
                                    {"0x2C00", "Micron"},
                                    {"0x802C", "Micron"},
                                    {"0x80AD", "Hynix"},
                                    {"0xAD00", "Hynix"},
                                    {"0x80CE", "Samsung"},
                                    {"0xCE00", "Samsung"},
                                    {"0x02FE", "Elpida"},
                                    {"0x5105", "Qimonda AG"},
                                    {"0x8551", "Qimonda AG"},
                                    {"0x859B", "Crucial"}};

  std::stringstream ss(cspec::shared::exec("system_profiler SPMemoryDataType"));

  for (string line; std::getline(ss, line, '\n');)
  {
    if (line.find("BANK ") != string::npos)
    {
      cspec::memory::memory_t chip;
      chip.bank = line.substr(line.find_first_not_of(' '));
      chip.bank.erase(chip.bank.length() - 1);
      u8 flag = 0;
      for (string chip_line; std::getline(ss, chip_line, '\n');)
      {
        if (chip_line.empty())
          if (++flag == 2)
            break;
          else
            continue;

        std::smatch mt;
        if (std::regex_search(chip_line, mt, R"(Size: (\d+) ([GMK]))"_regex))
        {
          chip.size = std::stoul(mt[1]);
          switch (mt[2].str()[0])
          {
            case 'G':
              chip.size *= 1000;
              [[fallthrough]];
            case 'M':
              chip.size *= 1000;
              [[fallthrough]];
            case 'K':
              chip.size *= 1000;
          }
        }
        else if (std::regex_search(chip_line, mt, R"(Speed: (\d+) ([GMK]))"_regex))
        {
          chip.speed = std::stoul(mt[1]);
          switch (mt[2].str()[0])
          {
            case 'G':
              chip.speed *= 1000;
              [[fallthrough]];
            case 'M':
              chip.speed *= 1000;
              [[fallthrough]];
            case 'K':
              chip.speed *= 1000;
          }
        }
        else if (chip_line.find("Manuf") != string::npos)
          if (manuf.find(chip_line.substr(chip_line.find(':') + 2)) != manuf.end())
            chip.manufacturer = manuf.at(chip_line.substr(chip_line.find(':') + 2));
          else
            chip.manufacturer = "Unknown";
        else if (chip_line.find("Serial") != string::npos)
          chip.serial = chip_line.substr(chip_line.find(':') + 2);
        else if (chip_line.find("Part Number") != string::npos)
          chip.model = chip_line.substr(chip_line.find(':') + 2);
      }
      chip.form_factor = "unknown";
      ret.push_back(chip);
    }
  }

  return ret;
}
#else
#endif
