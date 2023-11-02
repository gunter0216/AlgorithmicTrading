#include "approx_dialog.h"

ApproxDialog::ApproxDialog(QWidget *parent) : QDialog{parent} {
  connect(&ok_btn_, SIGNAL(clicked()), this, SLOT(OkBtnClick()));
  degree_sb_.setMinimum(1);
  degree_sb_.setMaximum(60);
  days_sb_.setMaximum(100);
  days_sb_.setMinimum(0);
  form_layout_.addRow(&degree_lb_, &degree_sb_);
  form_layout_.addRow(&days_lb_, &days_sb_);
  form_layout_.addRow(&dots_lb_, &dots_sb_);
  form_layout_.addRow(&ok_btn_);
}

auto ApproxDialog::Run(int &degree, int &days_after, int &dots, int min_dots)
    -> void {
  degree_ = &degree;
  days_after_ = &days_after;
  dots_ = &dots;
  *degree_ = 0;
  *days_after_ = 0;
  *dots_ = 0;
  dots_sb_.setMinimum(min_dots);
  dots_sb_.setMaximum(std::max(4 * min_dots,1000));
  exec();
}

auto ApproxDialog::OkBtnClick() -> void {
  *degree_ = degree_sb_.value();
  *dots_ = dots_sb_.value();
  *days_after_ = days_sb_.value();
  close();
}
