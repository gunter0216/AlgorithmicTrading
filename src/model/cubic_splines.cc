#include "cubic_splines.h"

CubicSplines::CubicSplines(
    const std::pair<std::vector<double>, std::vector<double>> &data) {
  SetData(data);
}

auto CubicSplines::SetData(
    const std::pair<std::vector<double>, std::vector<double>> &data) -> void {
  if (data.first.empty()) throw std::invalid_argument("Data is empty.");
  if (data.first.size() != data.second.size())
    throw std::invalid_argument("Data is incorrect.");
  data_ = data;
  data_size_ = data_.first.size();
  a.clear();
  b.clear();
  c.clear();
  d.clear();
  a.resize(data_size_ - 1);
  b.resize(data_size_ - 1);
  c.resize(data_size_ - 1);
  d.resize(data_size_ - 1);
  NormalizeData();
  CalcCoef();
}

auto CubicSplines::CreateTridiagonalMatrix(
    const std::vector<double> &h, s21::Matrix<double> &tridiagonalMatrix)
    -> void {
  for (size_t i = 0; i < data_size_ - 2; ++i) {
    tridiagonalMatrix[i][i] = 2 * (h[i] + h[i + 1]);
  }
  for (size_t i = 0; i < data_size_ - 3; ++i) {
    tridiagonalMatrix[i][i + 1] = h[i + 1];
    tridiagonalMatrix[i + 1][i] = h[i + 1];
  }
  for (size_t i = 1; i < data_size_ - 1; ++i) {
    tridiagonalMatrix[i - 1][data_size_ - 2] =
        (data_.second[i + 1] - data_.second[i]) * 6 / h[i] -
        (data_.second[i] - data_.second[i - 1]) * 6 / h[i - 1];
  }
}

auto CubicSplines::GaussElimination(s21::Matrix<double> &tridiagonalMatrix,
                                    std::vector<double> &temp) -> void {
  for (size_t i = 0; i < data_size_ - 3; ++i) {
    for (size_t j = i + 1; j < data_size_ - 2; ++j) {
      double t = tridiagonalMatrix[j][i] / tridiagonalMatrix[i][i];
      for (size_t k = 0; k < data_size_ - 1; ++k) {
        tridiagonalMatrix[j][k] -= t * tridiagonalMatrix[i][k];
      }
    }
  }
  for (int i = data_size_ - 3; i >= 0; --i) {
    temp[i] = tridiagonalMatrix[i][data_size_ - 2];
    for (int j = i + 1; j < (int)data_size_ - 2; ++j) {
      temp[i] -= tridiagonalMatrix[i][j] * temp[j];
    }
    temp[i] /= tridiagonalMatrix[i][i];
  }
}

auto CubicSplines::CalcCoef() -> void {
  std::vector<double> h(data_size_ - 1);
  for (size_t i = 0; i < data_size_ - 1; ++i) {
    h[i] = x_[i + 1] - x_[i];
  }
  s21::Matrix<double> tridiagonalMatrix{(int)(data_size_ - 2),
                                        (int)(data_size_ - 1)};
  std::vector<double> sig(data_size_);
  CreateTridiagonalMatrix(h, tridiagonalMatrix);
  GaussElimination(tridiagonalMatrix, sig);

  for (int i = data_size_ - 2; i >= 1; --i) {
    sig[i] = sig[i - 1];
  }
  sig[0] = 0;

  for (int i = 0; i < (int)data_size_ - 1; ++i) {
    a[i] = (sig[i + 1] - sig[i]) / (h[i] * 6.0);
    b[i] = sig[i] / 2.0;
    c[i] = (data_.second[i + 1] - data_.second[i]) / h[i] -
           h[i] * (2 * sig[i] + sig[i + 1]) / 6.0;
    d[i] = data_.second[i];
  }
}

auto CubicSplines::CreateGraph(
    std::pair<std::vector<double>, std::vector<double>> &result, int size)
    -> void {
  if (data_.first.empty()) throw std::invalid_argument("Data is empty.");
  if (size < (int)data_.first.size())
    throw std::invalid_argument(
        "Size new graph should be more or eq then data size.");

  result.first.clear();
  result.second.clear();
  result.first.resize(size);
  result.second.resize(size);

  double delta = (x_[data_size_ - 1] - x_[0]) / (double)size;

  result.first[0] = x_[0];
  result.second[0] = data_.second[0];
  for (int i = 1; i < size; ++i) {
    result.first[i] = result.first[i - 1] + delta;
    result.second[i] = CalcValuePrivate(result.first[i]);
  }
  for (int i = 0; i < size; ++i) {
    result.first[i] = (result.first[i] * delta_time_) + start_time_;
  }
}

auto CubicSplines::CalcValuePrivate(double x) -> double {
  int i = 0;
  while (i < (int)data_size_ - 1 && x >= x_[i]) {
    ++i;
  }
  --i;
  double dif = x - x_[i];
  return a[i] * pow(dif, 3) + b[i] * pow(dif, 2) + c[i] * (dif) + d[i];
}

auto CubicSplines::CalcValue(double x) -> double {
  if (data_.first.empty()) throw std::invalid_argument("Data is empty.");
  if (x < data_.first[0])
    throw std::invalid_argument("X should be more or eq then first element x.");
  if (x > data_.first[data_.first.size() - 1])
    throw std::invalid_argument("X should be less or eq then last element x.");

  return CalcValuePrivate((x - start_time_) / delta_time_);
}
