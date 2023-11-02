#ifndef SRC_NEWTON_H_
#define SRC_NEWTON_H_

#include "../other/matrix.h"
#include "abstract_formula.h"

class Newton : public AbstractFormula {
 public:
  Newton() = default;

  explicit Newton(
      const std::pair<std::vector<double>, std::vector<double>> &data);

  ~Newton() override = default;

  auto SetData(const std::pair<std::vector<double>, std::vector<double>> &data)
      -> void override;

  auto CalcValue(double x, int n) -> double;

  auto CreateGraph(std::pair<std::vector<double>, std::vector<double>> &result,
                   int degree, int size) -> void;

 private:
  s21::Matrix<double> differenceTable_;

  auto CreateDifferenceTable() -> void;
  auto CalcValuePrivate(double x, int n) -> double;
};

#endif  // SRC_NEWTON_H_
