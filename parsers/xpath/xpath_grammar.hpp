/*=============================================================================
 Copyright (c) 2001-2015 Arun Muralidharan
 
 Distributed under the Boost Software License, Version 1.0. (See accompanying
 file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 =============================================================================*/

#ifndef YANG_CPP_XPATH_GRAMMAR_HPP
#define YANG_CPP_XPATH_GRAMMAR_HPP

#include <string>
#include <boost/optional/optional_io.hpp>
#include <boost/spirit/home/x3.hpp>

#include "xpath_ast.hpp"
#include "xpath_ast_adapted.hpp"

namespace x3 = boost::spirit::x3;

namespace yang_cpp { namespace xpath { namespace grammar {

  using x3::lexeme;
  using x3::alpha;
  using x3::alnum;
  using x3::ascii::char_;
  using x3::ascii::string;
  using x3::lit;
  using x3::ascii::digit;

  /*!
   */
  struct node_type_: x3::symbols<xpath::ast::NodeType>
  {
    node_type_()
    {
      add
        ("comment()" , xpath::ast::NodeType::COMMENT)
        ("text()"    , xpath::ast::NodeType::TEXT)
        ("node()"    , xpath::ast::NodeType::NODE)
      ;
    }
  } node_type;

  auto current_path = lit(".");
  auto prev_path    = lit("..");

  //// Parser rules
  // Internal parser rules
  x3::rule<class qname_tok,  std::string> qname     = "qname-token";
  x3::rule<class ncname_tok, std::string> ncname    = "ncname-token";

  /// Exposed parser rules
  x3::rule<class step_tok,           xpath::ast::step_t> step      = "step";
  x3::rule<class name_test_tok, xpath::ast::node_name_t> name_test = "name-test-token";
  x3::rule<class node_test_tok, xpath::ast::node_test_t> node_test = "node-test-token";
  x3::rule<class relative_loc,  xpath::ast::rel_loc_path_t> relative_location = "relative-location";


  template<typename T>
  auto as = [](auto p) { return x3::rule<struct _, T>{} = as_parser(p); };


  //// Parser Rule definitions

  /*!
   * Non-Colonized name
   */
  auto ncname_def = lexeme[+(char_ - ':')];

  /*!
   * Qualified Name
   * QName ::= PrefixedName | UnprefixedName
   */
  auto qname_def = as<std::string>(ncname >> char_(':') >> ncname)
                 | as<std::string>(ncname)
                 ;

  /*!
   * Selects a node in the current context.
   * NameTest ::= '*'
   *            | NCName ':''*'
   *            | QName
   */
  auto name_test_def = qname;

  /*!
   * NodeTest ::= NameTest
   *            | NodeType'('')'
   *            | 'processing-instruction' '(' Literal ')' 
   *
   * NOTE: 'processing-instruction' token is not
   * supported in this implementation.
   */
  auto node_test_def = name_test
                     | node_type
                     ;

  /*!
   * A step in the xpath expression.
   *
   * Step ::= AxisSpecifier NodeTest Predicate*
   *          | AbbreviatedStep
   *
   * NOTE: AxisSpecifier is not implemented.
   *
   * TODO: Implement predicate.
   */
  auto step_def = node_test /* predicate */
                | current_path
                | prev_path
                ;

  /*!
   * Relative location path is basically a sequence
   * of steps.
   *
   * RelativeLocationPath ::= Step
   *                        | RelativeLocationPath '/' Step
   *                        | AbbreviatedRelativeLocationPath
   *
   * TODO: AbbreviatedRelativeLocationPath needs
   * to be implemented.
   */
  auto relative_location_def = step % '/';


  BOOST_SPIRIT_DEFINE (ncname,
                       qname,
                       step,
                       name_test,
                       node_test,
                       relative_location)

}}}

#endif
