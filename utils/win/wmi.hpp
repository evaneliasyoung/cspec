/**
 *  @file      wmi.hpp
 *  @brief     WMI wrapper class.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-21
 *  @date      2021-11-21
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#pragma once

#include "../../core.h"

#ifdef WIN
#include <Wbemidl.h>
#include <algorithm>
#include <codecvt>
#include <comdef.h>
#include <iostream>
#include <iterator>
#include <locale>
#include <map>
#include <sstream>
#include <vector>

class WMI
{
  struct InitStates
  {
    bool com = false;
    bool locator = false;
    bool service = false;
    bool enumerator = false;
    bool klass = false;
  } _inits;

  struct ResourcePointers
  {
    IWbemLocator *locator = NULL;
    IWbemServices *service = NULL;
    IEnumWbemClassObject *enumerator = NULL;
    IWbemClassObject *klass = NULL;
  } _res;

  public:
  HRESULT status = -1;
  bool failed = true;
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

  wstring widen(const string &in)
  {
    return this->converter.from_bytes(in);
  }

  string narrow(const wstring &in)
  {
    return this->converter.to_bytes(in);
  }

  bool Failed()
  {
    this->failed = FAILED(this->status);
    return this->failed;
  }

  ~WMI()
  {
    this->release_all();
  }

  void release_all()
  {
    this->release_enumerator();
    this->release_service();
    this->release_locator();
    this->release_com();
  }

  void release_com()
  {
    if (!this->_inits.com)
      return;
    CoUninitialize();
    this->_inits.com = false;
  }

  void release_locator()
  {
    if (!this->_inits.locator)
      return;
    this->_res.locator->Release();
    this->_inits.locator = false;
  }

  void release_service()
  {
    if (!this->_inits.service)
      return;
    this->_res.service->Release();
    this->_inits.service = false;
  }

  void release_enumerator()
  {
    if (!this->_inits.enumerator)
      return;
    this->_res.enumerator->Release();
    this->_inits.enumerator = false;
  }

  void release_class()
  {
    if (!this->_inits.com)
      return;
    this->_res.klass->Release();
    this->_inits.com = false;
  }

  bool initialize()
  {
    this->status = CoInitializeEx(0, COINIT_MULTITHREADED);

    if (!this->Failed())
      this->_inits.com = true;
    return !this->failed;
  }

  bool secure()
  {
    if (!this->_inits.com)
      return false;

    this->status = CoInitializeSecurity(NULL,
                                        -1,                          // COM authentication
                                        NULL,                        // Authentication services
                                        NULL,                        // Reserved
                                        RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication
                                        RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation
                                        NULL,                        // Authentication info
                                        EOAC_NONE,                   // Additional capabilities
                                        NULL                         // Reserved
    );

    if (this->Failed())
      this->release_all();
    return !this->failed;
  }

  bool locate()
  {
    if (!this->_inits.com)
      return false;

    this->status =
      CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID *)&this->_res.locator);
    this->_inits.locator = true;

    if (this->Failed())
      this->release_all();
    return !this->failed;
  }

  bool connect()
  {
    if (!this->_inits.locator)
      return false;

    this->status = this->_res.locator->ConnectServer(BSTR(L"ROOT\\CIMV2"), // Object path of WMI namespace
                                                     NULL,                 // User name. NULL = current user
                                                     NULL,                 // User password. NULL = current
                                                     0,                    // Locale. NULL indicates current
                                                     0,                    // Security flags.
                                                     0,                    // Authority (for example, Kerberos)
                                                     0,                    // Context object
                                                     &this->_res.service   // pointer to IWbemServices proxy
    );
    this->_inits.service = true;

    if (this->Failed())
      this->release_all();
    return !this->failed;
  }

  bool proxy()
  {
    if (!this->_inits.service)
      return false;

    this->status = CoSetProxyBlanket(this->_res.service,          // Indicates the proxy to set
                                     RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
                                     RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
                                     NULL,                        // Server principal name
                                     RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx
                                     RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
                                     NULL,                        // client identity
                                     EOAC_NONE                    // proxy capabilities
    );

    if (this->Failed())
      this->release_all();
    return !this->failed;
  }

  wstring build_request(const string &klass, const std::vector<string> &keys)
  {
    std::stringstream request;

    request << "SELECT ";
    for (const auto &key: keys)
      request << key << ", ";
    request.seekp(-2, request.cur);
    request << " FROM " << klass;

    return this->widen(request.str());
  }

  bool prepare()
  {
    return this->initialize() && this->secure() && this->locate() && this->connect() && this->proxy();
  }

  bool query(const string &klass, const std::vector<string> &keys)
  {
    if (!this->_inits.service)
      return false;

    wstring request = this->build_request(klass, keys);

    this->status = this->_res.service->ExecQuery(BSTR(L"WQL"), (BSTR)request.c_str(),
                                                 WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL,
                                                 &this->_res.enumerator);
    this->_inits.enumerator = true;

    if (this->Failed())
      this->release_all();

    return !this->failed;
  }

  bool enumerate()
  {
    if (!this->_inits.enumerator)
      return false;

    ULONG ret = 0;
    this->_res.enumerator->Next(WBEM_INFINITE, 1, &this->_res.klass, &ret);

    return ret != 0;
  }

  std::map<string, string> retrieve(const std::vector<string> &keys)
  {
    std::vector<wstring> wkeys;
    std::map<string, string> keyval;
    std::transform(keys.cbegin(), keys.cend(), back_inserter(wkeys),
                   [this](const auto &key)
                   {
                     return this->widen(key);
                   });

    ULONG ret = 0;

    while (this->_res.enumerator)
    {
      if (!this->enumerate())
        break;

      VARIANT prop;
      for (const auto &key: wkeys)
      {
        this->_res.klass->Get(key.c_str(), 0, &prop, 0, 0);
        keyval.insert(make_pair(this->narrow(key), this->narrow(prop.bstrVal)));
        VariantClear(&prop);
      };

      this->release_class();
    }
    return keyval;
  }

  std::map<string, string> query_and_retrieve(const string &klass, const std::vector<string> &keys)
  {
    if (!this->query(klass, keys))
      return {};
    return this->retrieve(keys);
  }
};
#endif
