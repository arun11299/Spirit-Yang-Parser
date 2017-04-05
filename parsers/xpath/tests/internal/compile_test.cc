#include <iostream>
#include "../../xpath_grammar.hpp"
#include "../../xpath_ast_helpers.hpp"

int main()
{
  using boost::spirit::x3::ascii::space;
  using yang_cpp::xpath::grammar::ncname;
  using yang_cpp::xpath::grammar::qname;
  using yang_cpp::xpath::grammar::name_test;
  using yang_cpp::xpath::grammar::step;

  {
    std::cout << "NCName test-----------" << std::endl;
    std::string input = "prefix:*";
    std::string prefix;
    bool res = phrase_parse(input.begin(), input.end(),
                            ncname,
                            space,
                            prefix
                            );
    if (!res) {
      std::cerr << "NCName parsing failed!\n";
      return 1;
    }
    std::cout << "Ncname: prefix = " << prefix << std::endl;

    std::cout << "-------End NcName test" << std::endl;
  }

  {
    std::cout << "QName test------------" << std::endl;
    std::string input = "prefix:test";
    std::string result;

    bool res = phrase_parse(input.begin(), input.end(),
                            qname,
                            space,
                            result
                            );
    if (!res) {
      std::cerr << "QName parsing failed!\n";
      return 1;
    }

    std::cout << "Prefix: " << result << std::endl;

    input = "just-name";
    std::string result2;

    res = phrase_parse(input.begin(), input.end(),
                       qname,
                       space,
                       result2
                       );
    if (!res) {
      std::cerr << "qname parsing failed(2)!\n";
      return 1;
    }

    std::cout << "Local part: " << result2<< std::endl;

    std::cout << "--------End QName test" << std::endl;
  }

  {
    std::cout << "NameTest start------------" << std::endl;
    std::string input = "*";

    for (std::string input : {"*", "only-local", "pref:local"}) {
      yang_cpp::xpath::ast::node_name_t ntest;

      bool res = phrase_parse(input.begin(), input.end(),
                              name_test,
                              space,
                              ntest
                             );

      if (!res) {
        std::cerr << "name test failed!\n";
        return 1;
      }

      if (ntest.is_wildstar()) {
        std::cout << input << " is wildstar" << std::endl;
        continue;
      }
      if (ntest.has_prefix()) {
        std::cout << "prefix: " << ntest.prefix() << std::endl;
      }
      std::cout << "local part: " << ntest.local_part() << std::endl;
    }
    std::cout << "--------------NameTest end" << std::endl;
  }

  {
    std::cout << "StepTest start------------" << std::endl;

    for (std::string input : {".", "..", "pref:local"}) { 
      yang_cpp::xpath::ast::step_t res_step;

      bool res = phrase_parse(input.begin(), input.end(),
                              step,
                              space,
                              res_step);
      if (!res) {
        std::cerr << "step test failed!\n";
        return 1;
      }

      //std::cout << res_step << std::endl;
    }

    std::cout << "--------------StepTest end" << std::endl;
  }

  return 0;
}
