#include "useredit.hpp"
#include "ui_useredit.h"
#define normal "background-color: rgb(181, 181, 181); \
border-radius:10px; \
color: rgb(255, 255, 255);"
#define wrong "color: rgb(255, 255, 255); \
border-radius:10px;  \
background-color: rgb(255, 62, 81);"

UserEdit::UserEdit(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::UserEdit)
{
  ui->setupUi(this);
}

UserEdit::~UserEdit() { delete ui; }

void UserEdit::closeEvent(QCloseEvent *) {
  emit disableToggle();
}

void UserEdit::edit(optional<User> *u) {
  User usr = u->value();
  user_->id = usr.id;
  user_->password = usr.password;
  user_->username = usr.username;
  ui->login->setPlaceholderText(usr.username);
}

void UserEdit::on_cancel_button_clicked() {
  this->close();
}

void UserEdit::on_okay_button_clicked() {
  QString l(ui->login->text()),cp(ui->cur_password->text()), p(ui->password->text()), c(ui->confirm->text());
  ui->pass_label->setStyleSheet(cp.isEmpty()?wrong:normal);
  ui->confirm_label->setStyleSheet(p.isEmpty()?wrong:normal);
  ui->curp_label->setStyleSheet(cp.isEmpty()?wrong:normal);
  user_->username=(l.isEmpty())?ui->login->placeholderText():l;
  if (ui->pass_label->styleSheet()==wrong||ui->confirm_label->styleSheet()==wrong||ui->curp_label->styleSheet()==wrong) return;
  else if (hashPassword(cp) != user_->password) {QMessageBox::warning(this, "Err","wrong current password", QMessageBox::Ok); return;}
  else if (p!=c){ ui->pass_label->setStyleSheet(wrong); ui->confirm_label->setStyleSheet(wrong); return;}
  user_->password = hashPassword(p);
  emit upd_user(user_);
  close();
}

