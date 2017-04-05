/*=============================================================================
 Copyright (c) 2001-2015 Arun Muralidharan
 
 Distributed under the Boost Software License, Version 1.0. (See accompanying
 file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 =============================================================================*/

#ifndef YANG_CPP_XPATH_AST_HELPERS_HPP
#define YANG_CPP_XPATH_AST_HELPERS_HPP

#include "xpath_ast.hpp"

namespace yang_cpp { namespace xpath { namespace ast {

bool node_name_t::has_prefix() noexcept
{
  if (local_part_.length()) {
    return prefix_.length() ? true : false;
  }

  auto pos = name.find(':');
  if (pos != std::string::npos) {
    prefix_ = name.substr(0, pos);
    local_part_ = name.substr(pos);
    return true;
  }

  local_part_ = name;
  return false;
}

bool node_name_t::is_wildstar() const noexcept
{
  return name[0] == '*';
}

const std::string& node_name_t::prefix() const noexcept
{
  return prefix_;
}

const std::string& node_name_t::local_part() const noexcept
{
  return local_part_;
}

}}}

#endif
