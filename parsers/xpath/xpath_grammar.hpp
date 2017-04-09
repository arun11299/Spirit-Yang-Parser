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

  /// Expression Grammar
  x3::rule<class expr,     xpath::ast::expression> expr = "expression";
  x3::rule<class or_expr,  xpath::ast::or_expression> or_expr = "or-expr";
  x3::rule<class and_expr, xpath::ast::and_expression> and_expr = "and-expr";
  x3::rule<class eq_expr,  xpath::ast::eq_expression> equality_expr = "equality-expr";
  x3::rule<class rel_expr, xpath::ast::rel_expression> relational_expr = "rel-expr";
  x3::rule<class add_expr, xpath::ast::add_expression> additive_expr = "add-expr";
  x3::rule<class mult_expr, xpath::ast::mul_expression> multiplicative_expr = "mul-expr";
  x3::rule<class unary_expr, xpath::ast::unary_expression> unary_expr = "unary-expr";
  x3::rule<class union_expr, xpath::ast::union_expression> union_expr = "union-expr";
  x3::rule<class filter_expr, xpath::ast::filter_expression> filter_expr = "filter-expr";

  /// Exposed parser rules
  x3::rule<class step_tok,           xpath::ast::step_t> step      = "step";
  x3::rule<class name_test_tok, xpath::ast::node_name_t> name_test = "name-test-token";
  x3::rule<class node_test_tok, xpath::ast::node_test_t> node_test = "node-test-token";


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
   */
  auto union_expr_def = path_expr
                      | (union_expr >> x3::lit('|') >> path_expr)
                      ;

  /*!
   */
  auto unary_expr_def = union_expr
                      | (x3::lit('-') >> unary_expr)
                      ;

  /*!
   */
  auto multiplicative_expr_def = unary_expr
                               | (multiplicative_expr >> x3::lit('*')   >> unary_expr)
                               | (multiplicative_expr >> x3::lit("div") >> unary_expr)
                               | (multiplicative_expr >> x3::lit("mod") >> unary_expr)
                               ;

  /*!
   */
  auto additive_expr_def = multiplicative_expr
                         | (additive_expr >> x3::lit('+') >> multiplicative_expr)
                         | (additive_expr >> x3::lit('-') >> multiplicative_expr)
                         ;

  /*!
   */
  auto relational_expr_def = additive_expr
                           | (relational_expr >> x3::lit('<')  >> additive_expr)
                           | (relational_expr >> x3::lit('>')  >> additive_expr)
                           | (relational_expr >> x3::lit("<=") >> additive_expr)
                           | (relational_expr >> x3::lit(">=") >> additive_expr)
                           ;

  /*!
   */
  auto equality_expr_def = relational_expr
                         | (equality_expr >> x3::lit("=")  >> relational_expr)
                         | (equality_expr >> x3::lit("!=") >> relational_expr) 
                         ;

  /*!
   */
  auto and_expr_def = equality_expr
                    | (and_expr >> x3::lit("and") >> equality_expr)
                    ;

  /*!
   */
  auto or_expr_def = and_expr
                   | (or_expr >> x3::lit("or") >> and_expr)
                   ;

  /*!
   */
  auto expr_def = or_expr;

  /*!
   */
  auto function_name = qname - node_type;

  /*!
   */
  auto function_arg  = expr;

  /*!
   */
  auto function_call = 
              function_name >> '(' >> -(function_arg % ',') >> ')';

  /*!
   */
  auto primary_expr = ('$' >> qname)                  // A variable reference
                    | ('(' >> expr >> ')')            // Expression
                    | ('"' >> *(char_ - '"') >> '"')  // Literal
                    | (int_ | uint_ | double_)        // Number
                    | function_call                   // Function Call
                    ;

  /*!
   */
  auto predicate = '[' > expr > ']';

  /*!
   */
  auto filter_expr_def = primary_expr
                       | (filter_expr >> predicate)
                       ;

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
   * AST generated is ast::loc_path_t
   */
  auto relative_location = step % '/';


  /*!
   * Absolute location path is basically a sequence
   * os steps but starts with '/'.
   *
   * AbsoluteLocationPath ::= '/' RelativeLocationPath ?
   *                        | AbbreviatedAbsoluteLocationPath
   *
   * AST generated is ast::loc_path_t
   */
  auto absolute_location = '/' % step;

  /*!
   */

  auto location_path = relative_location | absolute_location;


  /*!
   */
  auto path_expr = location_path
                 | filter_expr
                 | (filter_expr >> '/' >> relative_location)
                 | (filter_expr >> x3::lit("//") >> relative_location)
                 ;


  BOOST_SPIRIT_DEFINE (ncname,
                       qname,
                       step,
                       name_test,
                       node_test)

}}}

#endif
