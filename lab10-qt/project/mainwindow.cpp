#include "mainwindow.h"
#define normal                                                                 \
    "background-color: rgb(255, 163, 26); border-radius:10px; color: rgb(0, 0, " \
    "0);"
#define wrong                                                                  \
    "color: rgb(0, 0, 0); border-radius:10px; background-color: rgb(255, 62, "   \
    "81);"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    storage_ = new SqliteStorage;
    ui->close->setHidden(1);
    ui->menu->setCurrentIndex(0);
    ui->entities->setCurrentIndex(0);
    ui->exit->setEnabled(1);
    ui->main_new->setEnabled(1);
    ui->main_open->setEnabled(1);
    ui->placeholder->setHidden(1);
    ui->placeholder_2->setHidden(1);
    ui->placeholder_3->setHidden(1);
    ui->menu->setCurrentIndex(0);
    ui->entities->setCurrentIndex(0);
    ui->fndr_prev->setHidden(1);
    ui->fndr_next->setHidden(1);
    ui->fndr_page->setHidden(1);
    ui->org_prev->setHidden(1);
    ui->org_next->setHidden(1);
    ui->org_page->setHidden(1);
    QValidator *validator = new QRegExpValidator(QRegExp("[a-zA-Z0-9]+"), this);
    ui->login_login->setValidator(validator);
}

MainWindow::~MainWindow() {
    if (storage_ != nullptr)
        delete storage_;
    delete ui;
}

/* ---- Global --- */
void MainWindow::on_exit_clicked() {
    if (storage_->isOpen())
        storage_->close();
    this->close();
}
void MainWindow::on_close_clicked() {
    auth_user = nullopt;
    storage_->close();
    ui->close->setHidden(1);
    ui->close->setDisabled(1);

    ui->login_login->clear();
    ui->login_pass->clear();
    ui->login_confirm->clear();
    ui->login_login->setDisabled(1);
    ui->login_confirm->setDisabled(1);
    ui->login_pass->setDisabled(1);
    ui->login_continue->setDisabled(1);

    ui->ent_user->clear();
    ui->ent_user_edit->setDisabled(1);
    ui->ent_user_logout->setDisabled(1);

    ui->orgs_list->clear();
    ui->org_label->clear();
    ui->org_country->clear();
    ui->org_add->setDisabled(1);
    ui->org_edit->setDisabled(1);
    ui->org_del->setDisabled(1);
    ui->orgs_list->setDisabled(1);
    ui->org_founders->setDisabled(1);

    ui->fndr_list->setDisabled(1);
    ui->fndr_name->clear();
    ui->fndr_org->clear();
    ui->fndr_age->clear();
    ui->fndr_wealth->clear();
    ui->fndr_add->setDisabled(1);
    ui->fndr_edit->setDisabled(1);
    ui->fndr_del->setDisabled(1);

    ui->org_page->setValue(0);
    ui->org_page->clear();
    ui->org_prev->setHidden(1);
    ui->org_next->setHidden(1);

    ui->menu->setCurrentIndex(0);
    ui->entities->setCurrentIndex(0);
}

void MainWindow::closeEvent(QCloseEvent *e) {
    (QMessageBox::question(this, "Ex it", "Рили?") ==
            QMessageBox::StandardButton::Yes)
            ? e->accept()
            : e->ignore();
}

void MainWindow::addOrg(const QVector<Org> &org) {
    ui->orgs_list->setEnabled(1);
    foreach (const Org &o, org) {
        QVariant var = QVariant::fromValue(o);
        QListWidgetItem *qOrg = new QListWidgetItem();
        qOrg->setText(o.label);
        qOrg->setData(Qt::UserRole, var);
        qOrg->setTextAlignment(Qt::AlignHCenter);
        ui->orgs_list->addItem(qOrg);
    }
    org_curr_page=0;
    org_max_page=ceil(storage_->countOrgs(auth_user->id, osearch) / 10 );

    if (org_max_page != 0) {
        ui->org_next->setHidden(0);
        ui->org_next->setText(QString::fromStdString(to_string(org_curr_page + 2)));
    }
    ui->org_page->setValue(0);
    ui->org_page->setMaximum(org_max_page);
    qDebug() << "Count: " << storage_->countOrgs(auth_user->id, osearch) / 10;
}

