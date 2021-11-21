/**
 *  @file      os.cpp
 *  @brief     Determines system os.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-17
 *  @date      2021-11-20
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"

#if defined(WIN)
#include "../utils/win/wstring.hpp"

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

static string version_name()
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
  string ret;
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

    ret = narrow_bstring(val.bstrVal);
  }
  return ret.substr(0, ret.find('|'));
}

cspec::system::os_info_t cspec::system::os()
{
  const auto kernel = cspec::system::kernel();
  return {"Windows NT", version_name(), kernel.major, kernel.minor, kernel.patch, kernel.build};
}
#else
#include <fstream>

static cspec::system::os_info_t lsb_release(std::ifstream &release)
{
  cspec::system::os_info_t ret{};

  for (string line; std::getline(release, line);)
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

static void trim_quotes(string &s)
{
  if (s.empty())
    return;
  if (s.back() == '"')
    s.pop_back();
  if (s.front() == '"')
    s.erase(s.begin());
}

cspec::system::os_info_t cspec::system::os()
{
  std::ifstream release("/etc/os-release");

  if (!release.is_open() || !release)
  {
    release.open("/usr/lib/os-release", std::ios::in);
    if (!release.is_open() || !release)
    {
      release.open("/etc/lsb-release", std::ios::in);
      if (!release.is_open() || !release)
        return {};
      else
        return lsb_release(release);
    }
  }

  cspec::system::os_info_t ret{};

  for (string line; std::getline(release, line);)
  {
    if (line.find("NAME") == 0)
      ret.name = line.substr(line.find('=') + 1);
    else if (line.find("PRETTY_NAME") == 0)
      ret.full_name = line.substr(line.find('=') + 1);
    else if (line.find("VERSION_ID") == 0)
    {
      char *marker = &line[line.find('=') + 1];
      if (marker[0] == '"')
        ++marker;
      ret.major = std::strtoul(marker, &marker, 10);
      if (marker[0] && marker[0] != '"')
        ret.minor = std::strtoul(marker + 1, &marker, 10);
      if (marker[0] && marker[0] != '"')
        ret.patch = std::strtoul(marker + 1, &marker, 10);
      if (marker[0] && marker[0] != '"')
        ret.build = std::strtoul(marker + 1, nullptr, 10);
    }
  }

  trim_quotes(ret.name);
  trim_quotes(ret.full_name);

  return ret;
}
#endif
