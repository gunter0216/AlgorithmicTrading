#include <gtest/gtest.h>

#include "../model/cubic_splines.h"
#include "../model/least_squares.h"
#include "../model/newton.h"

TEST(newton, newton_1) {
  std::pair<std::vector<double>, std::vector<double>> data;
  data.first = {1, 2, 3, 4, 5};
  data.second = {1, 4, 9, 16, 25};
  Newton newton{data};
  newton.SetData(data);
  EXPECT_EQ(newton.CalcValue(1, 1), 1);
  EXPECT_EQ(newton.CalcValue(2, 1), 4);
  EXPECT_EQ(newton.CalcValue(3, 1), 9);
  EXPECT_EQ(newton.CalcValue(4, 1), 16);
  EXPECT_EQ(newton.CalcValue(5, 1), 25);
  EXPECT_EQ(newton.CalcValue(1, 2), 1);
  EXPECT_EQ(newton.CalcValue(2, 3), 4);
  EXPECT_EQ(newton.CalcValue(3, 4), 9);
  EXPECT_EQ(newton.CalcValue(4, 4), 16);
  EXPECT_EQ(newton.CalcValue(5, 4), 25);
  EXPECT_ANY_THROW(newton.CalcValue(1, 5));
  EXPECT_ANY_THROW(newton.CalcValue(1, -1));
  EXPECT_ANY_THROW(newton.CalcValue(6, 1));
  EXPECT_ANY_THROW(newton.CalcValue(0, 1));
  EXPECT_ANY_THROW(newton.CalcValue(-1, 1));
  EXPECT_ANY_THROW(newton.CalcValue(-1, -1));
  std::pair<std::vector<double>, std::vector<double>> new_data;
  newton.CreateGraph(new_data, 2, 10);
  EXPECT_EQ(new_data.first.size(), 10);
  EXPECT_ANY_THROW(newton.CreateGraph(new_data, 2, 4));
  EXPECT_ANY_THROW(newton.CreateGraph(new_data, 2, -1));
  EXPECT_ANY_THROW(newton.CreateGraph(new_data, 5, 6));
  EXPECT_ANY_THROW(newton.CreateGraph(new_data, -1, 5));
}

TEST(newton, newton_2) {
  Newton newton;
  std::pair<std::vector<double>, std::vector<double>> data1;
  EXPECT_ANY_THROW(newton.CalcValue(2, 2));
  EXPECT_ANY_THROW(newton.CreateGraph(data1, 3, 3));
  std::pair<std::vector<double>, std::vector<double>> data2;
  EXPECT_ANY_THROW(newton.SetData(data2));
  std::pair<std::vector<double>, std::vector<double>> data3;
  data3.first = {1, 2, 3};
  data3.second = {1, 2};
  EXPECT_ANY_THROW(newton.SetData(data3));
}

TEST(cubicsplines, cubicsplines_1) {
  std::pair<std::vector<double>, std::vector<double>> data;
  data.first = {1, 2, 3, 4, 5};
  data.second = {1, 4, 9, 16, 25};
  CubicSplines cubic_splines{data};
  cubic_splines.SetData(data);
  EXPECT_EQ(cubic_splines.CalcValue(1), 1);
  EXPECT_EQ(cubic_splines.CalcValue(2), 4);
  EXPECT_EQ(cubic_splines.CalcValue(3), 9);
  EXPECT_EQ(cubic_splines.CalcValue(4), 16);
  EXPECT_ANY_THROW(cubic_splines.CalcValue(6));
  EXPECT_ANY_THROW(cubic_splines.CalcValue(0));
  EXPECT_ANY_THROW(cubic_splines.CalcValue(-1));
  std::pair<std::vector<double>, std::vector<double>> new_data;
  cubic_splines.CreateGraph(new_data, 10);
  EXPECT_EQ(new_data.first.size(), 10);
  EXPECT_ANY_THROW(cubic_splines.CreateGraph(new_data, 4));
  EXPECT_ANY_THROW(cubic_splines.CreateGraph(new_data, -1));
}

TEST(cubicsplines, cubicsplines_2) {
  CubicSplines cubic_splines;
  std::pair<std::vector<double>, std::vector<double>> data1;
  EXPECT_ANY_THROW(cubic_splines.CalcValue(2));
  EXPECT_ANY_THROW(cubic_splines.CreateGraph(data1, 3));
  std::pair<std::vector<double>, std::vector<double>> data2;
  EXPECT_ANY_THROW(cubic_splines.SetData(data2));
  std::pair<std::vector<double>, std::vector<double>> data3;
  data3.first = {1, 2, 3};
  data3.second = {1, 2};
  EXPECT_ANY_THROW(cubic_splines.SetData(data3));
}

TEST(leastsquares, leastsquares_1) {
  std::pair<std::vector<double>, std::vector<double>> data;
  data.first = {1, 2, 3, 4, 5};
  data.second = {1, 4, 9, 16, 25};
  LeastSquares least_squares{data};
  least_squares.SetData(data);
  least_squares.CalcValue(1, 1);
  least_squares.CalcValue(2, 2);
  least_squares.CalcValue(3, 3);
  least_squares.CalcValue(4, 4);
  least_squares.CalcValue(5, 4);
  least_squares.CalcValue(6, 4);
  least_squares.CalcValue(7, 4);
  least_squares.CalcValue(10000, 4);
  EXPECT_ANY_THROW(least_squares.CalcValue(1, 5));
  EXPECT_ANY_THROW(least_squares.CalcValue(1, -1));
  EXPECT_ANY_THROW(least_squares.CalcValue(-1, 1));
  EXPECT_ANY_THROW(least_squares.CalcValue(-1, -1));
  std::pair<std::vector<double>, std::vector<double>> new_data;
  least_squares.CreateGraph(new_data, 2, 5, 10);
  EXPECT_EQ(new_data.first.size(), 15);
  EXPECT_ANY_THROW(least_squares.CreateGraph(new_data, 2, 7, -1));
  EXPECT_ANY_THROW(least_squares.CreateGraph(new_data, 2, 5, -1));
  EXPECT_ANY_THROW(least_squares.CreateGraph(new_data, -1, 5, 5));
  EXPECT_ANY_THROW(least_squares.CreateGraph(new_data, 6, 5, 6));
}

TEST(leastsquares, leastsquares_2) {
  LeastSquares least_squares;
  std::pair<std::vector<double>, std::vector<double>> data1;
  EXPECT_ANY_THROW(least_squares.CalcValue(2, 2));
  EXPECT_ANY_THROW(least_squares.CreateGraph(data1, 2, 5, 3));
  std::pair<std::vector<double>, std::vector<double>> data2;
  EXPECT_ANY_THROW(least_squares.SetData(data2));
  std::pair<std::vector<double>, std::vector<double>> data3;
  data3.first = {1, 2, 3};
  data3.second = {1, 2};
  EXPECT_ANY_THROW(least_squares.SetData(data3));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
