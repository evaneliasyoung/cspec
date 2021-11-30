/**
 *  @file      json.cpp
 *  @brief     JSON casting and decasting for system.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-29
 *  @date      2021-11-29
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "../shared/ns.h"
#include "ns.h"

void cspec::system::to_json(json &j, const cspec::system::kernel_info_t &krn)
{
  j = json{{"version", krn.version}, {"type", cspec::system::krntos(krn.type)}};
}

void cspec::system::from_json(const json &j, cspec::system::kernel_info_t &krn)
{
  j.at("version").get_to(krn.version);
  krn.type = cspec::system::stokrn(j.at("type"));
}

void cspec::system::to_json(json &j, const cspec::system::os_info_t &os)
{
  j = json{{"name", os.name}, {"full_name", os.full_name}, {"version", os.version}};
}

void cspec::system::from_json(const json &j, cspec::system::os_info_t &os)
{
  j.at("name").get_to(os.name);
  j.at("full_name").get_to(os.full_name);
  j.at("version").get_to(os.version);
}

void cspec::system::to_json(json &j, const cspec::system::times_t &tms)
{
  j = json{{"current", cspec::shared::tmtos(tms.current)},
           {"install", cspec::shared::tmtos(tms.install)},
           {"boot", cspec::shared::tmtos(tms.boot)}};
}

void cspec::system::from_json(const json &j, cspec::system::times_t &tms)
{
  tms.current = cspec::shared::stotm(j.at("current"));
  tms.install = cspec::shared::stotm(j.at("install"));
  tms.boot = cspec::shared::stotm(j.at("boot"));
}
