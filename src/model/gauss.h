#ifndef SRC_MODEL_GAUSE_H_
#define SRC_MODEL_GAUSE_H_

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

#include "../other/matrix.h"

class GaussAlgorithm {
 public:
  GaussAlgorithm() = default;
  ~GaussAlgorithm() = default;

  auto SolveSoLE(const s21::Matrix<double> &matrix) -> void;
  auto GetResult() -> const std::vector<double> & { return result_; }

 private:
  s21::Matrix<double> matrix_;
  std::vector<double> result_;

  auto FindIndexAndSwap(int start_row) -> void;
  auto NormalizeRow(int start_row) -> void;
  auto NormalizeRowParallels(int i, double value) -> void;
  auto SubRow(int start_row) -> void;
  auto SubRowParallels(int i, int start_row) -> void;
  auto ReverseSubstitution() -> void;
};

#endif  // SRC_MODEL_GAUSE_H_
