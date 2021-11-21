/**
 *  @file      shell.hpp
 *  @brief     Shell command utilities.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-20
 *  @date      2021-11-20
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "../core.h"

#include <fstream>
#include <memory>

static string exec(const char *cmd)
{
  array<char, 128> buf;
  string ret;
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);

  if (!pipe)
    throw std::runtime_error("popen() failed!");
  while (fgets(buf.data(), buf.size(), pipe.get()) != nullptr)
    ret += buf.data();

  return ret;
}
