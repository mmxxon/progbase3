#include "orgadd.hpp"
#include "ui_orgadd.h"

OrgAdd::OrgAdd(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::OrgAdd)
{
  ui->setupUi(this);
}

OrgAdd::~OrgAdd()
{
  delete ui;
}

void OrgAdd::on_okay_button_clicked() {
  Org* o = new Org();
  o->label = ui->edit_label->text();
  o->country = ui->edit_country->text();
  o->founded_date = ui->founded->value();
  emit org_add(o);
  close();
}

void OrgAdd::on_cancer_button_clicked() { close(); }

void OrgAdd::closeEvent(QCloseEvent* event) {
  QWidget::closeEvent(event);
  emit disableToggle();
}
