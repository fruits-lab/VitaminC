#include "llvm/util.hpp"

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>

#include <cassert>
#include <memory>
#include <string>
#include <vector>

#include "type.hpp"

using namespace util;

llvm::IntegerType* LLVMIRUtil::IntType() {
  return builder_->getInt32Ty();
}

llvm::PointerType* LLVMIRUtil::IntPtrType() {
  return builder_->getPtrTy();
}

void LLVMIRUtil::CreateBrIfNoBrBefore(llvm::BasicBlock* next_bb) {
  auto bb = builder_->GetInsertBlock();
  bool has_terminator = false;
  for (auto it = bb->begin(); it != bb->end();) {
    if (it->isTerminator()) {
      has_terminator = true;
      break;
    } else {
      ++it;
    }
  }

  if (!has_terminator) {
    builder_->CreateBr(next_bb);
  }
}

void LLVMIRUtil::CurrBBFallThroughNextBB(llvm::BasicBlock* curr_bb,
                                         llvm::BasicBlock* next_bb) {
  auto bb = curr_bb;
  bool has_terminator = false;
  for (auto it = bb->begin(); it != bb->end();) {
    if (it->isTerminator()) {
      has_terminator = true;
      break;
    } else {
      ++it;
    }
  }

  if (!has_terminator) {
    builder_->SetInsertPoint(curr_bb);
    builder_->CreateBr(next_bb);
  }
}

llvm::BasicBlock* LLVMIRUtil::FindBBWithNameOf(const std::string& id) {
  auto func = builder_->GetInsertBlock()->getParent();
  for (auto& bb_iter : *func) {
    if (bb_iter.getName() == id) {
      return &(bb_iter);
    }
  }

  return nullptr;
}

llvm::Function* LLVMIRUtil::CurrFunc() {
  return builder_->GetInsertBlock()->getParent();
}

llvm::Type* LLVMIRUtil::GetLLVMType(const std::unique_ptr<Type>& type) {
  if (type->IsPtr()) {
    // TODO: If type's base_type() IsFunc(), then return
    // GetLLVMType(base_type()). Otherwise, return pointer to base_type() type.
    auto ptr_type = dynamic_cast<PtrType*>(type.get());
    assert(ptr_type);
    return IntPtrType();
  } else if (type->IsArr()) {
    auto arr_type = dynamic_cast<ArrType*>(type.get());
    return llvm::ArrayType::get(IntType(), arr_type->len());
  } else if (type->IsStruct() || type->IsUnion()) {
    std::string record_prefix = type->IsStruct() ? "struct_" : "union_";
    auto record_type = dynamic_cast<RecordType*>(type.get());
    std::vector<llvm::Type*> field_types;
    for (auto& field : record_type->fields()) {
      field_types.push_back(GetLLVMType(field->type));
    }

    return llvm::StructType::create(builder_->getContext(), field_types,
                                    record_prefix + record_type->id());
  } else if (type->IsFunc()) {
    auto func_type = dynamic_cast<FuncType*>(type.get());
    auto return_type = GetLLVMType(func_type->return_type());

    std::vector<llvm::Type*> param_types;
    for (auto& param_type : func_type->param_types()) {
      param_types.push_back(GetLLVMType(param_type));
    }

    return llvm::FunctionType::get(return_type, param_types, false);
  }

  return IntType();
}