void MainWindow::addFndr(const QVector<Fndr> &fndr) {
    for (const Fndr &f : fndr) {
        QVariant var = QVariant::fromValue(f);
        QListWidgetItem *qf = new QListWidgetItem();
        qf->setText(f.name);
        qf->setData(Qt::UserRole, var);
        qf->setTextAlignment(Qt::AlignHCenter);
        ui->fndr_list->addItem(qf);
    }
}

void MainWindow::org_push(Org *o) {
    o->id = storage_->insertOrg(*o, auth_user.value().id);
    QListWidgetItem *item = new QListWidgetItem();

    QVariant var;
    var.setValue(*o);
    item->setText(o->label);
    item->setData(Qt::UserRole, var);
    item->setTextAlignment(Qt::AlignHCenter);

    ui->orgs_list->addItem(item);
    org_curr_page=0;
    org_max_page=ceil(storage_->countOrgs(auth_user->id, osearch) / 10 );
    if (org_max_page != 0) {
        ui->org_next->setHidden(0);
        ui->org_next->setText(QString::fromStdString(to_string(org_curr_page + 2)));
    }
    ui->org_page->setValue(0);
    ui->org_page->setMaximum(org_max_page);
    qDebug() << "Count: " << storage_->countOrgs(auth_user->id, osearch) / 10;
}

void MainWindow::fndr_push(Fndr *f) {
    int id = storage_->insertFndr(*f);
    storage_->insertOrgFndr(global_org.id, id);
    QListWidgetItem *item = new QListWidgetItem();

    QVariant var;
    var.setValue(*f);
    item->setText(f->name);
    item->setData(Qt::UserRole, var);
    item->setTextAlignment(Qt::AlignHCenter);

    ui->fndr_list->addItem(item);
}

void MainWindow::user_update(User *u) {
    if (!storage_->change_user_options(u->id, u->username, u->password)) {
        qDebug() << "wrong chng usr";
        QMessageBox::warning(this, "Error", "Error on user changing", QMessageBox::Ok);
    }
    else {
        auth_user.value().id = u->id;
        auth_user.value().password = u->password;
        auth_user.value().username = u->username;
        ui->ent_user->setText(u->username);
    }
}

void MainWindow::org_update(Org *org) {
    QListWidgetItem *item = ui->orgs_list->selectedItems().at(0);
    Org orga = item->data(Qt::UserRole).value<Org>();
    org->id = orga.id;
    if (!storage_->updateOrg(*org)) {
        qDebug() << "Err on update";
        return;
    }
    QVariant var;
    var.setValue(*org);
    item->setText(org->label);
    item->setData(Qt::UserRole, var);
    item->setTextAlignment(Qt::AlignHCenter);
    on_orgs_list_itemClicked(item);
}

void MainWindow::fndr_update(Fndr *fndr) {
    QListWidgetItem *item = ui->fndr_list->selectedItems().at(0);
    Fndr fn = item->data(Qt::UserRole).value<Fndr>();
    fndr->id = fn.id;
    if (!storage_->updateFndr(*fndr)) {
        qDebug() << "Err on update";
        return;
    }
    QVariant var;
    var.setValue(*fndr);
    item->setText(fndr->name);
    item->setData(Qt::UserRole, var);
    item->setTextAlignment(Qt::AlignHCenter);
    on_fndr_list_itemClicked(item);
}

void MainWindow::dialogDisabling() {
    QGraphicsBlurEffect *effect = new QGraphicsBlurEffect(this);
    if (ui->centralwidget->isEnabled()) {
        ui->centralwidget->setEnabled(0);
        setGraphicsEffect(effect);
    } else {
        ui->centralwidget->setEnabled(1);
        setGraphicsEffect(0);
    }
}

