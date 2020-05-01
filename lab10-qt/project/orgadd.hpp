#pragma once

#include "organisation.h"

#include <QDebug>
#include <QListWidget>
#include <QMainWindow>
#include <QMessageBox>
#include <QDialog>

namespace Ui {
class OrgAdd;
}

class OrgAdd : public QDialog
{
  Q_OBJECT
signals:
  void org_add(Org*);
  void disableToggle();

public:
  explicit OrgAdd(QWidget *parent = nullptr);
  ~OrgAdd();

protected:
  void closeEvent(QCloseEvent*);
private slots:

  void on_okay_button_clicked();

  void on_cancer_button_clicked();

private:
  Ui::OrgAdd *ui;
};

