/*=============================================================================
 Copyright (c) 2001-2015 Arun Muralidharan
 
 Distributed under the Boost Software License, Version 1.0. (See accompanying
 file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 =============================================================================*/

#ifndef YANG_CPP_XPATH_AST_HPP
#define YANG_CPP_XPATH_AST_HPP

#include <string>
#include <vector>
#include <boost/optional.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>

namespace x3 = boost::spirit::x3;

namespace yang_cpp { namespace xpath { namespace ast {

  using ncname_t = std::string;

  /*!
   */
  enum class NodeType
  {
    COMMENT,
    TEXT,
    NODE,
  };

  /*!
   * Represents the node name of a step.
   * Stored as a string if the parsed
   * step is of type 'NameTest' token.
   */
  struct node_name_t
  {
    using ast_type = std::string;
    ast_type name;

    /// Not Part of adapted ast
  public:
    // Helper functions
    bool has_prefix() noexcept;
    bool is_wildstar() const noexcept;
    
    const std::string& prefix() const noexcept;
    const std::string& local_part() const noexcept;

  private:
    std::string prefix_;
    std::string local_part_;
  };


  /*!
   */
  struct node_test_t: x3::variant<
                        node_name_t,
                        NodeType
                      >
  {
    using base_type::base_type;
    using base_type::operator=;
  };

  /*!
   */
  struct step_t: x3::variant<
                  node_test_t,
                  std::string
                 >
  {
    using base_type::base_type;
    using base_type::operator=;
  };


  using loc_path_t = std::vector<step_t>;



  /*
   * Predicate Expressions
   */


  struct expression;
  struct function_call;
  struct filter_expr_impl;
  struct union_expr_impl;
  struct unary_expr_impl;
  struct mult_expr_impl;
  struct add_expr_impl;
  struct rel_expr_impl;
  struct eq_expr_impl;
  struct and_expr_impl;
  struct or_expr_impl;
  struct path_expression;

  struct primary_expression: x3::variant<
                              std::string,
                              x3::forward_ast<expression>,
                              std::string,
                              int32_t,
                              uint32_t,
                              double,
                              x3::forward_ast<function_call>
                             >
  {
    using base_type::base_type;
    using base_type::operator=;
  };

  struct union_expression: x3::variant<
                            x3::forward_ast<path_expression>,
                            x3::forward_ast<union_expr_impl>
                           >
  {
    using base_type::base_type;
    using base_type::operator=;
  };

  struct unary_expression: x3::variant<
                            union_expression,
                            x3::forward_ast<unary_expr_impl>
                           >
  {
    using base_type::base_type;
    using base_type::operator=;
  };

  struct unary_expr_impl
  {
    unary_expression expr_;
  };

  struct multiplicative_expression: x3::variant<
                                      unary_expression,
                                      x3::forward_ast<mult_expr_impl>
                                    >
  {
    using base_type::base_type;
    using base_type::operator=;
  };

  struct mult_expr_impl
  {
    std::pair<multiplicative_expression, unary_expression> expr_;
  };

  struct additive_expression: x3::variant<
                                multiplicative_expression,
                                x3::forward_ast<add_expr_impl>
                              >
  {
    using base_type::base_type;
    using base_type::operator=;
  };

  struct add_expr_impl
  {
    std::pair<additive_expression, multiplicative_expression> expr_;
  };

  struct relational_expression: x3::variant<
                                  additive_expression,
                                  x3::forward_ast<rel_expr_impl>
                                >
  {
    using base_type::base_type;
    using base_type::operator=;
  };

  struct rel_expr_impl
  {
    std::pair<relational_expression, additive_expression> expr_;
  };

  struct equality_expression: x3::variant<
                                relational_expression,
                                x3::forward_ast<eq_expr_impl>
                              >
  {
    using base_type::base_type;
    using base_type::operator=;
  };

  struct eq_expr_impl
  {
    std::pair<equality_expression, relational_expression> expr_;
  };

  struct and_expression: x3::variant<
                          equality_expression,
                          x3::forward_ast<and_expr_impl>
                         >
  {
    using base_type::base_type;
    using base_type::operator=;
  };

  struct and_expr_impl
  {
    std::pair<and_expression, equality_expression> expr_;
  };

  struct or_expression: x3::variant<
                          and_expression,
                          x3::forward_ast<or_expr_impl> 
                        >
  {
    using base_type::base_type;
    using base_type::operator=;
  };

  struct or_expr_impl
  {
    std::pair<or_expression, and_expression> expr_;
  };

  struct filter_expression: x3::variant<
                              primary_expression,
                              x3::forward_ast<filter_expr_impl>
                            >
  {
    using base_type::base_type;
    using base_type::operator=;
  };

  struct filter_expr_impl
  {
    std::pair<filter_expression, expression> expr_;
  };

  struct path_expression: x3::variant<
                            loc_path_t,
                            filter_expression,
                            std::pair<filter_expression, loc_path_t>
                          >
  {
    using base_type::base_type;
    using base_type::operator=;
  };

  struct union_expr_impl
  {
    std::pair<union_expression, path_expression> expr_;
  };

  struct function_call
  {
    std::string function_name;
    std::vector<expression> args;
  };


}}}

#endif
