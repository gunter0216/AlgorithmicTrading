#ifndef A8_ABSTRACT_FORMULA_H
#define A8_ABSTRACT_FORMULA_H

#include <vector>

#include "../other/matrix.h"

class AbstractFormula {
 public:
  AbstractFormula() = default;
  virtual ~AbstractFormula() = default;
  virtual auto SetData(
      const std::pair<std::vector<double>, std::vector<double>> &data)
      -> void = 0;

 protected:
  std::pair<std::vector<double>, std::vector<double>> data_;
  std::vector<double> x_;
  size_t data_size_ = 0;
  double start_time_ = 0;
  double delta_time_ = 0;

  auto NormalizeData() -> void {
    x_.resize(data_size_);
    start_time_ = data_.first[0];
    delta_time_ = data_.first[1] - data_.first[0];
    for (size_t i = 0; i < data_size_; ++i) {
      x_[i] = (data_.first[i] - start_time_) / delta_time_;
    }
  }
};

#endif  // A8_ABSTRACT_FORMULA_H