/* ---- Main Menu ---- */
void MainWindow::on_main_open_clicked() {
    auto path = QFileDialog::getExistingDirectory(this, "Dialoh", ".");
    if (path.isEmpty())
        return;
    storage_->setName(path);
    if (!storage_->exists()) {
        QMessageBox::warning(this, "Err", "storage 404", QMessageBox::Ok);
        return;
    }
    ui->menu->setCurrentIndex(1);
    ui->close->setEnabled(1);
    ui->close->setHidden(0);
    ui->login_login->setEnabled(1);
    storage_->open();
    qDebug() << "Opened on: " << path;
}
void MainWindow::on_main_new_clicked() {
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::Directory);
    QString path = dialog.getSaveFileName(
                this, "new", QDir::currentPath() + "/data", "Folders");
    if (path.isEmpty())
        return;
    storage_->setName(path);
    if (!storage_->newStorage())
        return;
    ui->menu->setCurrentIndex(1);
    ui->close->setEnabled(1);
    ui->close->setHidden(0);
    ui->login_login->setEnabled(1);
    storage_->open();
    qDebug() << "Opened on: " << path;
}

/* ---- Login Menu ---- */
void MainWindow::on_login_login_textChanged(const QString &text) {
    ui->login_login_label->setStyleSheet(normal);
    ui->login_pass_label->setStyleSheet(normal);
    ui->login_pass->clear();
    ui->login_confirm->clear();
    ui->login_pass->setEnabled((text.isEmpty()) ? 0 : 1);
}
void MainWindow::on_login_confirm_textChanged(const QString &) {
    ui->login_confirm_label->setStyleSheet(normal);
    ui->login_pass_label->setStyleSheet(normal);
}
void MainWindow::on_login_pass_textChanged(const QString &text) {
    ui->login_pass_label->setStyleSheet(normal);
    ui->login_confirm->clear();
    ui->login_confirm->setEnabled((text.isEmpty()) ? 0 : 1);
    ui->login_continue->setEnabled((text.isEmpty()) ? 0 : 1);
}
void MainWindow::on_login_continue_clicked() {
    QString login = ui->login_login->text();
    QString pass = ui->login_pass->text();
    QString confirm = ui->login_confirm->text();

    ui->login_login_label->setStyleSheet((login.isEmpty()) ? wrong : normal);
    ui->login_pass_label->setStyleSheet(pass.isEmpty() ? wrong : normal);

    if (login.isEmpty() || pass.isEmpty())
        return;
    else if (pass != confirm && !confirm.isEmpty()) {
        ui->login_pass_label->setStyleSheet(wrong);
        ui->login_confirm_label->setStyleSheet(wrong);
        return;
    } else if (!confirm.isEmpty()) {
        bool status_reg = storage_->registration_user(login, pass);
        if (!status_reg) {
            QMessageBox::warning(this, "Registration", "This username is occupied",
                                 QMessageBox::Ok);
            return;
        }
    }
    auth_user = storage_->getUserAuth(login, pass);
    if (!auth_user) {
        ui->login_login_label->setStyleSheet(wrong);
        ui->login_pass_label->setStyleSheet(wrong);
        return;
    }
    qDebug() << auth_user->id;
    qDebug() << auth_user->username;
    qDebug() << auth_user->password;

    addOrg(storage_->getAllUserOrgs(auth_user.value().id, osearch, org_curr_page));
    ui->login_login->clear();
    ui->login_pass->clear();
    ui->login_confirm->clear();
    ui->login_confirm->setDisabled(1);
    ui->login_pass->setDisabled(1);
    ui->login_continue->setDisabled(1);

    ui->org_add->setEnabled(1);
    org_curr_page=0;
    ui->org_page->setValue(org_curr_page + 1);
    ui->org_page->setHidden(0);

    ui->ent_user->setText(login);
    ui->ent_user_edit->setEnabled(1);
    ui->ent_user_logout->setEnabled(1);

    ui->menu->setCurrentIndex(2);
}
/* ---- User Menu ---- */
void MainWindow::on_ent_user_edit_clicked() {
    optional<User> user = auth_user;
    useredit = new UserEdit(this);
    dialogDisabling();
    useredit->show();
    connect(this, SIGNAL(user_to_edit(optional<User> *)), useredit, SLOT(edit(optional<User> *)));
    connect(useredit, SIGNAL(upd_user(User *)), this, SLOT(user_update(User*)));
    connect(useredit, SIGNAL(disableToggle()), SLOT(dialogDisabling()));
    emit(user_to_edit(&user));
}

