#ifndef SCOPE_HPP_
#define SCOPE_HPP_

#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "symbol.hpp"

/// @brief Manages scopes and symbol tables.
class ScopeStack {
 public:
  void PushScope() {
    scopes_.push_back(std::make_unique<SymbolTable>());
  }

  /// @throws `NotInScopeError`
  void PopScope() {
    TopScope_();  // ensure in scope
    scopes_.pop_back();
  }

  /// @brief Adds an entry to the top-most scope.
  /// @throws `NotInScopeError`
  std::shared_ptr<SymbolEntry> Add(std::unique_ptr<SymbolEntry> entry) {
    return TopScope_().Add(std::move(entry));
  }

  /// @brief Looks up the `id` from through all scopes.
  /// @throws `NotInScopeError`
  std::shared_ptr<SymbolEntry> LookUp(const std::string& id) const {
    TopScope_();  // ensure in scope
    // Iterates backward since we're using the container as a stack.
    for (auto it = scopes_.crbegin(); it != scopes_.crend(); ++it) {
      if (auto entry = (*it)->Probe(id)) {
        return entry;
      }
    }
    return nullptr;
  }

  /// @brief Probes the `id` from the top-most scope.
  /// @throws `NotInScopeError`
  std::shared_ptr<SymbolEntry> Probe(const std::string& id) const {
    return TopScope_().Probe(id);
  }

  using NotInScopeError = std::runtime_error;

 private:
  std::vector<std::unique_ptr<SymbolTable>> scopes_{};

  /// @throws `NotInScopeError`
  SymbolTable& TopScope_() const {
    if (scopes_.empty()) {
      throw NotInScopeError{""};
    } else {
      return *scopes_.back();
    }
  }
};

#endif  // SCOPE_HPP_
