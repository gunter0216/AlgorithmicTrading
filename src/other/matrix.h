#ifndef SRC_OTHER_MATRIX_H_
#define SRC_OTHER_MATRIX_H_

#include <climits>
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <string>

namespace s21 {
template <typename T>
class Matrix {
 public:
  //  Matrix();

  explicit Matrix(int rows = 3, int cols = 3);

  Matrix(const Matrix &other);

  Matrix(Matrix &&other) noexcept;

  Matrix(const std::initializer_list<std::initializer_list<T>> &list);

  explicit Matrix(const std::string &path);

  ~Matrix();

  int GetRows() const { return rows_; }

  int GetCols() const { return cols_; }

  int size() const { return GetRows(); }

  bool empty() const { return matrix_ == nullptr; }

  T **getMatrix();

  void clear();

  void LoadMatrixFromFile(const std::string &path);

  void swap(Matrix &other);

  void print(char separator = ' ') const;

  void RemoveRowColumn(int row, int col);

  Matrix<T> &operator=(const Matrix &other);

  Matrix<T> &operator=(Matrix &&other) noexcept;

  bool operator==(const Matrix &other);

  bool operator!=(const Matrix &other);

  T &operator()(int i, int j) const;

  T *operator[](int i) const;

 private:
  int rows_{}, cols_{};
  T **matrix_;
};
}  // namespace s21

template <typename T>
s21::Matrix<T>::Matrix(const std::string &path) : Matrix() {
  LoadMatrixFromFile(path);
}

// template <typename T>
// s21::Matrix<T>::Matrix() {
//     cols_ = 0;
//     rows_ = 0;
//     matrix_ = nullptr;
// }

template <typename T>
s21::Matrix<T>::Matrix(int rows, int cols) {
  cols_ = cols;
  rows_ = rows;
  matrix_ = new T *[rows_];
  for (int i = 0; i < rows_; i++) matrix_[i] = new T[cols_]{};
}

template <typename T>
s21::Matrix<T>::Matrix(const Matrix &other) {
  if (this != &other) {
    rows_ = other.rows_;
    cols_ = other.cols_;
    matrix_ = new T *[rows_];
    for (int i = 0; i < rows_; i++) {
      matrix_[i] = new T[cols_];
      for (int j = 0; j < cols_; j++) {
        matrix_[i][j] = other.matrix_[i][j];
      }
    }
  }
}

template <typename T>
s21::Matrix<T>::Matrix(Matrix &&other) noexcept {
  if (this != &other) {
    cols_ = other.cols_;
    rows_ = other.rows_;
    matrix_ = other.matrix_;
    other.cols_ = other.rows_ = 0;
    other.matrix_ = nullptr;
  }
}

template <typename T>
s21::Matrix<T>::Matrix(
    const std::initializer_list<std::initializer_list<T>> &list)
    : Matrix(list.size(), list.begin()->size()) {
  int i = 0, j = 0;
  for (auto item1 : list) {
    for (auto item2 : item1) {
      matrix_[i][j] = item2;
      ++j;
    }
    ++i;
    j = 0;
  }
}

template <typename T>
s21::Matrix<T>::~Matrix() {
  clear();
}

template <typename T>
void s21::Matrix<T>::LoadMatrixFromFile(const std::string &path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    throw std::invalid_argument("can't open file");
  }
  clear();
  file >> rows_;
  file >> cols_;
  if (rows_ > 0 && cols_ > 0) {
    matrix_ = new T *[rows_];
    for (int i = 0; i < rows_; ++i) {
      matrix_[i] = new T[cols_];
      for (int j = 0; j < cols_; ++j) {
        file >> matrix_[i][j];
      }
    }
  }
  file.close();
}

template <typename T>
T **s21::Matrix<T>::getMatrix() {
  return matrix_;
}

template <typename T>
void s21::Matrix<T>::clear() {
  if (matrix_ != nullptr) {
    for (int i = 0; i < rows_; ++i) delete[] matrix_[i];
    delete[] matrix_;
  }
  matrix_ = nullptr;
  rows_ = cols_ = 0;
}

template <typename T>
void s21::Matrix<T>::swap(Matrix &other) {
  std::swap(matrix_, other.matrix_);
  std::swap(cols_, other.cols_);
  std::swap(rows_, other.rows_);
}

template <typename T>
s21::Matrix<T> &s21::Matrix<T>::operator=(const Matrix &other) {
  if (this != &other) {
    clear();
    cols_ = other.cols_;
    rows_ = other.rows_;
    matrix_ = new T *[rows_];
    for (int i = 0; i < rows_; i++) {
      matrix_[i] = new T[cols_];
      for (int j = 0; j < cols_; j++) {
        matrix_[i][j] = other.matrix_[i][j];
      }
    }
  }
  return *this;
}

template <typename T>
s21::Matrix<T> &s21::Matrix<T>::operator=(Matrix &&other) noexcept {
  if (this != &other) {
    clear();
    cols_ = other.cols_;
    rows_ = other.rows_;
    matrix_ = other.matrix_;
    other.matrix_ = nullptr;
    other.cols_ = other.rows_ = 0;
  }
  return *this;
}

template <typename T>
bool s21::Matrix<T>::operator==(const Matrix &other) {
  bool result = true;
  if (other.cols_ != cols_ || other.rows_ != rows_) {
    result = false;
  } else {
    for (int i = 0; i < rows_ && result; ++i) {
      for (int j = 0; j < cols_ && result; ++j) {
        if (other.matrix_[i][j] != matrix_[i][j]) {
          result = false;
        }
      }
    }
  }
  return result;
}

template <typename T>
bool s21::Matrix<T>::operator!=(const Matrix &other) {
  return !(*this == other);
}

template <typename T>
T &s21::Matrix<T>::operator()(int i, int j) const {
  if (i >= rows_ || j >= cols_ || i < 0 || j < 0) {
    std::string exc = "Index was out of range, i = ";
    exc += std::to_string(i) + " j = " + std::to_string(j) + " when size is " +
           std::to_string(rows_) + "x" + std::to_string(cols_);
    throw std::out_of_range(exc);
  }
  return matrix_[i][j];
}

template <typename T>
T *s21::Matrix<T>::operator[](int i) const {
  return matrix_[i];
}

template <typename T>
void s21::Matrix<T>::print(char separator) const {
  if (matrix_ != nullptr) {
    std::cout << rows_ << " " << cols_ << std::endl;
    for (int i = 0; i < rows_; ++i) {
      for (int j = 0; j < cols_; ++j) {
        std::cout << matrix_[i][j] << separator;
      }
      std::cout << std::endl;
    }
  }
}

template <typename T>
void s21::Matrix<T>::RemoveRowColumn(int row, int col) {
  if (row > rows_ || col > cols_)
    throw std::out_of_range("Index was out of range");

  Matrix matrix(rows_ - 1, cols_ - 1);
  int i2 = 0;
  for (int i = 0; i < rows_; ++i) {
    if (i != row) {
      int j2 = 0;
      for (int j = 0; j < cols_; ++j) {
        if (j != col) {
          matrix[i2][j2] = (*this)[i][j];
          ++j2;
        }
      }
      ++i2;
    }
  }
  *this = matrix;
}

#endif  // SRC_OTHER_MATRIX_H_
