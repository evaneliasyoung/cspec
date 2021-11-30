/**
 *  @file      json.cpp
 *  @brief     JSON casting and decasting for filesystem.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-29
 *  @date      2021-11-29
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"

void cspec::filesystem::to_json(json &j, const cspec::filesystem::sizes_t &sizes)
{
  j = json{{"available", sizes.available}, {"total", sizes.total}, {"used", sizes.used}};
}

void cspec::filesystem::from_json(const json &j, cspec::filesystem::sizes_t &sizes)
{
  j.at("available").get_to(sizes.available);
  j.at("total").get_to(sizes.total);
  j.at("used").get_to(sizes.used);
}

void cspec::filesystem::to_json(json &j, const cspec::filesystem::filesystem_t &fs)
{
  j = json{{"name", fs.name}, {"type", cspec::filesystem::fstos(fs.type)}, {"sizes", fs.sizes}, {"mount", fs.mount}};
}

void cspec::filesystem::from_json(const json &j, cspec::filesystem::filesystem_t &fs)
{
  j.at("name").get_to(fs.name);
  j.at("sizes").get_to(fs.sizes);
  j.at("mount").get_to(fs.mount);
  fs.type = cspec::filesystem::stofs(j.at("type"));
}
