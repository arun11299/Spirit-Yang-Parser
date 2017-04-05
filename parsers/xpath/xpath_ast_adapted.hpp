/*=============================================================================
 Copyright (c) 2001-2015 Arun Muralidharan
 
 Distributed under the Boost Software License, Version 1.0. (See accompanying
 file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 =============================================================================*/

#ifndef YANG_CPP_XPATH_AST_ADAPTED_HPP
#define YANG_CPP_XPATH_AST_ADAPTED_HPP

#include <boost/fusion/include/adapt_struct.hpp>

#include "xpath_ast.hpp"

BOOST_FUSION_ADAPT_STRUCT(
    yang_cpp::xpath::ast::node_name_t,
    (yang_cpp::xpath::ast::node_name_t::ast_type, name)
);

#endif
