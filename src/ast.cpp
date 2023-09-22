#include "ast.hpp"

#include <fstream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "visitor.hpp"

/// @brief qbe intermediate file
extern std::ofstream output;

namespace {

/// @brief Returns the next local number and increment it by 1. The first number
/// will be 1.
int NextLocalNum() {
  /// @brief temporary index under a scope
  static int next_local_num = 1;
  return next_local_num++;
}

/// @note Use this as the return local number if the it's not expected to be
/// used, e.g., `StmtNode`.
const int kDummyLocalNum = -1;

/// @brief Returns the function-scope temporary with sigil (`%`).
std::string PrefixSigil(int local_num) {
  return "%." + std::to_string(local_num);
}

std::map<std::string, int> id_to_num{};

}  // namespace

void AstNode::Accept(NonModifyingVisitor& v) const {
  v.Visit(*this);
}

void AstNode::Accept(ModifyingVisitor& v) {
  v.Visit(*this);
}

void StmtNode::Accept(NonModifyingVisitor& v) const {
  v.Visit(*this);
}

void StmtNode::Accept(ModifyingVisitor& v) {
  v.Visit(*this);
}

void ExprNode::Accept(NonModifyingVisitor& v) const {
  v.Visit(*this);
}

void ExprNode::Accept(ModifyingVisitor& v) {
  v.Visit(*this);
}

void DeclNode::Accept(NonModifyingVisitor& v) const {
  v.Visit(*this);
}

void DeclNode::Accept(ModifyingVisitor& v) {
  v.Visit(*this);
}

int DeclNode::CodeGen() const {
  int id_num = NextLocalNum();
  output << PrefixSigil(id_num) << " =l alloc4 4" << std::endl;

  if (init_) {
    int init_num = init_->CodeGen();
    output << "storew " << PrefixSigil(init_num) << ", " << PrefixSigil(id_num)
           << std::endl;
  }
  // Set up the number of the id so we know were to load it back.
  id_to_num[id_] = id_num;
  return kDummyLocalNum;
}

void BlockStmtNode::Accept(NonModifyingVisitor& v) const {
  v.Visit(*this);
}

void BlockStmtNode::Accept(ModifyingVisitor& v) {
  v.Visit(*this);
}

int BlockStmtNode::CodeGen() const {
  output << "@start" << std::endl;
  for (const auto& decl : decls_) {
    decl->CodeGen();
  }
  for (const auto& stmt : stmts_) {
    stmt->CodeGen();
  }

  return kDummyLocalNum;
}

void ProgramNode::Accept(NonModifyingVisitor& v) const {
  v.Visit(*this);
}

void ProgramNode::Accept(ModifyingVisitor& v) {
  v.Visit(*this);
}

int ProgramNode::CodeGen() const {
  output << "export function w $main() {" << std::endl;
  block_->CodeGen();
  output << "}";

  return kDummyLocalNum;
}

void NullStmtNode::Accept(NonModifyingVisitor& v) const {
  v.Visit(*this);
}

void NullStmtNode::Accept(ModifyingVisitor& v) {
  v.Visit(*this);
}

int NullStmtNode::CodeGen() const {
  return kDummyLocalNum;
}

void ReturnStmtNode::Accept(NonModifyingVisitor& v) const {
  v.Visit(*this);
}

void ReturnStmtNode::Accept(ModifyingVisitor& v) {
  v.Visit(*this);
}

int ReturnStmtNode::CodeGen() const {
  int ret_num = expr_->CodeGen();
  output << " ret " << PrefixSigil(ret_num) << std::endl;
  return kDummyLocalNum;
}

void ExprStmtNode::Accept(NonModifyingVisitor& v) const {
  v.Visit(*this);
}

void ExprStmtNode::Accept(ModifyingVisitor& v) {
  v.Visit(*this);
}

int ExprStmtNode::CodeGen() const {
  expr_->CodeGen();

  return kDummyLocalNum;
}

void IdExprNode::Accept(NonModifyingVisitor& v) const {
  v.Visit(*this);
}

void IdExprNode::Accept(ModifyingVisitor& v) {
  v.Visit(*this);
}

int IdExprNode::CodeGen() const {
  /// @brief Plays the role of a "pointer". Its value has to be loaded to
  /// the register before use.
  int id_num = id_to_num.at(id_);
  int reg_num = NextLocalNum();
  output << PrefixSigil(reg_num) << " =w loadw " << PrefixSigil(id_num)
         << std::endl;
  return reg_num;
}

void IntConstExprNode::Accept(NonModifyingVisitor& v) const {
  v.Visit(*this);
}

void IntConstExprNode::Accept(ModifyingVisitor& v) {
  v.Visit(*this);
}

