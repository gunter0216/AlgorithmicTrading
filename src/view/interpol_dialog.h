#ifndef INTERPOLDIALOG_H
#define INTERPOLDIALOG_H

#include <QComboBox>
#include <QDialog>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QWidget>

class InterpolDialog : public QDialog {
  Q_OBJECT
 public:
  explicit InterpolDialog(QWidget *parent = nullptr);
  auto Run(int &degree, int &dots, int min_dots) -> void;

 private:
  int *degree_ = nullptr;
  int *dots_ = nullptr;
  QFormLayout layout_{this};
  QComboBox combo_box_{this};
  QSpinBox degree_sb_{this};
  QSpinBox dots_sb_{this};
  QLabel method_lb_{"Method:", this};
  QLabel degree_lb_{"Polynomial degree:", this};
  QLabel dots_lb_{"Dots:", this};
  QPushButton ok_btn_{"OK", this};
 private slots:
  auto OkBtnClick() -> void;
  auto SetDegreeVisibility() -> void;
};

#endif  // INTERPOLDIALOG_H
