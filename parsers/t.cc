#include "stmt_grammars.hpp"
#include <iostream>


int main() {
  using boost::spirit::x3::ascii::space;
  using yang_cpp::grammar::import_stmt;
  using yang_cpp::grammar::nspace_stmt;
  using yang_cpp::grammar::revision_stmt;

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
  std::cout << "Imported module: " << imp.module_name << std::endl;
  std::cout << "Module prefix: " << imp.prefix << std::endl;

  if (imp.revision_date) {
    std::cout << "Module revision date: " << imp.revision_date << std::endl;
  }

  input = "namespace \"http://test/yang\";";
  std::string ns;
  auto sitr = input.begin();
  std::cout << input << std::endl;

  res = phrase_parse(sitr, input.end(), nspace_stmt, space, ns);
  if (not res) {
    std::cout << *sitr << " : " << std::distance(input.begin(), sitr) << std::endl;
  }
  std::cout << "Namespace: " << ns << std::endl;

  input = "revision \"2017-03-20\" {"
          " description \"abc def\";"
          "}";

  yang_cpp::ast::revision_stmt_t rev_s;
  sitr = input.begin();
  res = phrase_parse(sitr, input.end(), revision_stmt, space, rev_s);

  std::cout << "Revision: " << rev_s.year << "-" << rev_s.month << "-" << rev_s.day << std::endl;
  std::cout << "Descirption: " << rev_s.description << std::endl;

  return 0;
}
