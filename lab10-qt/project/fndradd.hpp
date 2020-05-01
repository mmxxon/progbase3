#pragma once

#include <QDialog>
#include "founder.h"

#include <QDebug>
#include <QListWidget>
#include <QMainWindow>
#include <QMessageBox>
#include <QDialog>
namespace Ui {
class FndrAdd;
}

class FndrAdd : public QDialog
{
  Q_OBJECT
signals:
  void fndr_add(Fndr*);
  void disableToggle();
public:
  explicit FndrAdd(QWidget *parent = nullptr);
  ~FndrAdd();

protected:
  void closeEvent(QCloseEvent*);
private slots:

  void on_okay_button_clicked();

  void on_cancer_button_clicked();
private:
  Ui::FndrAdd *ui;
};

