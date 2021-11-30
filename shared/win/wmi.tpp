/**
 *  @file      wmi.tpp
 *  @brief     WMI wrapper class.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-29
 *  @date      2021-11-29
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#pragma once

#ifdef WIN
template<typename StringWidth, typename StreamWidth> StringWidth
cspec::shared::WMI::stream_request(StreamWidth &stream, const StringWidth klass, const vector<StringWidth> &keys)
{
  stream << "SELECT ";
  for (const auto &key: keys)
    stream << key << ", ";
  stream.seekp(-2, stream.cur);
  stream << " FROM " << klass;
  return stream.str();
}

template<typename StringWidth> bool cspec::shared::WMI::query(const StringWidth &klass, const vector<StringWidth> &keys)
{
  if (!this->_inits.service)
    return false;

  // Construct SQL-esce request from keys
  wstring request = this->build_request(klass, keys);
  this->status = this->_res.service->ExecQuery(
    static_cast<BSTR>(const_cast<wchar_t *>(L"WQL")), static_cast<BSTR>(const_cast<wchar_t *>(request.c_str())),
    WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &this->_res.enumerator);
  this->_inits.enumerator = true;

  if (this->Failed())
    this->release_all();

  return !this->failed;
}

template<typename StringWidth> bool cspec::shared::WMI::query(const StringWidth &klass, const StringWidth &key)
{
  return this->query(klass, vector<StringWidth>({key}));
}

template<typename StringWidth> std::map<string, string>
cspec::shared::WMI::query_and_retrieve(const StringWidth &klass, const vector<StringWidth> &keys)
{
  return this->query(klass, keys) ? this->retrieve(keys) : std::map<string, string>();
}

template<typename StringWidth> vector<std::map<string, string>>
cspec::shared::WMI::query_and_retrieve_multiple(const StringWidth &klass, const vector<StringWidth> &keys)
{
  return this->query(klass, keys) ? this->retrieve_multiple(keys) : vector<std::map<string, string>>();
}
#endif
