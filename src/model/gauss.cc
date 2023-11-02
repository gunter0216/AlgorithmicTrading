#include "gauss.h"

auto GaussAlgorithm::SolveSoLE(const s21::Matrix<double> &matrix) -> void {
  matrix_ = s21::Matrix<double>{matrix.GetRows(), matrix.GetCols()};
  for (int i = 0; i < matrix.GetRows(); ++i) {
    for (int j = 0; j < matrix.GetCols(); ++j) {
      matrix_[i][j] = matrix[i][j];
    }
  }
  result_ = std::vector<double>(matrix.GetRows());
  for (int i = 0; i < matrix_.GetRows(); ++i) {
    FindIndexAndSwap(i);
    NormalizeRow(i);
    SubRow(i);
  }
  ReverseSubstitution();
}

auto GaussAlgorithm::FindIndexAndSwap(int start_row) -> void {
  double max = std::abs(matrix_[start_row][start_row]);
  int index = start_row;
  for (int i = start_row + 1; i < matrix_.GetRows(); ++i) {
    if (max < std::abs(matrix_[i][start_row])) {
      max = std::abs(matrix_[i][start_row]);
      index = i;
    }
  }
  if (max < 1e-7) {
    throw std::invalid_argument("No solutions");
  }
  if (index != start_row) {
    for (int j = 0; j < matrix_.GetCols(); ++j) {
      std::swap(matrix_[start_row][j], matrix_[index][j]);
    }
  }
}

auto GaussAlgorithm::NormalizeRow(int start_row) -> void {
  for (int i = start_row; i < matrix_.GetRows(); ++i) {
    double temp = matrix_[i][start_row];
    if (std::abs(temp) < 1e-7) continue;
    for (int j = 0; j < matrix_.GetCols(); ++j) {
      matrix_[i][j] /= temp;
    }
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

auto GaussAlgorithm::NormalizeRowParallels(int i, double value) -> void {
  if (std::abs(value) < 1e-7) return;
  for (int j = 0; j < matrix_.GetCols(); ++j) {
    matrix_[i][j] /= value;
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

auto GaussAlgorithm::SubRow(int start_row) -> void {
  for (int i = start_row + 1; i < matrix_.GetRows(); ++i) {
    for (int j = 0; j < matrix_.GetCols(); ++j) {
      matrix_[i][j] -= matrix_[start_row][j];
    }
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

auto GaussAlgorithm::SubRowParallels(int i, int start_row) -> void {
  for (int j = 0; j < matrix_.GetCols(); ++j) {
    matrix_[i][j] -= matrix_[start_row][j];
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

auto GaussAlgorithm::ReverseSubstitution() -> void {
  for (int i = matrix_.GetRows() - 1; i >= 0; --i) {
    result_[i] = matrix_[i][matrix_.GetCols() - 1];
    for (int j = 0; j < i; ++j) {
      matrix_[j][matrix_.GetCols() - 1] -= matrix_[j][i] * result_[i];
    }
  }
}