/**
 *  @file      exception.cpp
 *  @brief     Defines new exceptions for cspec cli.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-25
 *  @date      2021-11-25
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"

const char *cspec::cli::exception::what() const
{
  return this->_msg.c_str();
}

cspec::cli::invalid_namespace::invalid_namespace(const string &ns)
{
  std::stringstream ss;
  ss << "invalid namespace " << '"' << ns << '"' << "; excepted \"all\" or one of {";
  for (const auto &exp: cspec::cli::namespaces)
    ss << '"' << exp << '"' << ',' << ' ';
  ss.seekp(ss.cur - 3, std::ios_base::end);
  ss << '}';
  this->_msg = ss.str();
}

cspec::cli::invalid_query::invalid_query(const string &ns, const string &query)
{
  std::stringstream ss;
  ss << "invalid query " << '"' << query << '"' << " in " << '"' << ns << '"' << "; excepted \"all\" or one of {";
  for (const auto &exp: cspec::cli::queries.at(ns))
    ss << '"' << exp << '"' << ',' << ' ';
  ss.seekp(ss.cur - 3, std::ios_base::end);
  ss << '}';
  this->_msg = ss.str();
}
