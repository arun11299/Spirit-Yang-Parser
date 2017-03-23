#include <iostream>
#include <string>
#include <boost/optional/optional_io.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/optional.hpp>

namespace x3 = boost::spirit::x3;

static int ping_cnt = 0;
static int pong_cnt = 0;

namespace parser {

  auto ping = lexeme[x3::lit("ping") >> x3::eol];
  auto pong = lexeme[x3::lit("pong") >> x3::eol];

  auto ping_counter = []{ ++ping_cnt; };
  auto pong_counter = []{ ++pong_cnt; };

  auto expr = ping[ping_counter] | pong[pong_counter];

}

int main() {
  using x3::space;
  using parser::expr;

  std::string input = "ping\n"
                      "pong\n"
                      "ping\n"
                      "pong\n";
  std::cout << input << std::endl;

  auto sitr = input.begin();
  while (sitr != input.end()) {
    bool res = phrase_parse(sitr, input.end(),
                            expr,
                            space);
    if (!res) {
      std::cout << "parsing failed: " << std::distance(input.begin(), sitr) << std::endl;
      return 1;
    }
  }

  std::cout << "Ping count = " << ping_cnt << std::endl;
  std::cout << "Pong count = " << pong_cnt << std::endl;

  return 0;
}
