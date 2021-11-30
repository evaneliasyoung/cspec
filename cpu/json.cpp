/**
 *  @file      json.cpp
 *  @brief     JSON casting and decasting for cpu.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-29
 *  @date      2021-11-29
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"

void cspec::cpu::to_json(json &j, const cspec::cpu::amounts_t &amt)
{
  j = json{{"threads", amt.threads}, {"cores", amt.cores}, {"packages", amt.packages}};
}

void cspec::cpu::from_json(const json &j, cspec::cpu::amounts_t &amt)
{
  j.at("threads").get_to(amt.threads);
  j.at("cores").get_to(amt.cores);
  j.at("packages").get_to(amt.packages);
}

void cspec::cpu::to_json(json &j, const cspec::cpu::cache_t &cch)
{
  j = json{{"association", cch.association},
           {"line_size", cch.line_size},
           {"size", cch.size},
           {"type", cspec::cpu::cchtos(cch.type)}};
}

void cspec::cpu::from_json(const json &j, cspec::cpu::cache_t &cch)
{
  j.at("association").get_to(cch.association);
  j.at("line_size").get_to(cch.line_size);
  j.at("size").get_to(cch.size);
  cch.type = cspec::cpu::stocch(j.at("type"));
}

void cspec::cpu::to_json(json &j, const cspec::cpu::group_t &grp)
{
  j = json{{"family", grp.family}, {"model", grp.model}, {"stepping", grp.stepping}};
}

void cspec::cpu::from_json(const json &j, cspec::cpu::group_t &grp)
{
  j.at("family").get_to(grp.family);
  j.at("model").get_to(grp.model);
  j.at("stepping").get_to(grp.stepping);
}
