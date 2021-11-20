/**
 *  @file      utils.h
 *  @brief     Project utilities.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-19
 *  @date      2021-11-19
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#pragma once

#include "pch.h"

#if defined(WIN)
#include "utils/win/registry.hpp"
#include "utils/win/wstring.hpp"
#elif defined(MAC)
#else
#endif
