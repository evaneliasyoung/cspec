/**
 *  @file      version.cpp
 *  @brief     Functions for the version structure.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-24
 *  @date      2021-11-24
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"

#include <iostream>
#include <sstream>

string cspec::shared::vertos(const cspec::shared::version_t &ver)
{
  std::stringstream ss;

  ss << ver.major << '.' << ver.minor << '.' << ver.patch << "-build." << ver.build;

  return ss.str();
}

cspec::shared::version_t cspec::shared::stover(const string &ver)
{
  std::smatch match;
  if (!std::regex_search(ver, match, R"(^([0-9]+)\.([0-9]+)\.([0-9]+)(?:[+-])(?:build\.([0-9]+)))"_regex))
    return {};

  auto iter = 0;
  const auto major = std::stoul(match[++iter]);
  const auto minor = std::stoul(match[++iter]);
  const auto patch = std::stoul(match[++iter]);
  const auto build = std::stoul(match[++iter]);

  return {major, minor, patch, build};
}

void cspec::shared::to_json(json &j, const cspec::shared::version_t &ver)
{
  j = json{{"major", ver.major}, {"minor", ver.minor}, {"build", ver.build}, {"patch", ver.patch}};
}

void cspec::shared::from_json(const json &j, cspec::shared::version_t &ver)
{
  j.at("major").get_to(ver.major);
  j.at("minor").get_to(ver.minor);
  j.at("build").get_to(ver.build);
  j.at("patch").get_to(ver.patch);
}
