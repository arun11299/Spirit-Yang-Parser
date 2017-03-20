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
  using x3::ascii::digit;

  /* Rules for parsing comments */
  x3::rule<class scomment> single_line_comment = "single-comment";
  x3::rule<class mcomment> multi_line_comment  = "multi-line-comment";

  /* Rules for parsing common types of tokens */
  x3::rule<class qstring, std::string> quoted_string   = "quoted-string";
  x3::rule<class ustring, std::string> unquoted_string = "unquoted-string";

  // Yang staement terminator
  auto eo_st = lit(';');

  /* Rules for parsing top level (header) yang statements */
  x3::rule<class nspace,            std::string> nspace_stmt   = "namespace-stmt";
  x3::rule<class prefix,            std::string> prefix_stmt   = "prefix-stmt";
  x3::rule<class revision, ast::revision_stmt_t> revision_stmt = "revision-stmt";
  x3::rule<class revision_date,     std::string> revision_date = "revision-date-stmt";
  x3::rule<class import,     ast::import_stmt_t> import_stmt   = "import-stmt";

  auto single_line_comment_def = lit("//") >> lexeme[*char_] >> x3::eol;
  auto multi_line_comment_def = lit("/*") >> +alnum >> lit("*/");

  // Parses the namespace statement
  auto nspace_stmt_def = 
      lit("namespace") >> lit('"') >> +(alnum | char_(':') | char_('/')) >> lit('"') >> eo_st;

  // Parses a string with or without double quotes
  auto quoted_string_def = lexeme['"' >> *(char_ - '"') >> '"'];

  auto unquoted_string_def = *char_;

  // Parses the prefix statement
  // For eg: prefix "yang-mod";
  auto prefix_stmt_def = 
      ( lit("prefix") >> quoted_string >> -lit('"') ) > eo_st;

  auto revision_stmt_def = 
      lit("revision") 
      >> lit('"')
      >> (x3::repeat(4)[digit] > lit('-') > x3::repeat(2)[digit] > lit('-') > x3::repeat(2)[digit])
      >> lit('"')
      >> lit('{')
      >> lit("description") >> quoted_string >> eo_st
      >> lit('}');

  // parses the revision date
  auto revision_date_def = 
      lit("revision-date") >> quoted_string >> eo_st;

  // Parses the import statement
  // Import statement can be written in two ways..
  auto import_stmt_def = 
      lit("import") >> quoted_string 
      >> lit('{')
      >> prefix_stmt 
      >> -revision_date
      >> lit('}');

  BOOST_SPIRIT_DEFINE (quoted_string,
                       nspace_stmt,
                       prefix_stmt,
                       revision_date,
                       revision_stmt,
                       import_stmt);

}}

#endif
