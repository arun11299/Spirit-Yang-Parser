/*=============================================================================
 Copyright (c) 2001-2015 Arun Muralidharan

 Distributed under the Boost Software License, Version 1.0. (See accompanying
 file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef YANG_CPP_STMT_GRAMMARS_HPP
#define YANG_CPP_STMT_GRAMMARS_HPP

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

  // Yang staement terminator
  auto eo_stmt = lit(';');

  /* Rules for parsing comments */
  x3::rule<class scomment> single_line_comment = "single-comment";
  x3::rule<class mcomment> multi_line_comment  = "multi-line-comment";

  /* Rules for parsing common types of tokens */
  x3::rule<class qstring, std::string> quoted_string    = "quoted-string";
  x3::rule<class ustring, std::string> unquoted_string  = "unquoted-string";
  x3::rule<class mstring, std::string> multiline_string = "multiline-string"; 

  /* Rules for parsing top level (header) yang statements */
  x3::rule<class nspace,            std::string> nspace_stmt    = "namespace-stmt";
  x3::rule<class prefix,            std::string> prefix_stmt    = "prefix-stmt";
  x3::rule<class organization,      std::string> org_stmt       = "organization-stmt";
  x3::rule<class contact,           std::string> contact_stmt   = "contact-stmt";
  x3::rule<class revision, ast::revision_stmt_t> revision_stmt  = "revision-stmt";
  x3::rule<class revision_date,     std::string> revision_date  = "revision-date-stmt";
  x3::rule<class import,     ast::import_stmt_t> import_stmt    = "import-stmt";
  x3::rule<class description,       std::string> desc_stmt      = "description-stmt";
  x3::rule<class error_msg,         std::string> error_msg_stmt = "error-message-stmt";
  x3::rule<class err_app_tag,       std::string> error_app_tag_stmt  = "error-app-tag-stmt";
  x3::rule<class config,            std::string> config_stmt    = "config-stmt";
  x3::rule<class reference,         std::string> reference_stmt = "reference-stmt";

  /// Pattern statement and its substatements

  /*!
   * Single line comment parser.
   * Eg: "// anything here is comment till end of line"
   * Currently, there is no attribute synthesized for
   * comments.
   */
  auto single_line_comment_def = lit("//") >> lexeme[*char_] >> x3::eol;

  /*!
   * Multi line comment parser.
   * Eg: Like this comment!
   * Currently, there is no attribute synthesized for
   * multi line comments.
   */
  auto multi_line_comment_def = lit("/*") >> +alnum >> lit("*/");

  /*!
   * Parses a double quoted string without the quotes.
   * Spaces are also included.
   * The string can begin on a new line also.
   */
  auto quoted_string_def = lexeme[-x3::eol >> '"' >> *(char_ - '"') >> '"'];

  /*!
   * Parses an unquoted string.
   * Spaces are not included.
   */
  auto unquoted_string_def = *char_;

  /*!
   * Multiline string parser.
   * Useful for parsing big strings like description etc.
   * TODO: Tabs/spaces in the beginning of a new line
   * are not ignored.
   * The string can start from a new line
   */
  auto multiline_string_def = 
      lexeme[-x3::eol >> '"' >> *((char_ - '"') | x3::eol) >> '"'];

  /*!
   * Section [7.19.3]
   * Parses the description statement.
   *
   */
  auto desc_stmt_def = lit("description") >> multiline_string >> eo_stmt;

  /*!
   * Section [7.1.3]
   * Parses the namespace statement.
   * The argument to the namespace is the URI of
   * the namespace.
   * The parsed URI is stored as a string.
   * No validation is done while parsing.
   */
  auto nspace_stmt_def = 
      lit("namespace") >> lit('"') >> +(alnum | char_(':') | char_('/')) >> lit('"') >> eo_stmt;


  /*!
   * Section [7.1.4]
   * Parses the prefix statement.
   * The argument of the prefix statement is the 
   * prefix string that is used as prefix to access a 
   * module.
   * The parsed prefix is stored as a string;
   */
  auto prefix_stmt_def = 
      ( lit("prefix") >> quoted_string ) > eo_stmt;

  /*!
   * Section [7.1.7]
   * Parses the organization statment.
   */
  auto org_stmt_def = 
      ( lit("organization") >> quoted_string ) > eo_stmt;

  /*!
   * Section [7.1.4]
   * Parses the contact statement.
   */
  auto contact_stmt_def = 
      ( lit("contact") >> quoted_string ) > eo_stmt;

  /*!
   * Section [7.1.9]
   * Parses the reviosion statement.
   * The parser only parses one definition os the revision
   * statement. There could be multiple revision statements
   * in a yang module, which need to be hadled in a composed
   * parser.
   * The parsed statement is stored in `revision_stmt_t`.
   * No validations are performed.
   */
  auto revision_stmt_def = 
      lit("revision") 
      >> lit('"')
      >> (x3::repeat(4)[digit] > lit('-') > x3::repeat(2)[digit] > lit('-') > x3::repeat(2)[digit])
      >> lit('"')
      >> lit('{') >> -x3::eol
      >> desc_stmt
      >> lit('}');

  /*!
   * Section [7.1.5]
   * Parses the revision date inside the import statement.
   */
  auto revision_date_def = 
      ( lit("revision-date") >> quoted_string ) > eo_stmt;

  /*!
   * Section [7.5.4.1]
   * Parses the optional error-message statement.
   * The parsed value/argument is stored as a string.
   * TODO: Parses a regular quoted string.
   */
  auto error_msg_stmt_def =
      ( lit("error-message") >> quoted_string ) > eo_stmt;

  /*!
   * Section [7.5.4.2]
   * Parses the optional error-app-tag statement.
   * The parsed value/argument is stored as a string.
   * TODO: Parses a regular quoted string.
   */
  auto error_app_tag_stmt_def =
      ( lit("error-app-tag") >> quoted_string ) > eo_stmt;

  /*!
   * Section [7.19.1]
   * Parses the config statement.
   * The argument is stored in a string.
   * TODO: Validation of the argument not done.
   */
  auto config_stmt_def =
      ( lit("config") >> (x3::string("true") | x3::string("false"))) > eo_stmt;

  /*!
   * Section [7.19.4]
   * Parses the reference statement.
   * The parsed argument is stored as a string.
   */
  auto reference_stmt_def =
      ( lit("reference") >> multiline_string ) > eo_stmt;

  /*!
   * Section [7.1.5]
   * Parses the import statement. The argument is the
   * name of the module to import.
   * It also parses the mandatory prefix statement and
   * the optional revision date.
   *
   * The parsed details are stored in `import_stmt_t`.
   */
  auto import_stmt_def = 
      lit("import") >> quoted_string 
      >> lit('{') >> -x3::eol
      >> prefix_stmt 
      >> -revision_date
      >> lit('}');

  BOOST_SPIRIT_DEFINE (quoted_string,
                       multiline_string,
                       nspace_stmt,
                       prefix_stmt,
                       org_stmt,
                       contact_stmt,
                       revision_date,
                       revision_stmt,
                       import_stmt,
                       desc_stmt,
                       error_msg_stmt,
                       error_app_tag_stmt,
                       config_stmt,
                       reference_stmt);

}}

#endif
