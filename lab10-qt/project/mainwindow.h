#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "fndradd.hpp"
#include "fndredit.hpp"
#include "orgadd.hpp"
#include "orgedit.hpp"
#include "sqlite_storage.h"
#include "storage.h"
#include "user.h"
#include "useredit.hpp"
#include <QCloseEvent>
#include <QGraphicsBlurEffect>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QMessageBox>

#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMetaType>

#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
signals:
  void user_to_edit(optional<User> *);
  void org_to_update(QListWidgetItem *);
  void fndr_to_update(QListWidgetItem *);

protected:
  void closeEvent(QCloseEvent *);

private slots:
  void on_exit_clicked();

  void on_main_new_clicked();
  void on_main_open_clicked();

  void on_close_clicked();
  void on_login_login_textChanged(const QString &);
  void on_login_confirm_textChanged(const QString &);
  void on_login_pass_textChanged(const QString &);
  void on_login_continue_clicked();

  void addOrg(const QVector<Org> &);
  void addFndr(const QVector<Fndr> &);
  void dialogDisabling();
  void user_update(User *);
  void org_push(Org *);
  void org_update(Org *);
  void fndr_push(Fndr *);
  void fndr_update(Fndr *);

  void on_ent_user_edit_clicked();
  void on_ent_user_logout_clicked();

  void on_org_add_clicked();
  void on_org_del_clicked();
  void on_org_edit_clicked();
  void on_org_founders_clicked();
  void on_orgs_list_itemClicked(QListWidgetItem *);

  void on_fndr_add_clicked();
  void on_fndr_del_clicked();
  void on_fndr_edit_clicked();
  void on_back_clicked();
  void on_fndr_list_itemClicked(QListWidgetItem *);

  void on_org_search_textChanged(const QString &arg1);

  void on_fndr_search_textChanged(const QString &arg1);

private:
  Ui::MainWindow *ui;
  FndrEdit *fndredit;
  FndrAdd *fndradd;
  OrgEdit *orgedit;
  OrgAdd *orgadd;
  UserEdit *useredit;
  Storage *storage_ = nullptr;
  optional<User> auth_user;
  Org global_org;
  QString osearch = "";
  QString fsearch = "";
  int org_curr_page=0;
  int org_max_page=0;
  int fndr_curr_page=0;
  int fndr_max_page=0;
};

#endif // MAINWINDOW_H
// Q_DECLARE_METATYPE(Operator);
