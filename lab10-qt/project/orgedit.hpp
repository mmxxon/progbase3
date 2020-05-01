#pragma once
#include "organisation.h"

#include <QDebug>
#include <QDialog>
#include <QListWidget>
#include <QMainWindow>
#include <QMessageBox>

namespace Ui {
class OrgEdit;
}

class OrgEdit : public QDialog
{
  Q_OBJECT
signals:
  void upd_org(Org*);
  void disableToggle();
protected:
  void closeEvent(QCloseEvent*);
private slots:

  void on_okay_button_clicked();

  void on_cancel_button_clicked();

  void edit_org(QListWidgetItem*);

public:
  explicit OrgEdit(QWidget *parent = nullptr);
  ~OrgEdit();

private:
  Ui::OrgEdit *ui;
};

