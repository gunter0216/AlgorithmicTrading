#ifndef SRC_LEAST_SQUARES_H_
#define SRC_LEAST_SQUARES_H_

#include <math.h>

#include "../other/matrix.h"
#include "abstract_formula.h"
#include "gauss.h"

class LeastSquares : public AbstractFormula {
 public:
  LeastSquares() = default;

  explicit LeastSquares(
      const std::pair<std::vector<double>, std::vector<double>> &data);

  ~LeastSquares() override = default;

  auto SetData(const std::pair<std::vector<double>, std::vector<double>> &data)
      -> void override;

  auto CalcValue(double x, int n) -> double;

  auto CreateGraph(std::pair<std::vector<double>, std::vector<double>> &result,
                   int degree, int size, int M) -> void;

 private:
  auto CalcCoef(std::vector<double> &coef, int degree) -> void;
  auto CalcValuePrivate(double x, int n) -> double;

  std::vector<double> coef;
};

#endif  // SRC_LEAST_SQUARES_H_
