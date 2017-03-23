/*=============================================================================
 Copyright (c) 2001-2015 Arun Muralidharan
 
 Distributed under the Boost Software License, Version 1.0. (See accompanying
 file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 =============================================================================*/
#ifndef YANG_CPP_ERROR_HANDLER_HPP
#define YANG_CPP_ERROR_HANDLER_HPP

#include <map>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>

#include "stmt_grammars.hpp"

namespace x3 = boost::spirit::x3;

namespace yang_cpp { namespace parser {

  // X3 Error Handler Utility
  template <typename Iterator>
  using error_handler = x3::error_handler<Iterator>;
 
  // Tag used to get our error handler from the context
  struct error_handler_tag;

  /*!
   * @class error_handler_base
   * @brief The base error handling class.
   */
  class error_handler_base
  {
  public:
    /// Constructor
    /// Populate the mapping table
    error_handler_base();

    /*!
     * Called when the grammar fails to parse the 
     * input.
     */
    template <typename Iterator, 
              typename Exception, 
              typename Context>
    x3::error_handler_result on_error(Iterator& first, 
                                      const Iterator& last,
                                      const Exception& x, 
                                      const Context& context);
  private:
    std::map<std::string, std::string> id_map_;
  };

  /// Member function definitions

  error_handler_base::error_handler_base()
  {
    id_map_["namespace-stmt"]     = "Namespace";
    id_map_["prefix-stmt"]        = "Prefix";
    id_map_["organization-stmt"]  = "Organization";
    id_map_["contact-stmt"]       = "Contact";
    id_map_["revision-stmt"]      = "Revision";
    id_map_["revision-date-stmt"] = "RevisionDate";
    id_map_["import-stmt"]        = "Import";
    id_map_["description-stmt"]   = "Description";
    id_map_["error-message-stmt"] = "ErrorMessage";
    id_map_["error-app-tag-stmt"] = "ErrorAppTag";
    id_map_["config-stmt"]        = "Config";
    id_map_["reference-stmt"]     = "Reference";
  }


    template <typename Iterator, 
              typename Exception, 
              typename Context>
    x3::error_handler_result
    error_handler_base::on_error(
                          Iterator& first, 
                          const Iterator& last,
                          const Exception& x, 
                          const Context& context)
    {
      std::string which = x.which();
      auto iter = id_map.find(which);

      if (iter != id_map.end()) {
        which = iter->second;
      }

      std::string message = "Error! Expecting: " + which + " here:";
      auto& error_handler = x3::get<error_handler_tag>(context).get();
      error_handler(x.where(), message);

      return x3::error_handler_result::fail;
    }

}} //end namespace


#endif
