#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <other/qcustomplot.h>

#include <QList>
#include <QMainWindow>
#include <map>
#include <set>
#include <vector>

#include "../model/cubic_splines.h"
#include "../model/least_squares.h"
#include "../model/newton.h"
#include "approx_dialog.h"
#include "interpol_dialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void on_ActionOpenStockQuotes_triggered();

  void on_show_graph_btn_clicked();

  void on_approx_btn_clicked();
  void on_del_approx_btn_clicked();
  void on_del_Interpol_btn_clicked();
  void on_Interpol_btn_clicked();
  void on_predict_btn_clicked();

 private:
  auto DrawCharts(QString &chart_name) -> void;
  auto DrawGraphs() -> void;
  auto DrawGraph(QCustomPlot *custom_plot,
                 std::pair<std::vector<double>, std::vector<double>> &graph,
                 QString &graph_name, QColor &color) -> void;
  auto ClearGraphs_(QCustomPlot *custom_plot) -> void;

  Ui::MainWindow *ui;
  std::pair<std::vector<double>, std::vector<double>> stock_quotes_;
  QList<std::pair<std::vector<double>, std::vector<double>>> graphs_;
  LeastSquares least_squares_;
  CubicSplines cubic_splines_;
  Newton newton_;
  ApproxDialog approx_dialog_;
  InterpolDialog interpol_dialog;
  int cur_approximation_days_ = 0;
};
#endif  // MAINWINDOW_H
