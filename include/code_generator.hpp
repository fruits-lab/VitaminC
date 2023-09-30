#ifndef CODE_GENERATOR_HPP_
#define CODE_GENERATOR_HPP_

#include "visitor.hpp"

class CodeGenerator : public NonModifyingVisitor {
 public:
  void Visit(const DeclNode&) override;
  void Visit(const BlockStmtNode&) override;
  void Visit(const ProgramNode&) override;
  void Visit(const NullStmtNode&) override;
  void Visit(const ReturnStmtNode&) override;
  void Visit(const ExprStmtNode&) override;
  void Visit(const IdExprNode&) override;
  void Visit(const IntConstExprNode&) override;
  void Visit(const BinaryExprNode&) override;
  void Visit(const PlusExprNode&) override;
  void Visit(const SubExprNode&) override;
  void Visit(const MulExprNode&) override;
  void Visit(const DivExprNode&) override;
  void Visit(const ModExprNode&) override;
  void Visit(const GreaterThanExprNode&) override;
  void Visit(const GreaterThanOrEqualToExprNode&) override;
  void Visit(const LessThanExprNode&) override;
  void Visit(const LessThanOrEqualToExprNode&) override;
  void Visit(const EqualToExprNode&) override;
  void Visit(const NotEqualToExprNode&) override;
  void Visit(const SimpleAssignmentExprNode&) override;
};

#endif  // CODE_GENERATOR_HPP_
