/**
 *  @file      wmi.cpp
 *  @brief     WMI wrapper class.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-29
 *  @date      2021-11-29
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"

#ifdef WIN
wstring cspec::shared::WMI::widen(const string &in)
{
  return this->converter.from_bytes(in);
}

string cspec::shared::WMI::narrow(const wstring &in)
{
  return this->converter.to_bytes(in);
}

bool cspec::shared::WMI::Failed()
{
  this->failed = FAILED(this->status);
  return this->failed;
}

cspec::shared::WMI::WMI(const bool &prepare)
{
  if (prepare)
    this->prepare();
}

cspec::shared::WMI::~WMI()
{
  this->release_all();
}

void cspec::shared::WMI::release_all()
{
  this->release_enumerator();
  this->release_service();
  this->release_locator();
  this->release_com();
}

void cspec::shared::WMI::release_com()
{
  if (!this->_inits.com)
    return;
  CoUninitialize();
  this->_inits.com = false;
}

void cspec::shared::WMI::release_locator()
{
  if (!this->_inits.locator)
    return;
  this->_res.locator->Release();
  this->_inits.locator = false;
}

void cspec::shared::WMI::release_service()
{
  if (!this->_inits.service)
    return;
  this->_res.service->Release();
  this->_inits.service = false;
}

void cspec::shared::WMI::release_enumerator()
{
  if (!this->_inits.enumerator)
    return;
  this->_res.enumerator->Release();
  this->_inits.enumerator = false;
}

void cspec::shared::WMI::release_class()
{
  if (!this->_inits.klass)
    return;
  this->_res.klass->Release();
  this->_inits.klass = false;
}

bool cspec::shared::WMI::initialize()
{
  this->status = CoInitializeEx(0, COINIT_MULTITHREADED);

  if (!this->Failed())
    this->_inits.com = true;
  return !this->failed;
}

bool cspec::shared::WMI::secure()
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

bool cspec::shared::WMI::locate()
{
  if (!this->_inits.com)
    return false;

  this->status = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator,
                                  reinterpret_cast<LPVOID *>(&this->_res.locator));
  this->_inits.locator = true;

  if (this->Failed())
    this->release_all();
  return !this->failed;
}

bool cspec::shared::WMI::connect()
{
  if (!this->_inits.locator)
    return false;

  this->status = this->_res.locator->ConnectServer(
    static_cast<BSTR>(const_cast<wchar_t *>(L"ROOT\\CIMV2")), // Object path of WMI namespace
    NULL,                                                     // User name. NULL = current user
    NULL,                                                     // User password. NULL = current
    0,                                                        // Locale. NULL indicates current
    0,                                                        // Security flags.
    0,                                                        // Authority (for example, Kerberos)
    0,                                                        // Context object
    &this->_res.service                                       // pointer to IWbemServices proxy
  );
  this->_inits.service = true;

  if (this->Failed())
    this->release_all();
  return !this->failed;
}

bool cspec::shared::WMI::proxy()
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

wstring cspec::shared::WMI::build_request(const string &klass, const vector<string> &keys)
{
  std::stringstream request;
  this->stream_request(request, klass, keys);
  return this->widen(request.str());
}

wstring cspec::shared::WMI::build_request(const wstring &klass, const vector<wstring> &keys)
{
  std::wstringstream request;
  this->stream_request(request, klass, keys);
  return request.str();
}

bool cspec::shared::WMI::prepare()
{
  return this->initialize() && this->secure() && this->locate() && this->connect() && this->proxy();
}

bool cspec::shared::WMI::enumerate()
{
  if (!this->_inits.enumerator)
    return false;

  ULONG ret = 0;
  this->_res.enumerator->Next(-1, 1, &this->_res.klass, &ret);
  this->failed = ret == 0;
  this->_inits.klass = !this->failed;

  return !this->failed;
}

std::map<string, string> cspec::shared::WMI::retrieve(const vector<string> &keys)
{
  return this->retrieve_multiple(keys)[0];
}

vector<std::map<string, string>> cspec::shared::WMI::retrieve_multiple(const vector<string> &keys)
{
  // Start by converting all strings to wstrings
  vector<wstring> wkeys;
  std::transform(keys.cbegin(), keys.cend(), back_inserter(wkeys),
                 [this](const auto &key)
                 {
                   return this->widen(key);
                 });
  return this->retrieve_multiple(wkeys);
}

std::map<string, string> cspec::shared::WMI::retrieve(const vector<wstring> &keys)
{
  return this->retrieve_multiple(keys)[0];
}

vector<std::map<string, string>> cspec::shared::WMI::retrieve_multiple(const vector<wstring> &keys)
{
  vector<std::map<string, string>> ret;
  while (this->_res.enumerator)
  {
    std::map<string, string> keyval;
    if (!this->enumerate())
      break;

    VARIANT prop;
    for (const auto &key: keys)
    {
      this->_res.klass->Get(key.c_str(), 0, &prop, 0, 0);
      wstring val;

      switch (prop.vt)
      {         // I hate this, but I'm sure it's weird legacy garbage
        case 3: // u64
          val = std::to_wstring(prop.ullVal);
          break;
        case 8: // str
          val = prop.bstrVal;
          break;
        case 11: // bool
          val = std::to_wstring(prop.boolVal);
          break;
        default: // noent
          continue;
          break;
      }
      keyval.insert(make_pair(this->narrow(key), this->narrow(val)));
      VariantClear(&prop);
    };

    this->release_class();
    ret.push_back(keyval);
  }
  return ret;
}
#endif
