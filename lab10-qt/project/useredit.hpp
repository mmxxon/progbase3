#pragma once
#include "sqlite_storage.h"
#include "user.h"
#include <QDialog>
#include <QListWidget>
#include <QMessageBox>

namespace Ui {
class UserEdit;
}

class UserEdit : public QDialog {
  Q_OBJECT
signals:
  void upd_user(User *);
  void disableToggle();

protected:
  void closeEvent(QCloseEvent *);
private slots:
  void edit(optional<User> *);

  void on_cancer_button_clicked();

  void on_okay_button_clicked();

public:
  explicit UserEdit(QWidget *parent = nullptr);
  ~UserEdit();

private:
  Ui::UserEdit *ui;
  User *user_ = new User();
};