void MainWindow::on_ent_user_logout_clicked() {
    auth_user = nullopt;
    ui->ent_user->clear();
    ui->ent_user_edit->setDisabled(1);
    ui->ent_user_logout->setDisabled(1);

    ui->orgs_list->clear();
    ui->org_label->clear();
    ui->org_country->clear();
    ui->org_add->setDisabled(1);
    ui->org_edit->setDisabled(1);
    ui->org_del->setDisabled(1);
    ui->orgs_list->setDisabled(1);
    ui->org_founders->setDisabled(1);

    ui->fndr_list->setDisabled(1);
    ui->fndr_name->clear();
    ui->fndr_org->clear();
    ui->fndr_age->clear();
    ui->fndr_wealth->clear();
    ui->fndr_add->setDisabled(1);
    ui->fndr_edit->setDisabled(1);
    ui->fndr_del->setDisabled(1);

    ui->org_page->setValue(0);
    ui->org_page->clear();
    ui->org_prev->setHidden(1);
    ui->org_next->setHidden(1);

    ui->fndr_page->setValue(0);
    ui->fndr_page->clear();
    ui->fndr_prev->setHidden(1);
    ui->fndr_next->setHidden(1);

    ui->menu->setCurrentIndex(1);
    ui->entities->setCurrentIndex(0);
}

/* ---- Orgs menu ----
 * ----- Windows ----- */
void MainWindow::on_org_add_clicked() {
    orgadd = new OrgAdd(this);
    dialogDisabling();
    orgadd->show();
    connect(orgadd, SIGNAL(org_add(Org *)), SLOT(org_push(Org *)));
    connect(orgadd, SIGNAL(disableToggle()), SLOT(dialogDisabling()));
}
void MainWindow::on_org_edit_clicked() {
    if (!ui->orgs_list->selectedItems().at(0)) return;
    orgedit = new OrgEdit(this);
    dialogDisabling();
    orgedit->show();
    connect(this, SIGNAL(org_to_update(QListWidgetItem *)), orgedit, SLOT(edit_org(QListWidgetItem *)));
    connect(orgedit, SIGNAL(upd_org(Org *)), this, SLOT(org_update(Org *)));
    connect(orgedit, SIGNAL(disableToggle()), SLOT(dialogDisabling()));

    emit(org_to_update(ui->orgs_list->selectedItems().at(0)));
}

void MainWindow::on_org_del_clicked() {
    if (!ui->orgs_list->selectedItems().at(0)) return;
    QMessageBox::StandardButton answer;
    answer = QMessageBox::question(this, "Deletion", "Really?",
                                   QMessageBox::Yes | QMessageBox::No);
    if (answer == QMessageBox::No)
        return;
    QVector<Fndr> fndrs = storage_->getAllOrgsFndrs(global_org.id, fsearch);
    for ( auto &item: fndrs) {
        storage_->removeOrgFndr(global_org.id, item.id);
        storage_->removeFndr(item.id);
    }
    storage_->removeOrg(global_org.id);

    delete ui->orgs_list->takeItem(ui->orgs_list->row(ui->orgs_list->selectedItems().at(0)));
    ui->org_del->setEnabled(false);
    ui->org_edit->setEnabled(false);

    ui->org_label->setText("");
    ui->org_country->setText("");
    ui->org_found->setText("");
}

/* Element */
void MainWindow::on_orgs_list_itemClicked(QListWidgetItem* item) {
    global_org = item->data(Qt::UserRole).value<Org>();
    ui->org_label->setText(global_org.label);
    ui->org_country->setText(global_org.country);
    ui->org_found->setText(QString::fromStdString(to_string(global_org.founded_date)));
    ui->org_edit->setEnabled(1);
    ui->org_del->setEnabled(1);
    ui->org_founders->setEnabled(1);
}

void MainWindow::on_org_founders_clicked() {
    if (!ui->orgs_list->selectedItems().at(0)) return;
    QVector fndrs = storage_->getAllOrgsFndrs(global_org.id, fsearch);
    ui->fndr_org->setText(ui->orgs_list->selectedItems()
                          .at(0)
                          ->data(Qt::UserRole)
                          .value<Org>()
                          .label);
    addFndr(fndrs);
    ui->entities->setCurrentIndex(1);
    ui->fndr_add->setEnabled(1);
    ui->fndr_list->setEnabled(1);
}
/* ---- Fndrs menu ----
 * ----- Windows ----- */
