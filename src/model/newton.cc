#include "newton.h"

Newton::Newton(
    const std::pair<std::vector<double>, std::vector<double>> &data) {
  SetData(data);
}

auto Newton::SetData(
    const std::pair<std::vector<double>, std::vector<double>> &data) -> void {
  if (data.first.empty()) throw std::invalid_argument("Data is empty.");
  if (data.first.size() != data.second.size())
    throw std::invalid_argument("Data is incorrect.");
  data_ = data;
  data_size_ = data_.first.size();
  NormalizeData();
  CreateDifferenceTable();
}

auto Newton::CreateDifferenceTable() -> void {
  differenceTable_ = s21::Matrix<double>{(int)data_size_, (int)data_size_};
  for (int i = 0; i < differenceTable_.GetRows(); ++i) {
    differenceTable_[i][0] = data_.second[i];
  }

  for (int j = 0; j < differenceTable_.GetCols() - 1; ++j) {
    for (int i = j; i < differenceTable_.GetRows() - 1; ++i) {
      double temp = (differenceTable_[i + 1][j] - differenceTable_[i][j]) /
                    (x_[i + 1] - x_[i - j]);
      differenceTable_[i + 1][j + 1] = temp;
    }
  }
}

auto Newton::CalcValue(double x, int n) -> double {
  if (data_.first.empty()) throw std::invalid_argument("Data is empty.");
  if (n <= 0) throw std::invalid_argument("N should be more 0.");
  if (n >= (int)data_size_)
    throw std::invalid_argument("N should be less count points.");
  if (x < data_.first[0])
    throw std::invalid_argument("X should be more or eq then first element x.");
  if (x > data_.first[data_.first.size() - 1])
    throw std::invalid_argument("X should be less or eq then last element x.");
  return CalcValuePrivate((x - start_time_) / delta_time_, n);
}

auto Newton::CalcValuePrivate(double x, int n) -> double {
  int start_point = 0;
  while (start_point < (int)data_size_ && x > x_[start_point]) {
    ++start_point;
  }
  if (start_point <= (n + 1) / 2) {
    start_point = 0;
  } else if (data_size_ - 1 - (size_t)start_point < (size_t)(n + 1) / 2) {
    start_point = data_size_ - n - 1;
  } else {
    start_point -= (n + 1) / 2;
  }
  double result = 0;
  for (int i = 0; i < n + 1; ++i) {
    double temp = differenceTable_[i + start_point][i];
    for (int j = 0; j < i; ++j) {
      temp *= (x - x_[j + start_point]);
    }
    result += temp;
  }
  return result;
}

auto Newton::CreateGraph(
    std::pair<std::vector<double>, std::vector<double>> &result, int degree,
    int size) -> void {
  if (data_.first.empty()) throw std::invalid_argument("Data is empty.");
  if (size < (int)data_.first.size())
    throw std::invalid_argument(
        "Size new graph should be more or eq then data size.");
  if (degree <= 0) throw std::invalid_argument("N should be more 0.");
  if (degree >= (int)data_size_)
    throw std::invalid_argument("N should be less count points.");

  result.first.clear();
  result.second.clear();
  result.first.resize(size);
  result.second.resize(size);
  double delta = (x_[data_size_ - 1] - x_[0]) / (double)size;

  result.first[0] = x_[0];
  result.second[0] = data_.second[0];
  for (int i = 1; i < size; ++i) {
    result.first[i] = result.first[i - 1] + delta;
    result.second[i] = CalcValuePrivate(result.first[i], degree);
  }
  for (int i = 0; i < size; ++i) {
    result.first[i] = (result.first[i] * delta_time_) + start_time_;
  }
}