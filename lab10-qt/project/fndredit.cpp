#include "fndredit.hpp"
#include "ui_fndredit.h"

FndrEdit::FndrEdit(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::FndrEdit)
{
  ui->setupUi(this);
}

FndrEdit::~FndrEdit()
{
  delete ui;
}
void FndrEdit::on_okay_button_clicked() {
  Fndr* f = new Fndr();

  f->name = (ui->edit_name->text().isEmpty()) ?
        ui->edit_name->placeholderText():
        ui->edit_name->text();
  f->age = ui->edit_age->value();
  f->wealth = ui->edit_wealth->value();
  emit upd_fndr(f);
  close();
}

void FndrEdit::on_cancel_button_clicked() { close(); }

void FndrEdit::closeEvent(QCloseEvent* event) {
  QWidget::closeEvent(event);
  emit disableToggle();
}

void FndrEdit::edit_fndr(QListWidgetItem* item) {
  Fndr f = item->data(Qt::UserRole).value<Fndr>();
  ui->edit_name->setPlaceholderText(f.name);
  ui->edit_age->setValue(f.age);
  ui->edit_wealth->setValue(f.wealth);
}
