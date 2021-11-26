/**
 *  @file      human.cpp
 *  @brief     Human formatting for cspec cli.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-25
 *  @date      2021-11-25
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"

string cspec::cli::human_format(const umax &n, const cspec::cli::si_unit_t &from, const cspec::cli::si_unit_t &to,
                                const string &suf, const umax &prec)
{
  return cspec::cli::human_format((double)n, from, to, suf, prec);
}

string cspec::cli::human_format(const umax &n, const cspec::cli::si_unit_t &to, const string &suf, const umax &prec)
{
  return cspec::cli::human_format(n, cspec::cli::si_unit_t::base, to, suf, prec);
}

string cspec::cli::human_format(const umax &n, const string &suf, const umax &prec)
{
  return cspec::cli::human_format(n, (cspec::cli::si_unit_t)(umax)std::floor(std::log(n) / std::log(1000)), suf, prec);
}

string cspec::cli::human_format(const double &n, const cspec::cli::si_unit_t &from, const cspec::cli::si_unit_t &to,
                                const string &suf, const umax &prec)
{
  std::stringstream ss;
  auto units = array{"", "K", "M", "G", "T"};
  const auto diff = from > to ? (umax)from - (umax)to : (umax)to - umax(from);
  const auto val = from > to ? n * pow(1000, diff) : n / pow(1000, diff);

  ss << std::fixed << std::setprecision(val == std::round(val) ? 0 : prec) << val << ' ' << units[(umax)to] << suf;
  return ss.str();
}

string cspec::cli::human_format(const double &n, const cspec::cli::si_unit_t &to, const string &suf, const umax &prec)
{
  return cspec::cli::human_format(n, cspec::cli::si_unit_t::base, to, suf, prec);
}

string cspec::cli::human_format(const double &n, const string &suf, const umax &prec)
{
  return cspec::cli::human_format(n, (cspec::cli::si_unit_t)(umax)std::floor(std::log(n) / std::log(1000)), suf, prec);
}

json cspec::cli::human_friendly_json(json &j)
{
  if (j.contains("cpu"))
    if (j["cpu"].contains("clock"))
      j["cpu"]["clock"] = cspec::cli::human_format((umax)j["cpu"]["clock"], cspec::cli::si_unit_t::giga, "Hz");
  if (j["cpu"].contains("cache"))
    for (auto &cch: j["cpu"]["cache"])
      cch["size"] = cspec::cli::human_format((umax)cch["size"], "B");
  if (j.contains("filesystem"))
    for (auto &fs: j["filesystem"])
      if (fs.contains("sizes"))
        for (auto &sz: fs["sizes"])
          sz = cspec::cli::human_format(sz.get<umax>(), "B", 4);
  if (j.contains("gpu"))
    for (auto &gpu: j["gpu"])
      if (gpu.contains("memory"))
        gpu["memory"] = cspec::cli::human_format((umax)gpu["memory"], "B");
  if (j.contains("memory"))
    for (auto &mem: j["memory"])
    {
      if (mem.contains("speed"))
        mem["speed"] = cspec::cli::human_format((umax)mem["speed"], cspec::cli::si_unit_t::mega, "Hz");
      if (mem.contains("size"))
        mem["size"] = cspec::cli::human_format((umax)mem["size"], "B", 4);
      if (mem.contains("voltage"))
        for (auto &vlt: mem["voltage"])
          vlt =
            cspec::cli::human_format(vlt.get<double>(), cspec::cli::si_unit_t::base, cspec::cli::si_unit_t::base, "V");
    }
  return j;
}
