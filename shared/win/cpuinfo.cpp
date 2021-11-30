/**
 *  @file      cpuinfo.cpp
 *  @brief     Windows equivalent of /proc/cpuinfo.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-29
 *  @date      2021-11-29
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"

#ifdef WIN
vector<SYSTEM_LOGICAL_PROCESSOR_INFORMATION> cspec::shared::cpuinfo_buffer()
{
  vector<SYSTEM_LOGICAL_PROCESSOR_INFORMATION> ret;

  DWORD bytes = 0;
  GetLogicalProcessorInformation(nullptr, &bytes);
  ret.resize(bytes / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION));
  GetLogicalProcessorInformation(ret.data(), &bytes);

  return ret;
}
#endif
