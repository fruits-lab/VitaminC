#include "ast_dumper.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <variant>

#include "ast.hpp"
#include "operator.hpp"
#include "type.hpp"

namespace {

std::string GetBinaryOperator(BinaryOperator op) {
  switch (op) {
    case BinaryOperator::kAdd:
      return "+";
    case BinaryOperator::kSub:
      return "-";
    case BinaryOperator::kMul:
      return "*";
    case BinaryOperator::kDiv:
      return "/";
    case BinaryOperator::kMod:
      return "%";
    case BinaryOperator::kGt:
      return ">";
    case BinaryOperator::kGte:
      return ">=";
    case BinaryOperator::kLt:
      return "<";
    case BinaryOperator::kLte:
      return "<=";
    case BinaryOperator::kEq:
      return "==";
    case BinaryOperator::kNeq:
      return "!=";
    default:
      return "Unknown";
  }
}

std::string GetUnaryOperator(UnaryOperator op) {
  switch (op) {
    case UnaryOperator::kIncr:
      return "++";
    case UnaryOperator::kDecr:
      return "--";
    case UnaryOperator::kPos:
      return "+";
    case UnaryOperator::kNeg:
      return "-";
    case UnaryOperator::kNot:
      return "!";
    case UnaryOperator::kAddr:
      return "&";
    case UnaryOperator::kDeref:
      return "*";
    case UnaryOperator::kBitComp:
      return "~";
    default:
      return "Unknown";
  }
}

}  // namespace

void AstDumper::Visit(const DeclNode& decl) {
  std::cout << indenter_.Indent() << "DeclNode " << decl.id << ": "
            << TypeToString(decl.type) << '\n';

  if (decl.init) {
    indenter_.IncreaseLevel();
    decl.init->Accept(*this);
    indenter_.DecreaseLevel();
  }
}

void AstDumper::Visit(const ParamNode& parameter) {
  std::cout << indenter_.Indent() << "ParamNode " << parameter.id << ": "
            << TypeToString(parameter.type) << '\n';
}

void AstDumper::Visit(const FuncDefNode& func_def) {
  std::cout << indenter_.Indent() << "FuncDefNode " << func_def.id << ": "
            << TypeToString(func_def.type) << '\n';

  indenter_.IncreaseLevel();
  for (const auto& parameter : func_def.parameters) {
    parameter->Accept(*this);
  }
  func_def.body->Accept(*this);
  indenter_.DecreaseLevel();
}

void AstDumper::Visit(const LoopInitNode& loop_init) {
  std::cout << indenter_.Indent() << "LoopInitNode\n";
  indenter_.IncreaseLevel();
  std::visit([this](auto&& clause) { clause->Accept(*this); },
             loop_init.clause);
  indenter_.DecreaseLevel();
}

void AstDumper::Visit(const CompoundStmtNode& compound_stmt) {
  std::cout << indenter_.Indent() << "CompoundStmtNode\n";
  indenter_.IncreaseLevel();
  for (const auto& item : compound_stmt.items) {
    std::visit([this](auto&& item) { item->Accept(*this); }, item);
  }
  indenter_.DecreaseLevel();
}

void AstDumper::Visit(const ProgramNode& program) {
  std::cout << indenter_.Indent() << "ProgramNode\n";
  indenter_.IncreaseLevel();
  for (const auto& func_def : program.func_def_list) {
    func_def->Accept(*this);
  }
  indenter_.DecreaseLevel();
}

void AstDumper::Visit(const IfStmtNode& if_stmt) {
  std::cout << indenter_.Indent() << "IfStmtNode\n";
  indenter_.IncreaseLevel();
  if_stmt.predicate->Accept(*this);
  std::cout << indenter_.Indent() << "// Then\n";
  if_stmt.then->Accept(*this);
  indenter_.DecreaseLevel();
  if (if_stmt.or_else) {
    indenter_.IncreaseLevel();
    std::cout << indenter_.Indent() << "// Else\n";
    if_stmt.or_else->Accept(*this);
    indenter_.DecreaseLevel();
  }
}

void AstDumper::Visit(const WhileStmtNode& while_stmt) {
  std::cout << indenter_.Indent() << "WhileStmtNode\n";
  if (while_stmt.is_do_while) {
    indenter_.IncreaseLevel();
    std::cout << indenter_.Indent() << "// Do\n";
    while_stmt.loop_body->Accept(*this);
    indenter_.DecreaseLevel();
  }
  indenter_.IncreaseLevel();
  std::cout << indenter_.Indent() << "// While\n";
  while_stmt.predicate->Accept(*this);
  if (!while_stmt.is_do_while) {
    std::cout << indenter_.Indent() << "// Body\n";
    while_stmt.loop_body->Accept(*this);
  }
  indenter_.DecreaseLevel();
}

void AstDumper::Visit(const ForStmtNode& for_stmt) {
  std::cout << indenter_.Indent() << "ForStmtNode\n";
  indenter_.IncreaseLevel();
  for_stmt.loop_init->Accept(*this);
  for_stmt.predicate->Accept(*this);
  for_stmt.step->Accept(*this);
  for_stmt.loop_body->Accept(*this);
  indenter_.DecreaseLevel();
}

