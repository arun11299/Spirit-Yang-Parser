#include "stmt_grammars.hpp"
#include <iostream>


int main() {
  using boost::spirit::x3::ascii::space;
  using yang_cpp::grammar::import_stmt;

  std::string input = "import \"test\" {"
    "prefix \"test\";"
  "}";
  std::cout << input << std::endl;
  yang_cpp::ast::import_stmt_t imp;

  bool res = phrase_parse(
        std::begin(input), std::end(input),
        import_stmt,
        space,
        imp
       );
  std::cout << "Import stmt: " << imp.module_name << std::endl;

  if (imp.prefix) {
    std::cout << "Prefix stmt: " << imp.prefix << std::endl;
  }

  return 0;
}
