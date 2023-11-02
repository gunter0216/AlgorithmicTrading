#include "interpol_dialog.h"

InterpolDialog::InterpolDialog(QWidget *parent) : QDialog{parent} {
  connect(&ok_btn_, SIGNAL(clicked()), this, SLOT(OkBtnClick()));
  connect(&combo_box_, SIGNAL(currentIndexChanged(int)), this,
          SLOT(SetDegreeVisibility()));
  degree_sb_.setMinimum(1);
  degree_sb_.setMaximum(9);
  combo_box_.addItem("Newton polynomial");
  combo_box_.addItem("Cubic spline");
  layout_.addRow(&method_lb_, &combo_box_);
  layout_.addRow(&dots_lb_, &dots_sb_);
  layout_.addRow(&degree_lb_, &degree_sb_);
  layout_.addRow(&ok_btn_);
}

auto InterpolDialog::Run(int &degree, int &dots, int min_dots) -> void {
  degree_ = &degree;
  dots_ = &dots;
  *degree_ = 0;
  *dots_ = 0;
  dots_sb_.setMinimum(min_dots);
  dots_sb_.setMaximum(std::max(4 * min_dots,1000));
  exec();
}

auto InterpolDialog::OkBtnClick() -> void {
  if (!degree_sb_.isHidden()) *degree_ = degree_sb_.value();
  *dots_ = dots_sb_.value();
  close();
}

auto InterpolDialog::SetDegreeVisibility() -> void {
  degree_sb_.setHidden(combo_box_.currentIndex());
  degree_lb_.setHidden(combo_box_.currentIndex());
}
