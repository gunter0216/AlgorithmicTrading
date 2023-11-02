#ifndef SRC_CUBICSPLINES_H_
#define SRC_CUBICSPLINES_H_

#include <math.h>

#include "../other/matrix.h"
#include "abstract_formula.h"

class CubicSplines : public AbstractFormula {
 public:
  CubicSplines() = default;
  explicit CubicSplines(
      const std::pair<std::vector<double>, std::vector<double>> &data);
  ~CubicSplines() override = default;

  auto CalcValue(double x) -> double;

  auto SetData(const std::pair<std::vector<double>, std::vector<double>> &data)
      -> void override;

  auto CreateGraph(std::pair<std::vector<double>, std::vector<double>> &result,
                   int size) -> void;

 private:
  auto CalcCoef() -> void;
  auto CreateTridiagonalMatrix(const std::vector<double> &h,
                               s21::Matrix<double> &tridiagonalMatrix) -> void;
  auto GaussElimination(s21::Matrix<double> &tridiagonalMatrix,
                        std::vector<double> &temp) -> void;
  auto CalcValuePrivate(double x) -> double;

  s21::Matrix<double> matrix_;
  std::vector<double> a;
  std::vector<double> b;
  std::vector<double> c;
  std::vector<double> d;
};

#endif  // SRC_CUBICSPLINES_H_
