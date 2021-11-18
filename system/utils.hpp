/**
 *  @file      utils.hpp
 *  @brief     Utilities for os info.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-17
 *  @date      2021-11-18
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#pragma once

#include "../pch.h"

#if defined(WIN)
struct scope_wrapper
{
  std::function<void()> func;

  scope_wrapper(const scope_wrapper &) = delete;
  ~scope_wrapper()
  {
    if (func)
      func();
  }
};

struct release_deleter
{
  template<typename T> void operator()(T *ptr) const
  {
    ptr->Release();
  }
};

static std::string transcode_from_wide(const wchar_t *wstr, std::size_t wstr_size)
{
  std::string ret;
  // convert even embedded NUL
  if (const auto len = WideCharToMultiByte(CP_UTF8, 0, wstr, static_cast<int>(wstr_size), nullptr, 0, 0, 0))
  {
    ret.resize(len, '\0');
    if (!WideCharToMultiByte(CP_UTF8, 0, wstr, static_cast<int>(wstr_size), &ret[0], len, 0, 0))
      ret.clear();
  }
  return ret;
}

std::string narrowen_winstring(const wchar_t *wstr)
{
  if (!wstr)
    return {};
  return transcode_from_wide(wstr, std::wcslen(wstr));
}

std::string narrowen_bstring(const wchar_t *bstr)
{
  if (!bstr)
    return {};
  return transcode_from_wide(bstr, SysStringLen(const_cast<BSTR>(bstr)));
}

static std::string version_name()
{
  // Initialize COM.
  auto err = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
  if (FAILED(err) && err != RPC_E_CHANGED_MODE)
    return {};
  scope_wrapper com_uninitialiser{CoUninitialize};

  // Set COM security.
  const auto init_result = CoInitializeSecurity(nullptr, -1, nullptr, nullptr, RPC_C_AUTHN_LEVEL_DEFAULT,
                                                RPC_C_IMP_LEVEL_IMPERSONATE, nullptr, EOAC_NONE, nullptr);
  if (FAILED(init_result) && init_result != RPC_E_TOO_LATE)
    return {};

  // Get pointer to WMI locater.
  IWbemLocator *wbem_loc_ptr;
  if (FAILED(CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator,
                              reinterpret_cast<void **>(&wbem_loc_ptr))))
    return {};
  std::unique_ptr<IWbemLocator, release_deleter> wbem_loc(wbem_loc_ptr);

  // Connect to WMI.
  IWbemServices *wbem_services_ptr;
  wchar_t net_res[] = LR"(ROOT\CIMV2)";
  if (FAILED(wbem_loc->ConnectServer(net_res, nullptr, nullptr, 0, 0, 0, 0, &wbem_services_ptr)))
    return {};
  std::unique_ptr<IWbemServices, release_deleter> wbem_services(wbem_services_ptr);

  // Set WMI Proxy security.
  if (FAILED(CoSetProxyBlanket(wbem_services.get(), RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, nullptr,
                               RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, nullptr, EOAC_NONE)))
    return {};

  // Make WMI query.
  IEnumWbemClassObject *qry_itr_ptr;
  wchar_t query_lang[] = L"WQL";
  wchar_t query[] = L"SELECT Name FROM Win32_OperatingSystem";
  if (FAILED(wbem_services->ExecQuery(query_lang, query, WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, nullptr,
                                      &qry_itr_ptr)))
    return {};
  std::unique_ptr<IEnumWbemClassObject, release_deleter> qry_itr(qry_itr_ptr);

  // Get data from query.
  std::string ret;
  while (qry_itr)
  {
    IWbemClassObject *value_raw;
    unsigned long iter_result;

    qry_itr->Next(static_cast<LONG>(WBEM_INFINITE), 1, &value_raw, &iter_result);
    if (!iter_result)
      break;
    std::unique_ptr<IWbemClassObject, release_deleter> value(value_raw);

    VARIANT val;
    value->Get(L"Name", 0, &val, 0, 0);
    scope_wrapper val_destructor{[&]
                                 {
                                   VariantClear(&val);
                                 }};

    ret = narrowen_bstring(val.bstrVal);
  }
  return ret.substr(0, ret.find('|'));
}
#elif defined(MAC)
#else
static cspec::system::os_info_t lsb_release(std::ifstream &release)
{
  cspec::system::os_info_t ret{};

  for (std::string line; std::getline(release, line);)
  {
    if (line.find("DISTRIB_ID") == 0)
      ret.name = line.substr(line.find('=') + 1);
    else if (line.find("DISTRIB_RELEASE") == 0)
    {
      char *marker = &line[line.find('=') + 1];
      ret.major = std::strtoul(marker, &marker, 10);
      ret.minor = std::strtoul(marker + 1, &marker, 10);
      ret.patch = std::strtoul(marker + 1, &marker, 10);
      ret.build = std::strtoul(marker + 1, nullptr, 10);
    }
    else if (line.find("DISTRIB_DESCRIPTION") == 0)
    {
      const auto beg_idx = line.find('"') + 1;
      const auto end_idx = line.size() - 1;
      ret.full_name = line.substr(beg_idx, end_idx - beg_idx);
    }
  }

  return ret;
}

static void trim_quotes(std::string &s)
{
  if (s.empty())
    return;
  if (s.back() == '"')
    s.pop_back();
  if (s.front() == '"')
    s.erase(s.begin());
}
#endif
