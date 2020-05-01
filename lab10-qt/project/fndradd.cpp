#include "fndradd.hpp"
#include "ui_fndradd.h"

FndrAdd::FndrAdd(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::FndrAdd)
{
  ui->setupUi(this);
}

FndrAdd::~FndrAdd()
{
  delete ui;
}
void FndrAdd::on_okay_button_clicked() {
  Fndr* f = new Fndr();
  f->name = ui->edit_name->text();
  f->age = ui->edit_age->value();
  f->wealth = ui->edit_wealth->value();
  emit fndr_add(f);
  close();
}

void FndrAdd::on_cancer_button_clicked() { close(); }

void FndrAdd::closeEvent(QCloseEvent* event) {
  QWidget::closeEvent(event);
  emit disableToggle();
}