void MainWindow::on_fndr_add_clicked() {
    fndradd = new FndrAdd(this);
    dialogDisabling();
    fndradd->show();
    connect(fndradd, SIGNAL(fndr_add(Fndr *)), SLOT(fndr_push(Fndr *)));
    connect(fndradd, SIGNAL(disableToggle()), SLOT(dialogDisabling()));
}

void MainWindow::on_fndr_edit_clicked() {
    if (!ui->fndr_list->selectedItems().at(0)) return;
    fndredit = new FndrEdit(this);
    dialogDisabling();
    fndredit->show();
    connect(this, SIGNAL(fndr_to_update(QListWidgetItem *)), fndredit,SLOT(edit_fndr(QListWidgetItem *)));
    connect(fndredit, SIGNAL(upd_fndr(Fndr *)), this, SLOT(fndr_update(Fndr*)));
    connect(fndredit, SIGNAL(disableToggle()), SLOT(dialogDisabling()));
    emit(org_to_update(ui->fndr_list->selectedItems().at(0)));
}


void MainWindow::on_fndr_del_clicked() {
    if (!ui->fndr_list->selectedItems().at(0)) return;
    QMessageBox::StandardButton answer;
    answer = QMessageBox::question(this, "Deletion", "Really?",
                                   QMessageBox::Yes | QMessageBox::No);
    if (answer == QMessageBox::No) return;
    auto item = ui->fndr_list->selectedItems().at(0);
    Fndr fndr = item->data(Qt::UserRole).value<Fndr>();
    qDebug() << "deletion: " << fndr.id;
    storage_->removeFndr(fndr.id);
    storage_->removeOrgFndr(global_org.id, fndr.id);
    delete ui->fndr_list->takeItem(ui->fndr_list->row(item));
    auto fndrs = storage_->getAllOrgsFndrs(global_org.id, fsearch);
    if (fndrs.empty()) {
        ui->fndr_del->setEnabled(false);
        ui->fndr_edit->setEnabled(false);

        ui->fndr_name->setText("");
        ui->fndr_age->setText("");
        ui->fndr_wealth->setText("");
    } else {
        fndr = ui->fndr_list->selectedItems().at(0)->data(Qt::UserRole).value<Fndr>();

        ui->fndr_name->setText(fndr.name);
        ui->fndr_age->setText(QString::fromStdString(to_string(fndr.age)));
        ui->fndr_wealth->setText(QString::fromStdString(to_string(fndr.wealth)));
    }
}

/* Element */
void MainWindow::on_fndr_list_itemClicked(QListWidgetItem *item) {
    Fndr fn = item->data(Qt::UserRole).value<Fndr>();
    ui->fndr_name->setText(fn.name);
    ui->fndr_age->setText(QString::fromStdString(to_string(fn.age)));
    ui->fndr_wealth->setText(QString::fromStdString(to_string(fn.wealth)));
    ui->fndr_edit->setEnabled(1);
    ui->fndr_del->setEnabled(1);
}

 void MainWindow::on_back_clicked() {
  ui->fndr_list->clear();
  ui->fndr_edit->setEnabled(0);
  ui->fndr_del->setEnabled(0);
  ui->fndr_list->setDisabled(1);
  ui->fndr_name->setText("");
  ui->fndr_age->setText("");
  ui->fndr_wealth->setText(QString(""));
  ui->entities->setCurrentIndex(0);
}

/* Additional funct */
void MainWindow::on_org_search_textChanged(const QString &arg1) {
  osearch = arg1;
  ui->orgs_list->clear();
  ui->org_edit->setDisabled(1);
  ui->org_del->setDisabled(1);
  org_curr_page=0;
  addOrg(storage_->getAllUserOrgs(auth_user.value().id, osearch, org_curr_page));
}

void MainWindow::on_fndr_search_textChanged(const QString &arg1) {
    fsearch = arg1;
    ui->fndr_list->clear();
    ui->fndr_edit->setDisabled(1);
    ui->fndr_del->setDisabled(1);
    addFndr(storage_->getAllOrgsFndrs(global_org.id, fsearch));
}
