#ifndef UTIL_HPP_
#define UTIL_HPP_

#include <cstddef>
#include <string>

class Indenter {
 public:
  /// @brief A strong named type for the size of the indention.
  enum SizePerLevel : std::size_t;
  /// @brief A strong named type for the max level of indention.
  enum MaxLevel : std::size_t;

  /// @return `size_per_level` * `level` number of `symbol`s.
  std::string Indent() const;

  void IncreaseLevel();

  /// @note The indention level saturates at 0.
  void DecreaseLevel();

  /// @param symbol The symbol used to indent with.
  /// @param size_per_level The indention size. For example, if the size is `2`,
  /// each indention level adds 2 `symbol`s.
  /// @param max_level If equals to `0`, there is no limit.
  Indenter(char symbol, SizePerLevel size_per_level,
           MaxLevel max_level = MaxLevel{0})
      : symbol_{symbol},
        size_per_level_{size_per_level},
        max_level_{max_level} {}

 private:
  char symbol_;
  std::size_t size_per_level_;
  std::size_t max_level_;
  std::size_t level_{0};

  bool HasNoLevelLimit_() const;
};

#endif  // UTIL_HPP_
