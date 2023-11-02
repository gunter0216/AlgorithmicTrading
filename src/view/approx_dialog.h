#ifndef APPROXDIALOG_H
#define APPROXDIALOG_H

#include <QDialog>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QWidget>

class ApproxDialog : public QDialog {
  Q_OBJECT
 public:
  explicit ApproxDialog(QWidget *parent = nullptr);
  auto Run(int &degree, int &days_after, int &dots, int min_dots) -> void;

 private:
  int *degree_ = nullptr;
  int *days_after_ = nullptr;
  int *dots_ = nullptr;
  QFormLayout form_layout_{this};
  QSpinBox degree_sb_{this};
  QSpinBox dots_sb_{this};
  QSpinBox days_sb_{this};
  QLabel degree_lb_{"Polynomial degree:", this};
  QLabel dots_lb_{"Dots:", this};
  QLabel days_lb_{"Days to add:", this};
  QPushButton ok_btn_{"OK", this};
 private slots:
  auto OkBtnClick() -> void;
};

#endif  // APPROXDIALOG_H
