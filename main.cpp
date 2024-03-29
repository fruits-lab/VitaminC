#include <cstdlib>
#include <cxxopts.hpp>
#include <fstream>
#include <iostream>
#include <memory>

#include "ast.hpp"
#include "ast_dumper.hpp"
#include "qbe_ir_generator.hpp"
#include "scope.hpp"
#include "type_checker.hpp"
#include "util.hpp"
#include "y.tab.hpp"

extern void yylex_destroy();  // NOLINT(readability-identifier-naming): extern
                              // from flex generated code.

int main(  // NOLINT(bugprone-exception-escape): Using a big try-catch block to
           // catch all exceptions isn't reasonable.
    int argc, char** argv)

{
  auto cmd_options = cxxopts::Options{
      argv[0],  // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic):
                // std::span is available in C++20.
      "A simple C compiler."};
  // clang-format off
  cmd_options.add_options()
      ("o, output", "Write output to <file>", cxxopts::value<std::string>()->default_value("test.ssa"), "<file>")
      ("d, dump", "Dump the abstract syntax tree", cxxopts::value<bool>()->default_value("false"))
      ("h, help", "Display available options")
      ;
  // clang-format on

  auto args = cmd_options.parse(argc, argv);
  if (args.count("help")) {
    std::cerr << cmd_options.help() << '\n';
    std::exit(0);
  }

  auto output = std::ofstream{args["output"].as<std::string>()};
  /// @brief The root node of the program.
  auto program = std::unique_ptr<AstNode>{};
  yy::parser parser{program};
  int ret = parser.parse();

  yylex_destroy();

  // 0 on success, 1 otherwise
  if (ret) {
    return ret;
  }

  // perform analyses and transformations on the ast
  auto scopes = ScopeStack{};
  TypeChecker type_checker{scopes};
  program->Accept(type_checker);
  if (args["dump"].as<bool>()) {
    const auto max_level = 80u;
    AstDumper ast_dumper{Indenter{' ', Indenter::SizePerLevel{2},
                                  Indenter::MaxLevel{max_level}}};
    program->Accept(ast_dumper);
  }
  QbeIrGenerator code_generator{output};
  program->Accept(code_generator);

  output.close();

  return 0;
}