void AstDumper::Visit(const ReturnStmtNode& ret_stmt) {
  std::cout << indenter_.Indent() << "ReturnStmtNode\n";
  indenter_.IncreaseLevel();
  ret_stmt.expr->Accept(*this);
  indenter_.DecreaseLevel();
}

void AstDumper::Visit(const GotoStmtNode& goto_stmt) {
  std::cout << indenter_.Indent() << "GotoStmtNode " << goto_stmt.label << '\n';
}

void AstDumper::Visit(const BreakStmtNode& break_stmt) {
  std::cout << indenter_.Indent() << "BreakStmtNode\n";
}

void AstDumper::Visit(const ContinueStmtNode& continue_stmt) {
  std::cout << indenter_.Indent() << "ContinueStmtNode\n";
}

void AstDumper::Visit(const SwitchStmtNode& switch_stmt) {
  std::cout << indenter_.Indent() << "SwitchStmtNode\n";
  indenter_.IncreaseLevel();
  switch_stmt.ctrl->Accept(*this);
  switch_stmt.stmt->Accept(*this);
  indenter_.DecreaseLevel();
}

void AstDumper::Visit(const IdLabeledStmtNode& id_labeled_stmt) {
  std::cout << indenter_.Indent() << "IdLabeledStmtNode "
            << id_labeled_stmt.label << '\n';
  indenter_.IncreaseLevel();
  id_labeled_stmt.stmt->Accept(*this);
  indenter_.DecreaseLevel();
}

void AstDumper::Visit(const CaseStmtNode& case_stmt) {
  std::cout << indenter_.Indent() << "CaseStmtNode\n";
  indenter_.IncreaseLevel();
  case_stmt.expr->Accept(*this);
  case_stmt.stmt->Accept(*this);
  indenter_.DecreaseLevel();
}

void AstDumper::Visit(const DefaultStmtNode& default_stmt) {
  std::cout << indenter_.Indent() << "DefaultStmtNode\n";
  indenter_.IncreaseLevel();
  default_stmt.stmt->Accept(*this);
  indenter_.DecreaseLevel();
}

void AstDumper::Visit(const ExprStmtNode& expr_stmt) {
  std::cout << indenter_.Indent() << "ExprStmtNode\n";
  indenter_.IncreaseLevel();
  expr_stmt.expr->Accept(*this);
  indenter_.DecreaseLevel();
}

void AstDumper::Visit(const NullExprNode& null_expr) {
  std::cout << indenter_.Indent() << "NullStmtNode\n";
}

void AstDumper::Visit(const IdExprNode& id_expr) {
  std::cout << indenter_.Indent() << "IdExprNode " << id_expr.id << ": "
            << TypeToString(id_expr.type) << '\n';
}

void AstDumper::Visit(const IntConstExprNode& int_expr) {
  std::cout << indenter_.Indent() << "IntConstExprNode " << int_expr.val << ": "
            << TypeToString(int_expr.type) << '\n';
}

void AstDumper::Visit(const ArgExprNode& arg_expr) {
  std::cout << indenter_.Indent() << "ArgExprNode "
            << TypeToString(arg_expr.type) << '\n';
  indenter_.IncreaseLevel();
  arg_expr.arg->Accept(*this);
  indenter_.DecreaseLevel();
}

void AstDumper::Visit(const FuncCallExprNode& call_expr) {
  std::cout << indenter_.Indent() << "FuncCallExprNode "
            << TypeToString(call_expr.type) << '\n';
  indenter_.IncreaseLevel();
  call_expr.func_expr->Accept(*this);
  for (const auto& arg : call_expr.args) {
    arg->Accept(*this);
  }
  indenter_.DecreaseLevel();
}

void AstDumper::Visit(const UnaryExprNode& unary_expr) {
  std::cout << indenter_.Indent() << "UnaryExprNode "
            << TypeToString(unary_expr.type) << " "
            << GetUnaryOperator(unary_expr.op) << '\n';
  indenter_.IncreaseLevel();
  unary_expr.operand->Accept(*this);
  indenter_.DecreaseLevel();
}

void AstDumper::Visit(const BinaryExprNode& bin_expr) {
  std::cout << indenter_.Indent() << "BinaryExprNode "
            << TypeToString(bin_expr.type) << " "
            << GetBinaryOperator(bin_expr.op) << '\n';
  indenter_.IncreaseLevel();
  bin_expr.lhs->Accept(*this);
  bin_expr.rhs->Accept(*this);
  indenter_.DecreaseLevel();
}

void AstDumper::Visit(const SimpleAssignmentExprNode& assign_expr) {
  std::cout << indenter_.Indent() << "SimpleAssignmentExprNode "
            << TypeToString(assign_expr.type) << '\n';
  indenter_.IncreaseLevel();
  assign_expr.lhs->Accept(*this);
  assign_expr.rhs->Accept(*this);
  indenter_.DecreaseLevel();
}
