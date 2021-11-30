/**
 *  @file      ns.h
 *  @brief     Shared code for Windows.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-29
 *  @date      2021-11-29
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#pragma once

#include "../../core.h"

#include <Wbemidl.h>
#include <algorithm>
#include <codecvt>
#include <comdef.h>
#include <iostream>
#include <iterator>
#include <locale>
#include <sstream>
#include <windows.h>

namespace cspec
{
  namespace shared
  {
    string transcode_from_wide(const wchar_t *wstr, std::size_t wstr_size);
    string narrow_wstring(const wchar_t *wstr);
    string narrow_bstring(const wchar_t *bstr);
    vector<SYSTEM_LOGICAL_PROCESSOR_INFORMATION> cpuinfo_buffer();

    bool open_registry_path(HKEY *hkey, HKEY source_key, const char *path);
    u32 read_registry_dw(HKEY source_key, const char *path, const char *key);
    umax read_registry_qw(HKEY source_key, const char *path, const char *key);
    template<u32 BufferSize> string read_registry_sz(HKEY source_key, const char *path, const char *key);

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

      wstring widen(const string &in);
      string narrow(const wstring &in);
      bool Failed();

      WMI(const bool &prepare = false);
      ~WMI();

      void release_all();
      void release_com();
      void release_locator();
      void release_service();
      void release_enumerator();
      void release_class();

      bool initialize();
      bool secure();
      bool locate();
      bool connect();
      bool proxy();

      template<typename StringWidth, typename StreamWidth>
      StringWidth stream_request(StreamWidth &stream, const StringWidth klass, const vector<StringWidth> &keys);
      wstring build_request(const string &klass, const vector<string> &keys);
      wstring build_request(const wstring &klass, const vector<wstring> &keys);
      bool prepare();
      template<typename StringWidth> bool query(const StringWidth &klass, const vector<StringWidth> &keys);
      template<typename StringWidth> bool query(const StringWidth &klass, const StringWidth &key);
      bool enumerate();
      std::map<string, string> retrieve(const vector<string> &keys);
      vector<std::map<string, string>> retrieve_multiple(const vector<string> &keys);
      std::map<string, string> retrieve(const vector<wstring> &keys);
      vector<std::map<string, string>> retrieve_multiple(const vector<wstring> &keys);
      template<typename StringWidth>
      std::map<string, string> query_and_retrieve(const StringWidth &klass, const vector<StringWidth> &keys);
      template<typename StringWidth> vector<std::map<string, string>>
      query_and_retrieve_multiple(const StringWidth &klass, const vector<StringWidth> &keys);
    };
  } // namespace shared
} // namespace cspec

#include "registry.tpp"
#include "wmi.tpp"