int IntConstExprNode::CodeGen() const {
  int num = NextLocalNum();
  output << PrefixSigil(num) << " =w copy " << val_ << std::endl;
  return num;
}

void BinaryExprNode::Accept(NonModifyingVisitor& v) const {
  v.Visit(*this);
}

void BinaryExprNode::Accept(ModifyingVisitor& v) {
  v.Visit(*this);
}

int BinaryExprNode::CodeGen() const {
  int left_num = lhs_->CodeGen();
  int right_num = rhs_->CodeGen();
  int num = NextLocalNum();
  output << PrefixSigil(num) << " =w " << OpName_() << " "
         << PrefixSigil(left_num) << ", " << PrefixSigil(right_num)
         << std::endl;

  return num;
}

void PlusExprNode::Accept(NonModifyingVisitor& v) const {
  v.Visit(*this);
}

void PlusExprNode::Accept(ModifyingVisitor& v) {
  v.Visit(*this);
}

std::string PlusExprNode::OpName_() const {
  return "add";
}

void SubExprNode::Accept(NonModifyingVisitor& v) const {
  v.Visit(*this);
}

void SubExprNode::Accept(ModifyingVisitor& v) {
  v.Visit(*this);
}

std::string SubExprNode::OpName_() const {
  return "sub";
}

void MulExprNode::Accept(NonModifyingVisitor& v) const {
  v.Visit(*this);
}

void MulExprNode::Accept(ModifyingVisitor& v) {
  v.Visit(*this);
}

std::string MulExprNode::OpName_() const {
  return "mul";
}

void DivExprNode::Accept(NonModifyingVisitor& v) const {
  v.Visit(*this);
}

void DivExprNode::Accept(ModifyingVisitor& v) {
  v.Visit(*this);
}

std::string DivExprNode::OpName_() const {
  return "div";
}

void ModExprNode::Accept(NonModifyingVisitor& v) const {
  v.Visit(*this);
}

void ModExprNode::Accept(ModifyingVisitor& v) {
  v.Visit(*this);
}

std::string ModExprNode::OpName_() const {
  return "rem";
}

void GreaterThanExprNode::Accept(NonModifyingVisitor& v) const {
  v.Visit(*this);
}

void GreaterThanExprNode::Accept(ModifyingVisitor& v) {
  v.Visit(*this);
}

std::string GreaterThanExprNode::OpName_() const {
  // signed
  return "sgt";
}

void GreaterThanOrEqualToExprNode::Accept(NonModifyingVisitor& v) const {
  v.Visit(*this);
}

void GreaterThanOrEqualToExprNode::Accept(ModifyingVisitor& v) {
  v.Visit(*this);
}

std::string GreaterThanOrEqualToExprNode::OpName_() const {
  // signed
  return "sge";
}

void LessThanExprNode::Accept(NonModifyingVisitor& v) const {
  v.Visit(*this);
}

void LessThanExprNode::Accept(ModifyingVisitor& v) {
  v.Visit(*this);
}

std::string LessThanExprNode::OpName_() const {
  // signed
  return "slt";
}

void LessThanOrEqualToExprNode::Accept(NonModifyingVisitor& v) const {
  v.Visit(*this);
}

void LessThanOrEqualToExprNode::Accept(ModifyingVisitor& v) {
  v.Visit(*this);
}

std::string LessThanOrEqualToExprNode::OpName_() const {
  // signed
  return "sle";
}

void EqualToExprNode::Accept(NonModifyingVisitor& v) const {
  v.Visit(*this);
}

void EqualToExprNode::Accept(ModifyingVisitor& v) {
  v.Visit(*this);
}

std::string EqualToExprNode::OpName_() const {
  return "eq";
}

void NotEqualToExprNode::Accept(NonModifyingVisitor& v) const {
  v.Visit(*this);
}

void NotEqualToExprNode::Accept(ModifyingVisitor& v) {
  v.Visit(*this);
}

std::string NotEqualToExprNode::OpName_() const {
  return "ne";
}

void AssignmentExprNode::Accept(NonModifyingVisitor& v) const {
  v.Visit(*this);
}

void AssignmentExprNode::Accept(ModifyingVisitor& v) {
  v.Visit(*this);
}

void SimpleAssignmentExprNode::Accept(NonModifyingVisitor& v) const {
  v.Visit(*this);
}

void SimpleAssignmentExprNode::Accept(ModifyingVisitor& v) {
  v.Visit(*this);
}

int SimpleAssignmentExprNode::CodeGen() const {
  int expr_num = expr_->CodeGen();
  output << "storew " << PrefixSigil(expr_num) << ", "
         << PrefixSigil(id_to_num.at(id_)) << std::endl;
  return expr_num;
}
