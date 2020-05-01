#include "mainwindow.h"
#define normal "background-color: rgb(255, 163, 26); \
border-radius:10px;                   \
color: rgb(0, 0, 0);"
#define wrong "color: rgb(0, 0, 0); \
border-radius:10px;  \
background-color: rgb(255, 62, 81);"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  storage_ = new SqliteStorage;
  ui->close_db->setHidden(1);
}

MainWindow::~MainWindow()
{
  if (storage_!=nullptr)
    delete storage_;
  delete ui;
}

void MainWindow::addOrg(const QVector<Org> & org) {
  for (const Org& o: org) {
    QVariant var =QVariant::fromValue(o);
    QListWidgetItem * qOrg = new QListWidgetItem();
    qOrg->setText(o.label);
    qOrg->setData(Qt::UserRole,var);
    qOrg->setTextAlignment(Qt::AlignHCenter);
    ui->org_list->addItem(qOrg);
  }
}

void MainWindow::addFndr(const QVector<Fndr> & fndr) {
  for (const Fndr& f: fndr) {
    QVariant var =QVariant::fromValue(f);
    QListWidgetItem * qf = new QListWidgetItem();
    qf->setText(f.name);
    qf->setData(Qt::UserRole,var);
    qf->setTextAlignment(Qt::AlignHCenter);
    ui->fndr_list->addItem(qf);
  }
}

void MainWindow::user_update(User *u) {
  auth_user.value().id = u->id;
  auth_user.value().password = u->password;
  auth_user.value().username = u->username;
  ui->user_login->setText(u->username);
  storage_->open();
  if (!storage_->change_user_options(u->id, u->username, u->password)) qDebug() << "wrong chng usr";
  storage_->close();
}

void MainWindow::org_update(Org* o) {
  storage_->open();
  QListWidgetItem* i = ui->org_list->selectedItems().at(0);
  Org org = i->data(Qt::UserRole).value<Org>();
  o->id = org.id;
  if (!storage_->updateOrg(*o)) {
    qDebug() << "Err on update";
    return;
  }
  QVariant var;
  var.setValue(*o);
  i->setText(o->label);
  i->setData(Qt::UserRole, var);
  i->setTextAlignment(Qt::AlignHCenter);
  on_org_list_itemClicked(i);
  storage_->close();
}

void MainWindow::fndr_update(Fndr* f) {
  storage_->open();
  QListWidgetItem* i = ui->fndr_list->selectedItems().at(0);
  Fndr fndr = i->data(Qt::UserRole).value<Fndr>();
  f->id = fndr.id;
  if (!storage_->updateFndr(*f)) {
    qDebug() << "Err on update";
    return;
  }
  QVariant var;
  var.setValue(*f);
  i->setText(f->name);
  i->setData(Qt::UserRole, var);
  i->setTextAlignment(Qt::AlignHCenter);
  on_org_list_itemClicked(i);
  storage_->close();
}

void MainWindow::org_push(Org* o) {
  storage_->open();
  storage_->insertOrg(*o);
  QListWidgetItem* item = new QListWidgetItem();

  QVariant var;
  var.setValue(*o);
  item->setText(o->label);
  item->setData(Qt::UserRole, var);
  item->setTextAlignment(Qt::AlignHCenter);

  ui->org_list->addItem(item);
  storage_->close();
}

void MainWindow::fndr_push(Fndr* f) {
  storage_->open();
  storage_->insertFndr(*f);
  QListWidgetItem* item = new QListWidgetItem();

  QVariant var;
  var.setValue(*f);
  item->setText(f->name);
  item->setData(Qt::UserRole, var);
  item->setTextAlignment(Qt::AlignHCenter);

  ui->fndr_list->addItem(item);
  storage_->close();
}

void MainWindow::on_Continue_clicked() {
  QString l(ui->login->text()), p(ui->password->text()), c(ui->confirm->text());
  ui->login->clear();
  ui->password->clear();
  ui->confirm->clear();
  ui->Login_label->setStyleSheet((l.isEmpty())?wrong:normal);
  ui->Password_label->setStyleSheet(p.isEmpty()?wrong:normal);
  if (ui->Password_label->styleSheet()==wrong||ui->Login_label->styleSheet()==wrong) return;
  if (p!=c && !c.isEmpty()){ ui->Password_label->setStyleSheet(wrong); ui->Confirm_label->setStyleSheet(wrong); return;}
  if (!c.isEmpty()){
    storage_->open();
    bool status_reg = storage_->registration_user(l, p);
    storage_->close();
    if (!status_reg) {
      QMessageBox::warning(this, "Registration", "This username is engaged", QMessageBox::Ok);
      return;
    }
  }
  storage_->open();
  auth_user = storage_->getUserAuth(l, p);
  if (!auth_user) {
    ui->Login_label->setStyleSheet(wrong);
    ui->Password_label->setStyleSheet(wrong);
    return;}
  ui->edit_user->setEnabled(1);
  ui->logout->setEnabled(1);
  ui->user_login->setText(auth_user->username);
  addOrg(storage_->getAllUserOrgs(auth_user.value().id));
  storage_->close();
  ui->stackedWidget->setCurrentIndex(2);
  ui->stackedWidget_2->setCurrentIndex(0);
  qDebug() << auth_user->id;
  qDebug() << auth_user->password;
  qDebug() << auth_user->username;
}

