/**
 *  @file      issudo.cpp
 *  @brief     Determines if a user has sudo permissions.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-30
 *  @date      2021-11-30
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"

#ifndef WIN
#include <unistd.h>

bool cspec::shared::issudo()
{
  return getuid() == 0;
}

bool cspec::shared::hassudo()
{
  return geteuid() == 0;
}
#endif
