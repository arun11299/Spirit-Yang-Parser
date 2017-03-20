/*=============================================================================
 Copyright (c) 2001-2015 Arun Muralidharan

 Distributed under the Boost Software License, Version 1.0. (See accompanying
 file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 =============================================================================*/

#ifndef STATEMENT_AST_HPP
#define STATEMENT_AST_HPP

#include <string>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/optional.hpp>

namespace yang_cpp { namespace ast {

  struct import_stmt_t 
  {
    std::string module_name;
    std::string prefix;
    boost::optional<std::string> revision_date;
  };

  struct revision_stmt_t 
  {
    std::string year;
    std::string month;
    std::string day;
    std::string description;
  };

}}

#endif
