#ifndef TYPE_CHECKER_HPP_
#define TYPE_CHECKER_HPP_

#include "ast.hpp"
#include "scope.hpp"
#include "visitor.hpp"

/// @brief A modifying pass; resolves the type of expressions.
class TypeChecker : public ModifyingVisitor {
 public:
  TypeChecker(ScopeStack& env) : env_{env} {}

  void Visit(LoopInitNode&) override;
  void Visit(DeclNode&) override;
  void Visit(BlockStmtNode&) override;
  void Visit(ProgramNode&) override;
  void Visit(IfStmtNode&) override;
  void Visit(WhileStmtNode&) override;
  void Visit(ForStmtNode&) override;
  void Visit(ReturnStmtNode&) override;
  void Visit(BreakStmtNode&) override;
  void Visit(ContinueStmtNode&) override;
  void Visit(ExprStmtNode&) override;
  void Visit(NullExprNode&) override;
  void Visit(IdExprNode&) override;
  void Visit(IntConstExprNode&) override;
  void Visit(UnaryExprNode&) override;
  void Visit(IncrExprNode&) override;
  void Visit(DecrExprNode&) override;
  void Visit(NegExprNode&) override;
  void Visit(AddrExprNode&) override;
  void Visit(DereferExprNode&) override;
  void Visit(NotExprNode&) override;
  void Visit(BitCompExprNode&) override;
  void Visit(BinaryExprNode&) override;
  void Visit(PlusExprNode&) override;
  void Visit(SubExprNode&) override;
  void Visit(MulExprNode&) override;
  void Visit(DivExprNode&) override;
  void Visit(ModExprNode&) override;
  void Visit(GreaterThanExprNode&) override;
  void Visit(GreaterThanOrEqualToExprNode&) override;
  void Visit(LessThanExprNode&) override;
  void Visit(LessThanOrEqualToExprNode&) override;
  void Visit(EqualToExprNode&) override;
  void Visit(NotEqualToExprNode&) override;
  void Visit(SimpleAssignmentExprNode&) override;

 private:
  ScopeStack& env_;
};

#endif  // TYPE_CHECKER_HPP_
