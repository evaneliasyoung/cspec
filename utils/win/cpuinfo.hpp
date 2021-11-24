/**
 *  @file      cpuinfo.hpp
 *  @brief     Windows equivalent of /proc/cpuinfo.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-20
 *  @date      2021-11-23
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "../../core.h"

#ifdef WIN
#include <vector>
#include <windows.h>

static vector<SYSTEM_LOGICAL_PROCESSOR_INFORMATION> cpuinfo_buffer()
{
  vector<SYSTEM_LOGICAL_PROCESSOR_INFORMATION> ret;

  DWORD bytes = 0;
  GetLogicalProcessorInformation(nullptr, &bytes);
  ret.resize(bytes / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION));
  GetLogicalProcessorInformation(ret.data(), &bytes);

  return ret;
}
#endif
