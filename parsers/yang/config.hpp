/*=============================================================================
 Copyright (c) 2001-2015 Arun Muralidharan
 
 Distributed under the Boost Software License, Version 1.0. (See accompanying
 file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 =============================================================================*/

#ifndef YANG_CPP_CONFIG_HPP
#define YANG_CPP_CONFIG_HPP

#include <boost/spirit/home/x3.hpp>
#include "error_handler.hpp"

namespace x3 = boost::spirit::x3;

namespace yang_cpp { namespace parser {

  // Our Iterator Type
  using iterator_type = std::string::const_iterator;

  // The Phrase Parse Context
  using phrase_context_type = 
    typename x3::phrase_parse_context<x3::ascii::space_type>::type;

  // Our Error Handler
  using error_handler_type = error_handler<iterator_type>;

  // Combined Error Handler and Phrase Parse Context
  using context_type = 
    typename x3::with_context<error_handler_tag,
                              std::reference_wrapper<error_handler_type> const,
                              phrase_context_type>::type;

}}

#endif
