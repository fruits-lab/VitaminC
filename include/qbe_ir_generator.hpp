#ifndef QBE_IR_GENERATOR_HPP_
#define QBE_IR_GENERATOR_HPP_

#include "visitor.hpp"

class QbeIrGenerator : public NonModifyingVisitor {
 public:
  void Visit(const LoopInitNode&) override;
  void Visit(const DeclNode&) override;
  void Visit(const BlockStmtNode&) override;
  void Visit(const ProgramNode&) override;
  void Visit(const IfStmtNode&) override;
  void Visit(const WhileStmtNode&) override;
  void Visit(const ForStmtNode&) override;
  void Visit(const ReturnStmtNode&) override;
  void Visit(const BreakStmtNode&) override;
  void Visit(const ContinueStmtNode&) override;
  void Visit(const ExprStmtNode&) override;
  void Visit(const NullExprNode&) override;
  void Visit(const IdExprNode&) override;
  void Visit(const IntConstExprNode&) override;
  void Visit(const UnaryExprNode&) override;
  void Visit(const IncrExprNode&) override;
  void Visit(const DecrExprNode&) override;
  void Visit(const NegExprNode&) override;
  void Visit(const AddrExprNode&) override;
  void Visit(const DereferExprNode&) override;
  void Visit(const NotExprNode&) override;
  void Visit(const BitCompExprNode&) override;
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

#endif  // QBE_IR_GENERATOR_HPP_
