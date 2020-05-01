#include "orgedit.hpp"
#include "ui_orgedit.h"

OrgEdit::OrgEdit(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::OrgEdit)
{
  ui->setupUi(this);
}

OrgEdit::~OrgEdit()
{
  delete ui;
}
void OrgEdit::on_okay_button_clicked() {
  Org* org = new Org();

  org->label = (ui->country->text().isEmpty()) ?
        ui->country->placeholderText():
        ui->country->text();
  org->country = (ui->country->text().isEmpty()) ?
        ui->country->placeholderText():
        ui->country->text();
  org->founded_date = ui->founded->value();
  emit upd_org(org);
  close();
}

void OrgEdit::on_cancel_button_clicked() { close(); }

void OrgEdit::closeEvent(QCloseEvent* event) {
  QWidget::closeEvent(event);
  emit disableToggle();
}

void OrgEdit::edit_org(QListWidgetItem* item) {
  Org org = item->data(Qt::UserRole).value<Org>();
  ui->label->setPlaceholderText(org.label);
  ui->country->setPlaceholderText(org.country);
  ui->founded->setValue(org.founded_date);
}
