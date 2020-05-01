#pragma once

#include "founder.h"

#include <QDebug>
#include <QDialog>
#include <QListWidget>
#include <QMainWindow>
#include <QMessageBox>

namespace Ui {
class FndrEdit;
}

class FndrEdit : public QDialog
{
  Q_OBJECT
signals:
  void upd_fndr(Fndr*);
  void disableToggle();
protected:
  void closeEvent(QCloseEvent*);
private slots:

  void on_okay_button_clicked();

  void on_cancel_button_clicked();

  void edit_fndr(QListWidgetItem*);
public:
  explicit FndrEdit(QWidget *parent = nullptr);
  ~FndrEdit();

private:
  Ui::FndrEdit *ui;
};

