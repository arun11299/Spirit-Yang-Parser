/*=============================================================================
 Copyright (c) 2001-2015 Arun Muralidharan

 Distributed under the Boost Software License, Version 1.0. (See accompanying
 file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef  STMT_GRAMMARS_HPP
#define STMT_GRAMMARS_HPP

#include <string>
#include <boost/optional/optional_io.hpp>
#include <boost/spirit/home/x3.hpp>

#include "../ast/statement_ast.hpp"
#include "../ast/statement_ast_adapted.hpp"

namespace x3 = boost::spirit::x3;

namespace yang_cpp { namespace grammar {

  using x3::raw;
  using x3::lexeme;
  using x3::alpha;
  using x3::alnum;
  using x3::ascii::char_;
  using x3::lit;

  /* Rules for parsing comments */
  x3::rule<class scomment> single_line_comment = "single-comment";
  x3::rule<class mcomment> multi_line_comment  = "multi-line-comment";

  /* Rules for parsing common types of tokens */
  x3::rule<class qstring,        std::string> quoted_string = "quoted-string";
  auto eol = lit(';');

  /* Rules for parsing top level (header) yang statements */
  x3::rule<class prefix,         std::string> prefix_stmt   = "prefix-stmt";
  x3::rule<class import,  ast::import_stmt_t> import_stmt   = "import-stmt";

  auto single_line_comment_def = lit("//") >> +alnum;

  auto multi_line_comment_def = lit("/*") >> +alnum >> lit("*/");

  // Parses a string with or without double quotes
  auto quoted_string_def = -lit('"') >> +alnum >> -lit('"');

  // Parses the prefix statement
  // For eg: prefix "yang-mod";
  auto prefix_stmt_def = 
      ( lit("prefix") >> quoted_string >> -lit('"') ) > eol;

  // Parses the import statement
  // Import statement can be written in two ways..
  // TODO: cross verify with RFC
  auto import_stmt_def = 
      ( lit("import") >> quoted_string ) >>
      ( eol | ( lit('{') >> prefix_stmt >> lit('}')) );

  BOOST_SPIRIT_DEFINE (quoted_string,
                       prefix_stmt,
                       import_stmt);

}}

#endif
