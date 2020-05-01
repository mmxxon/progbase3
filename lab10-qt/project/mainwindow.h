#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QListWidget>
#include "storage.h"
#include "user.h"
#include "useredit.hpp"
#include "orgedit.hpp"
#include "fndradd.hpp"
#include "fndredit.hpp"
#include "orgadd.hpp"
#include "sqlite_storage.h"
#include <QGraphicsBlurEffect>

#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMetaType>

#include <QDebug>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
signals:
   void user_to_edit(optional<User>*);
   void org_to_update(QListWidgetItem*);
   void fndr_to_update(QListWidgetItem*);
protected:
   void closeEvent(QCloseEvent *);

private slots:
   void addOrg(const QVector<Org> &);

   void addFndr(const QVector<Fndr> &);

   void user_update(User*);

   void org_update(Org*);

   void org_push(Org*);

   void fndr_push(Fndr*);

   void fndr_update(Fndr*);

   void dialogDisabling();

   void on_Continue_clicked();

   void on_open_button_clicked();

   void on_password_textChanged(const QString &);

   void on_login_textChanged(const QString &);

   void on_new_button_clicked();

   void on_confirm_textChanged(const QString &);

   void on_org_list_itemClicked(QListWidgetItem *);
   void on_logout_clicked();
   void on_exit_clicked();

   void on_close_db_clicked();

   void on_edit_user_clicked();

   void on_org_edit_clicked();

   void on_org_add_clicked();

   void on_founders_button_clicked();

   void on_fndr_list_itemClicked(QListWidgetItem*);

   void on_fndr_add_clicked();

   void on_org_del_clicked();

   void on_back_clicked();

   void on_fndr_del_clicked();

   void on_fndr_edit_clicked();

private:
    Ui::MainWindow *ui;
    FndrEdit* fndredit;
    FndrAdd* fndradd;
    OrgEdit* orgedit;
    OrgAdd* orgadd;
    UserEdit* useredit;
    Storage * storage_=nullptr;
    optional <User>  auth_user;
};

#endif // MAINWINDOW_H
//Q_DECLARE_METATYPE(Operator);