void MainWindow::on_open_button_clicked() {
  auto path=QFileDialog::getExistingDirectory(this,"Dialoh",".");
  if (path.isEmpty()) return;
  storage_->setName(path);
  if (!storage_->exists()) { QMessageBox::warning(this, "Err","storage 404", QMessageBox::Ok); return; }
  ui->stackedWidget->setCurrentIndex(1);
  ui->close_db->setEnabled(1);
  ui->close_db->setHidden(0);
}

void MainWindow::on_new_button_clicked() {
  QFileDialog dialog;
  dialog.setFileMode(QFileDialog::Directory);
  QString path = dialog.getSaveFileName(this,"new",QDir::currentPath() + "/data","Folders");
  if (path.isEmpty()) return;
  storage_->setName(path);
  if (storage_->newStorage()) {
    ui->stackedWidget->setCurrentIndex(1);
    ui->stackedWidget_2->setCurrentIndex(0);
  }
  ui->close_db->setEnabled(1);
  ui->close_db->setHidden(0);
}

// FIELDS\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
void MainWindow::on_password_textChanged(const QString &arg1) {
  ui->Password_label->setStyleSheet(normal);
  ui->confirm->clear();
  if (arg1.length() != 0) ui->confirm->setEnabled(1);
  else (ui->confirm->setEnabled(0));
}

void MainWindow::on_login_textChanged(const QString &){
  ui->Login_label->setStyleSheet(normal);
  ui->Password_label->setStyleSheet(normal);
}

void MainWindow::on_confirm_textChanged(const QString &) {
  ui->Confirm_label->setStyleSheet(normal);
  ui->Password_label->setStyleSheet(normal);
}
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//events
void MainWindow::closeEvent(QCloseEvent *e) {(QMessageBox::question(this,"Ex it","Рили?")==QMessageBox::StandardButton::Yes)?e->accept():e->ignore();}



void MainWindow::on_org_list_itemClicked(QListWidgetItem *i) {
  Org o = i->data(Qt::UserRole).value<Org>();
  ui->label_field->setText(o.label);
  ui->country_field->setText(o.country);
  ui->foundation_field->setText(QString::fromStdString(to_string(o.founded_date)));
  ui->org_edit->setEnabled(1);
  ui->org_del->setEnabled(1);
  ui->founders_button->setEnabled(1);
}

void MainWindow::on_logout_clicked() {
  auth_user = nullopt;
  ui->edit_user->setEnabled(0);
  ui->logout->setEnabled(0);
  ui->user_login->clear();
  ui->stackedWidget->setCurrentIndex(1);
  ui->stackedWidget_2->setCurrentIndex(0);
}

void MainWindow::on_exit_clicked() {
  this->close();
}

void MainWindow::on_close_db_clicked() {
  storage_->close();
  auth_user = nullopt;
  ui->edit_user->setEnabled(0);
  ui->logout->setEnabled(0);
  ui->user_login->clear();
  ui->stackedWidget->setCurrentIndex(0);
  ui->stackedWidget_2->setCurrentIndex(0);
  ui->close_db->setDisabled(1);
  ui->close_db->setHidden(1);
}

void MainWindow::dialogDisabling() {
  QGraphicsBlurEffect* effect = new QGraphicsBlurEffect(this);
  if (ui->centralwidget->isEnabled()) {
    ui->centralwidget->setEnabled(0);
    setGraphicsEffect(effect);
  } else {
    ui->centralwidget->setEnabled(1);
    setGraphicsEffect(0);
  }
}

void MainWindow::on_edit_user_clicked() {
  optional<User> u = auth_user;
  useredit = new UserEdit(this);
  dialogDisabling();
  useredit->show();
  connect(this, SIGNAL(user_to_edit(optional<User>*)), useredit, SLOT(edit(optional<User>*)));
  connect(useredit, SIGNAL(upd_user(User*)), this, SLOT(user_update(User*)));
  connect(useredit, SIGNAL(disableToggle()), SLOT(dialogDisabling()));
  emit(user_to_edit(&u));
}

void MainWindow::on_org_edit_clicked() {
  orgedit = new OrgEdit(this);
  dialogDisabling();
  orgedit->show();
  connect(this, SIGNAL(org_to_update(QListWidgetItem*)), orgedit,
          SLOT(edit_org(QListWidgetItem*)));
  connect(orgedit, SIGNAL(upd_org(Org*)), this,
          SLOT(org_update(Org*)));
  connect(orgedit, SIGNAL(disableToggle()), SLOT(dialogDisabling()));

  emit(org_to_update(ui->org_list->selectedItems().at(0)));
}

void MainWindow::on_org_add_clicked() {
  orgadd = new OrgAdd(this);
  dialogDisabling();
  orgadd->show();
  connect(orgadd, SIGNAL(org_add(Org*)), SLOT(org_push(Org*)));
  connect(orgadd, SIGNAL(disableToggle()), SLOT(dialogDisabling()));
}

