/*=============================================================================
 Copyright (c) 2001-2015 Arun Muralidharan
 
 Distributed under the Boost Software License, Version 1.0. (See accompanying
 file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 =============================================================================*/

#ifndef STATEMENT_AST_ADAPTED_HPP
#define STATEMENT_AST_ADAPTED_HPP

#include "statement_ast.hpp"

BOOST_FUSION_ADAPT_STRUCT (
    yang_cpp::ast::import_stmt_t,
    (std::string, module_name),
    (std::string, prefix),
    (boost::optional<std::string>, revision_date)
);

BOOST_FUSION_ADAPT_STRUCT (
    yang_cpp::ast::revision_stmt_t,
    (std::string, year),
    (std::string, month),
    (std::string, day),
    (std::string, description)
);

#endif
