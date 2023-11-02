#include "least_squares.h"

LeastSquares::LeastSquares(
    const std::pair<std::vector<double>, std::vector<double>> &data) {
  SetData(data);
}

auto LeastSquares::SetData(
    const std::pair<std::vector<double>, std::vector<double>> &data) -> void {
  if (data.first.empty()) throw std::invalid_argument("Data is empty.");
  if (data.first.size() != data.second.size())
    throw std::invalid_argument("Data is incorrect.");
  data_ = data;
  data_size_ = data_.first.size();
  NormalizeData();
}

auto LeastSquares::CalcValue(double x, int n) -> double {
  if (data_.first.empty()) throw std::invalid_argument("Data is empty.");
  if (n <= 0) throw std::invalid_argument("N should be more 0.");
  if (n >= (int)data_size_)
    throw std::invalid_argument("N should be less count points.");
  if (x < data_.first[0])
    throw std::invalid_argument("X should be more or eq then first element x.");

  return CalcValuePrivate((x - start_time_) / delta_time_, n);
}

auto LeastSquares::CalcValuePrivate(double x, int n) -> double {
  double result = 0;

  std::vector<double> coef;
  CalcCoef(coef, n);

  for (size_t i = 0; i < coef.size(); ++i) {
    result += coef[i] * pow(x, i);
  }

  return result;
}

auto LeastSquares::CalcCoef(std::vector<double> &coef, int degree) -> void {
  s21::Matrix<double> matrix{degree + 1, degree + 2};

  std::vector<double> sums(2 * (degree + 1));

  sums[0] = x_.size();
  for (size_t i = 1; i < sums.size(); ++i) {
    for (size_t j = 0; j < data_size_; ++j) {
      sums[i] += pow(x_[j], i);
    }
  }

  for (int i = 0; i < degree + 1; ++i) {
    for (int j = 0; j < degree + 1; ++j) {
      matrix[i][j] = sums[i + j];
    }
  }

  for (int i = 0; i < degree + 1; ++i) {
    for (size_t j = 0; j < data_size_; ++j) {
      matrix[i][degree + 1] += pow(x_[j], i) * data_.second[j];
    }
  }

  GaussAlgorithm gauss;
  gauss.SolveSoLE(matrix);
  coef = gauss.GetResult();
}

auto LeastSquares::CreateGraph(
    std::pair<std::vector<double>, std::vector<double>> &result, int degree,
    int size, int M) -> void {
  if (data_.first.empty()) throw std::invalid_argument("Data is empty.");
  if (size + M < (int)data_size_)
    throw std::invalid_argument("Size + M should be more then data size.");
  if (M < 0) throw std::invalid_argument("M should be more or eq 0.");
  if (degree <= 0) throw std::invalid_argument("Degree should be more 0.");
  if (degree >= (int)data_size_)
    throw std::invalid_argument("Degree should be less count points.");

  std::vector<double> coef;
  CalcCoef(coef, degree);

  result.first.clear();
  result.second.clear();
  result.first.resize(size + M);
  result.second.resize(size + M);
  double delta = (x_[data_size_ - 1] - x_[0] + M) / (double)(size - 1 + M);

  result.first[0] = 0;
  for (int i = 1; i < size + M; ++i) {
    result.first[i] = result.first[i - 1] + delta;
  }
  for (int i = 0; i < size + M; ++i) {
    double y = 0;
    for (size_t j = 0; j < coef.size(); ++j) {
      y += coef[j] * pow(result.first[i], j);
    }
    result.second[i] = y;
  }
  for (size_t i = 0; i < result.first.size(); ++i) {
    result.first[i] = (result.first[i] * delta_time_) + start_time_;
  }
}