void MainWindow::on_founders_button_clicked() {
  storage_->open();
  QVector fndrs = storage_->getAllOrgsFndrs(ui->org_list->selectedItems().at(0)->data(Qt::UserRole).value<Org>().id);
  storage_->close();
  ui->fndr_org->setText(ui->org_list->selectedItems().at(0)->data(Qt::UserRole).value<Org>().label);
  addFndr(fndrs);
  ui->stackedWidget_2->setCurrentIndex(1);
}

void MainWindow::on_fndr_list_itemClicked(QListWidgetItem *i) {
  Fndr f = i->data(Qt::UserRole).value<Fndr>();
  ui->name->setText(f.name);
  ui->age->setText(QString(f.age));
  ui->wealth->setText(QString(f.wealth));
  ui->fndr_edit->setEnabled(1);
  ui->fndr_del->setEnabled(1);
  ui->back->setEnabled(1);
}

void MainWindow::on_fndr_add_clicked() {
  fndradd = new FndrAdd(this);
  dialogDisabling();
  fndradd->show();
  connect(fndradd, SIGNAL(fndr_add(Fndr*)), SLOT(fndr_push(Fndr*)));
  connect(fndradd, SIGNAL(disableToggle()), SLOT(dialogDisabling()));
}

void MainWindow::on_org_del_clicked() {
  QMessageBox::StandardButton answer;
  answer = QMessageBox::question(this, "Deletion", "Really?",
                                 QMessageBox::Yes | QMessageBox::No);
  if (answer == QMessageBox::No) return;
  QList<QListWidgetItem*> items = ui->org_list->selectedItems();
  if (items.at(0) == NULL) return;
  foreach (QListWidgetItem* item, items) {
    items = ui->org_list->selectedItems();
    Org org = item->data(Qt::UserRole).value<Org>();
    qDebug() << "deletion: " << org.id;
    storage_->removeOrg(org.id);
    delete ui->org_list->takeItem(ui->org_list->row(item));
  }
  QVector<Org> orgs = storage_->getAllOrgs();
  if (orgs.empty()) {
    ui->org_del->setEnabled(false);
    ui->org_edit->setEnabled(false);

    ui->label_field->setText("");
    ui->country_field->setText("");
    ui->foundation_field->setText("");
  } else {
    items = ui->org_list->selectedItems();
    QListWidgetItem* item = items.at(0);
    QVariant var = item->data(Qt::UserRole);
    Org org = var.value<Org>();

    ui->label_field->setText(org.label);
    ui->country_field->setText(org.country);
    ui->foundation_field->setText(QString(org.founded_date));
  }
}

void MainWindow::on_back_clicked() {
  ui->fndr_edit->setEnabled(0);
  ui->fndr_del->setEnabled(0);
  ui->back->setEnabled(0);
  ui->label_field->setText("");
  ui->country_field->setText("");
  ui->foundation_field->setText(QString(""));
}

void MainWindow::on_fndr_del_clicked() {
  QMessageBox::StandardButton answer;
  answer = QMessageBox::question(this, "Deletion", "Really?",
                                 QMessageBox::Yes | QMessageBox::No);
  if (answer == QMessageBox::No) return;
  QList<QListWidgetItem*> items = ui->fndr_list->selectedItems();
  if (items.at(0) == NULL) return;
  foreach (QListWidgetItem* item, items) {
    items = ui->fndr_list->selectedItems();
    Fndr fndr = item->data(Qt::UserRole).value<Fndr>();
    qDebug() << "deletion: " << fndr.id;
    storage_->removeOrg(fndr.id);
    delete ui->org_list->takeItem(ui->fndr_list->row(item));
  }
  QVector<Fndr> fndrs = storage_->getAllOrgsFndrs(ui->org_list->selectedItems().at(0)->data(Qt::UserRole).value<Org>().id);
  if (fndrs.empty()) {
    ui->fndr_del->setEnabled(false);
    ui->fndr_edit->setEnabled(false);

    ui->name->setText("");
    ui->age->setText("");
    ui->wealth->setText("");
  } else {
    items = ui->fndr_list->selectedItems();
    QListWidgetItem* item = items.at(0);
    QVariant var = item->data(Qt::UserRole);
    Fndr f = var.value<Fndr>();

    ui->name->setText(f.name);
    ui->country_field->setText(QString(f.age));
    ui->foundation_field->setText(QString(f.wealth));
  }
}

void MainWindow::on_fndr_edit_clicked() {
  fndredit = new FndrEdit(this);
  dialogDisabling();
  fndredit->show();
  connect(this, SIGNAL(fndr_to_update(QListWidgetItem*)), fndredit,
          SLOT(edit_fndr(QListWidgetItem*)));
  connect(fndredit, SIGNAL(upd_fndr(Fndr*)), this,
          SLOT(fndr_update(Fndr*)));
  connect(fndredit, SIGNAL(disableToggle()), SLOT(dialogDisabling()));

  emit(org_to_update(ui->fndr_list->selectedItems().at(0)));
}
