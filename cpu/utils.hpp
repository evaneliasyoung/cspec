/**
 *  @file      utils.hpp
 *  @brief     Utilities for cpu info.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-16
 *  @date      2021-11-20
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#pragma once

#if defined(WIN)
#include <vector>

static std::vector<SYSTEM_LOGICAL_PROCESSOR_INFORMATION> cpuinfo_buffer()
{
  std::vector<SYSTEM_LOGICAL_PROCESSOR_INFORMATION> ret;

  DWORD bytes = 0;
  GetLogicalProcessorInformation(nullptr, &bytes);
  ret.resize(bytes / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION));
  GetLogicalProcessorInformation(ret.data(), &bytes);

  return ret;
}
#elif defined(MAC)
#else
#endif
