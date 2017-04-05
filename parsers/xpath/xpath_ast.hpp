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


  using rel_loc_path_t = std::vector<step_t>;



}}}

#endif
