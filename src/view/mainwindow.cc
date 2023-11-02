#include "mainwindow.h"

#include <QDateTime>
#include <QSharedMemory>
#include <QString>
#include <QTimeEdit>
#include <QVector>
#include <fstream>
#include <memory>
#include <set>

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->centralwidget->setEnabled(false);
  ui->InterpolDock->hide();
  ui->ApproxDock->hide();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_ActionOpenStockQuotes_triggered() {
  QString filename =
      QFileDialog::getOpenFileName(this, "open file", "", "*.csv");
  if (!filename.isEmpty()) {
    QString tmp;
    QFile file(filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream ss(&file);
    ss >> tmp;
    if (tmp == "Date,Close") {
      stock_quotes_.first.clear();
      stock_quotes_.second.clear();
      while (!ss.atEnd()) {
        ss >> tmp;
        auto a = tmp.split(",");
        if (a.size() == 2) {
          stock_quotes_.first.push_back(static_cast<double>(
              QDateTime::fromString(a[0], "yyyy-MM-dd").toSecsSinceEpoch()));
          stock_quotes_.second.push_back(a[1].toDouble());
        } else {
          break;
        }
      }
      graphs_.clear();
    }
    auto chart_name = filename.split("/");
    chart_name = chart_name.back().split('.');
    DrawCharts(chart_name[0]);
    ui->predict_date_edit->setMinimumDateTime(
        QDateTime::fromSecsSinceEpoch(stock_quotes_.first.back()));
    ui->predict_date_edit->setDate(ui->predict_date_edit->minimumDate());
    ui->centralwidget->setEnabled(true);
    on_show_graph_btn_clicked();
    ui->predict_lb->clear();
  }
}

auto MainWindow::DrawCharts(QString &chart_name) -> void {
  auto draw_graph = [&](QCustomPlot *custom_plot) {
    custom_plot->legend->setVisible(true);
    custom_plot->legend->setBrush(QBrush{QColor(0, 0, 0, 0)});
    custom_plot->legend->setBorderPen(QPen{QColor{0, 0, 0, 0}});

    QVector<double> x{stock_quotes_.first.begin(), stock_quotes_.first.end()},
        y{stock_quotes_.second.begin(), stock_quotes_.second.end()};
    custom_plot->clearGraphs();
    custom_plot->addGraph();
    custom_plot->graph(0)->setName(chart_name);
    custom_plot->graph(0)->setData(x, y);
    custom_plot->xAxis->setRange(x.front(), x.back());
    auto mm = std::minmax_element(y.begin(), y.end());
    //    custom_plot->yAxis->setRange(*std::min(y.begin(), y.end()),
    //                                 *std::max(y.begin(), y.end()));
    custom_plot->yAxis->setRange(*mm.first, *mm.second);
    custom_plot->graph(0)->setPen(QPen(Qt::black, 2));
    QCPScatterStyle sstyle(QCPScatterStyle::ssCircle);
    sstyle.setPen(QPen(Qt::darkGray));
    custom_plot->graph(0)->setScatterStyle(sstyle);

    custom_plot->xAxis->setLabel("Date");
    custom_plot->yAxis->setLabel("Price");
    QSharedPointer<QCPAxisTickerDateTime> date_ticker(
        new QCPAxisTickerDateTime);
    date_ticker->setDateTimeFormat("d. MMM\nyyyy");
    custom_plot->xAxis->setTicker(date_ticker);

    custom_plot->replot();

    custom_plot->setInteraction(QCP::iRangeZoom, true);
    custom_plot->setInteraction(QCP::iRangeDrag, true);
  };
  draw_graph(ui->approx_plot);
  draw_graph(ui->interpol_plot);
}

void MainWindow::DrawGraphs() {
  if (graphs_.empty()) return;
  static int color = 7;
  if (color > 18) color = 7;
  ui->approx_plot->addGraph();
  QVector<double> x{graphs_[0].first.begin(), graphs_[0].first.end()};
  QVector<double> y{graphs_[0].second.begin(), graphs_[0].second.end()};
  ui->approx_plot->graph(1)->addData(x, y);
  ui->approx_plot->graph(1)->setPen(QPen{(Qt::GlobalColor)color++});

  ui->approx_plot->replot();
}

auto MainWindow::DrawGraph(
    QCustomPlot *custom_plot,
    std::pair<std::vector<double>, std::vector<double>> &graph,
    QString &graph_name, QColor &color) -> void {
  QVector<double> x{graph.first.begin(), graph.first.end()},
      y{graph.second.begin(), graph.second.end()};
  int graph_n = custom_plot->graphCount();
  custom_plot->addGraph();
  custom_plot->graph(graph_n)->setData(x, y);
  custom_plot->graph(graph_n)->setPen(QPen{color});
  custom_plot->graph(graph_n)->setName(graph_name);
  custom_plot->replot();
}

void MainWindow::on_show_graph_btn_clicked() {
  ui->ApproxDock->show();
  ui->InterpolDock->show();
}

void MainWindow::on_approx_btn_clicked() {
  if (!stock_quotes_.first.empty() and !stock_quotes_.second.empty()) {
    if (ui->approx_plot->graphCount() > 5) return;
    int M = 6, degree = 2, dots = 0;
    approx_dialog_.Run(degree, M, dots, (int)stock_quotes_.second.size());
    if (!dots or !degree) return;
    if (M != cur_approximation_days_) {
      cur_approximation_days_ = M;
      ClearGraphs_(ui->approx_plot);
    }
    ui->approx_plot->setStatusTip("Approximation  " + QString::number(M) +
                                  " days ahead");
    least_squares_.SetData(stock_quotes_);
    std::pair<std::vector<double>, std::vector<double>> graph;
    least_squares_.CreateGraph(graph, degree, dots, M);
    QString graph_name = "Approximation. Degree = ";
    graph_name += QString::number(degree);
    QColor color(rand());
    DrawGraph(ui->approx_plot, graph, graph_name, color);
  }
}

void MainWindow::on_Interpol_btn_clicked() {
  if (!stock_quotes_.first.empty() and !stock_quotes_.second.empty()) {
    if (ui->interpol_plot->graphCount() > 5) return;
    int degree = 0, dots = 0;
    interpol_dialog.Run(degree, dots, (int)stock_quotes_.second.size());
    if (dots) {
      std::pair<std::vector<double>, std::vector<double>> graph;
      QString graph_name;
      if (degree) {
        // newton
        newton_.SetData(stock_quotes_);
        newton_.CreateGraph(graph, degree, dots);
        graph_name = "Newton. Degree = " + QString::number(degree);
      } else {
        // cubic
        cubic_splines_.SetData(stock_quotes_);
        cubic_splines_.CreateGraph(graph, dots);
        graph_name = "Cubic Splines";
      }
      QColor color(rand());
      DrawGraph(ui->interpol_plot, graph, graph_name, color);
    }
  }
}

void MainWindow::on_del_approx_btn_clicked() {
  if (ui->approx_plot->graphCount() >= 2) ui->approx_plot->removeGraph(1);
  ui->approx_plot->replot();
  ui->approx_plot->setStatusTip("");
}

void MainWindow::on_del_Interpol_btn_clicked() {
  if (ui->interpol_plot->graphCount() >= 2) ui->interpol_plot->removeGraph(1);
  ui->interpol_plot->replot();
}

void MainWindow::on_predict_btn_clicked() {
  least_squares_.SetData(stock_quotes_);
  ui->predict_lb->setText(QString::number(least_squares_.CalcValue(
      (double)ui->predict_date_edit->dateTime().toSecsSinceEpoch(),
      ui->predict_degree_sb->value())));
}

auto MainWindow::ClearGraphs_(QCustomPlot *custom_plot) -> void {
  while (custom_plot->graphCount() > 1) {
    custom_plot->removeGraph(1);
  }
}
