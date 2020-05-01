#include "sqlite_storage.h"
#include <QCryptographicHash>

SqliteStorage::SqliteStorage(const QString &dir_name) : Storage(dir_name) {
  database_ = QSqlDatabase::addDatabase("QSQLITE");
}

bool SqliteStorage::isOpen() const { return database_.isOpen(); }

bool SqliteStorage::exists() const {
  return QFile(name() + "/data.sqlite").exists();
}

bool SqliteStorage::open() {
  if (!name().isEmpty()) {
    QDir().mkdir(name());
  } else
    return false;

  database_.setDatabaseName(name() + "/data.sqlite");

  if (!database_.open()) {
    qDebug() << "on open: " << database_.lastError();
    return false;
  }
  return true;
}

bool SqliteStorage::newStorage() {
  this->open();
  QSqlQuery query;
  query.prepare("CREATE TABLE orgs (id	INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,country	TEXT NOT NULL,label	TEXT NOT NULL,founded_date	INTEGER NOT NULL,user_id INTEGER,FOREIGN KEY (user_id) REFERENCES users (id))");
  if (!query.exec()) {
    qDebug() << "on org q:"<< query.lastError();
    return false;
  }

  query.prepare("CREATE TABLE fndrs (id	INTEGER NOT NULL UNIQUE,name	TEXT NOT NULL,age	INTEGER NOT NULL,wealth	INTEGER NOT NULL)");
  if (!query.exec()) {
    qDebug() << "on fndr q:" << query.lastError();
    return false;
  }
  query.prepare("CREATE TABLE users ( id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, username TEXT NOT NULL, password_hash TEXT NOT NULL)");
  if (!query.exec()) {
    qDebug() << "on user q:" << query.lastError();
    return false;
  }

  query.prepare("CREATE TABLE links (id	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,org_id	INTEGER,fndr_id	INTEGER)");
  if (!query.exec()) {
    qDebug() << "on link q:" << query.lastError();
    return false;
  }
  return true;
}

void SqliteStorage::close() { database_.close(); }

Org OrgFields(QSqlQuery &q) {
  Org o;
  o.id = q.value("id").toInt();
  o.country = q.value("country").toString();
  o.label = q.value("label").toString();
  o.founded_date = q.value("founded_date").toInt();
  return o;
}

QVector<Org> SqliteStorage::getAllOrgs() {
  QVector<Org> os;
  QSqlQuery q("SELECT * FROM orgs");
  while (q.next()) os.push_back(OrgFields(q));
  return os;
}

optional<Org> SqliteStorage::getOrgById(int oid) {
  QSqlQuery q;
  if (!q.prepare("SELECT * FROM orgs WHERE id = :id")) {
    qDebug() << "get org prep:" << q.lastError();
    return nullopt;
  }
  q.bindValue(":id", oid);
  if (!q.exec()) {
    qDebug() << "get org exec:" << q.lastError();
    return nullopt;
  }
  if (q.next()) return optional<Org>(OrgFields(q));
  else {
    qDebug() << "org 404";
    return nullopt;
  }
}

bool SqliteStorage::updateOrg(const Org &o) {
  QSqlQuery q;
  if (!q.prepare("UPDATE orgs SET country = :country, "
                 "label = :label, "
                 "founded_date = :founded_date "
                 "WHERE id = :id")) {
    qDebug() << "upd org prep:" << q.lastError();
    return false;
  }
  q.bindValue(":id", o.id);
  q.bindValue(":country", o.country);
  q.bindValue(":label", o.label);
  q.bindValue(":founded_date", o.founded_date);
  if (!q.exec()) {
    qDebug() << "upd org exec:" << q.lastError();
    return 0;
  } else if (q.numRowsAffected() == 0) {
    qDebug() << "0 rows";
    return 0;
  }
  return true;
}

bool SqliteStorage::removeOrg(int oid) {
  QSqlQuery q;
  if (!q.prepare("DELETE FROM orgs WHERE id = :id")) {
    qDebug() << "del org prep:" << q.lastError();
    return false;
  }
  q.bindValue(":id", oid);
  if (!q.exec()) {
    qDebug() << "upd org exec:" << q.lastError();
    return 0;
  } else if (q.numRowsAffected() == 0) {
    qDebug() << "0 rows";
    return 0;
  } return true;
}

int SqliteStorage::insertOrg(const Org & o) {
  QSqlQuery q;
  if (!q.prepare("INSERT INTO orgs (country, label, founded_date) VALUES (:country, :label, :founded_date)")) {
    qDebug() << "ins org prep" << q.lastError();
  }
  q.bindValue(":country", o.country);
  q.bindValue(":label", o.label);
  q.bindValue(":founded_date", o.founded_date);
  if (!q.exec()) {
    qDebug() << "ins org exec:" << q.lastError();
    return 0;
  }
  return q.lastInsertId().toInt();
}

Fndr FndrFields(QSqlQuery &q) {
  Fndr f;
  f.id = q.value("id").toInt();
  f.name = q.value("name").toString();
  f.age = q.value("age").toInt();
  f.wealth = q.value("wealth").toInt();
  return f;
}

QVector<Fndr> SqliteStorage::getAllFndrs() {
  QVector<Fndr> fs;
  QSqlQuery q("SELECT * FROM fndrs");
  while (q.next()) fs.push_back(FndrFields(q));
  return fs;
}
optional<Fndr> SqliteStorage::getFndrById(int fid) {
  QSqlQuery q;
  if (!q.prepare("SELECT * FROM fndrs WHERE id = :id")) {
    qDebug() << "get fndr prep:" << q.lastError();
    return nullopt;
  }
  q.bindValue(":id", fid);
  if (!q.exec()) {
    qDebug() << "get fndr exec:" << q.lastError();
    return nullopt;
  }
  if (q.next()) return optional<Fndr>(FndrFields(q));
  else {
    qDebug() << "fndr 404";
    return nullopt;
  }
}

bool SqliteStorage::updateFndr(const Fndr & f) {
  QSqlQuery q;
  if (!q.prepare("UPDATE fndrs SET name = :name, age = "
                 ":age, wealth = :wealth WHERE id = :id")) {
    qDebug() << "upd fndr prep:" << q.lastError();
    return 0;
  }
  q.bindValue(":id", f.id);
  q.bindValue(":name", f.name);
  q.bindValue(":age", f.age);
  q.bindValue(":wealth", f.wealth);
  if (!q.exec()) {
    qDebug() << "upd fndr exec:" << q.lastError();
    return 0;
  } else if (q.numRowsAffected() == 0) {
    qDebug() << "0 rows";
    return false;
  }
  return true;
}

bool SqliteStorage::removeFndr(int fid) {
  QSqlQuery q;
  if (!q.prepare("DELETE FROM fndrs WHERE id = :id")) {
    qDebug() << "del fndr prep:" << q.lastError();
    return false;
  }
  q.bindValue(":id", fid);
  if (!q.exec()) {
    qDebug() << "del fndr exec:" << q.lastError();
    return false;
  } else if (q.numRowsAffected() == 0) {
    qDebug() << "0 rows";
    return false;
  }
  else return true;
}

int SqliteStorage::insertFndr(const Fndr & f) {
  QSqlQuery q;
  if (!q.prepare("INSERT INTO fndrs (name, age, wealth) VALUES (:name, :age, :wealth)")) {
    qDebug() << "ins fndr exec:" << q.lastError();
  }
  q.bindValue(":name", f.name);
  q.bindValue(":age", f.age);
  q.bindValue(":wealth", f.wealth);
  if (!q.exec()) {
    qDebug() << "ins fndr exec" << q.lastError();
    return 0;
  } return q.lastInsertId().toInt();
}

QString hashPassword(QString const &pass) {
  QByteArray p = pass.toUtf8();
  QByteArray h = QCryptographicHash::hash(p, QCryptographicHash::Sha512);
  QString hash = QString(h.toHex());
  return hash;
}

bool SqliteStorage::registration_user(const QString &usr, const QString &pas) {
  QSqlQuery q;
  if (!q.prepare("SELECT * FROM users "
                     "WHERE username = :username;")) {
    qDebug() << "srch usr prep:" << q.lastError();
    return 0;
  }
  q.bindValue(":username", usr);
  if (!q.exec()) {
    qDebug() << "srch usr exec:" << q.lastError();
    throw q.lastError();
  } else if (q.next()) return 0;
  else if (!q.prepare("INSERT INTO users (username, password_hash) VALUES "
                     "(:username, :password_hash)")) {
    qDebug() << "reg usr prep:" << q.lastError();
    return 0;
  }
  q.bindValue(":username", usr);
  q.bindValue(":password_hash", hashPassword(pas));
  if (!q.exec()) {
    qDebug() << "reg usr exec:" << q.lastError();
    return 0;
  }
  return 1;
}

bool SqliteStorage::change_user_options(int & id, const QString & ulog, const QString &upas) {
  QSqlQuery q;

  if (!q.prepare("SELECT * FROM users WHERE username = :username;")) {
    qDebug() << "srch usr prep:" << q.lastError();
    return 0;
  }
  q.bindValue(":username", ulog);
  if (!q.exec()) {
    qDebug() << "srch usr exec:" << q.lastError();
    return 0;
  }
  if (q.next() && q.value("username").toString() == ulog && q.value("id").toInt() != id) return false;
  QString txt;
  if (!upas.isEmpty() && !ulog.isEmpty())
    txt = "UPDATE users SET username = :username, password_hash = :password_hash  WHERE id = :id";
  else if (upas.isEmpty())
    txt = "UPDATE users SET username = :username WHERE id = :id";
  else if (ulog.isEmpty())
    txt = "UPDATE users SET password_hash = :password_hash WHERE id = :id";
  if (!q.prepare(txt)) {
    qDebug() << "upd user prep:" << q.lastError();
    return 0;
  }
  else if (!ulog.isEmpty())
    q.bindValue(":username", ulog);
  if (!upas.isEmpty())
    q.bindValue(":password_hash", upas);
  q.bindValue(":id", id);
  if (!q.exec()) {
    qDebug() << "upd user exec:" << q.lastError();
    return 0;
  }
  else if (q.numRowsAffected() == 0) {
    qDebug() << "0 rows";
    return 0;
  }
  return 1;
}

optional<User> SqliteStorage::getUserAuth(const QString &ulog, const QString &upas) {
  QSqlQuery q;
  if (!q.prepare("SELECT * FROM users WHERE username = :username AND password_hash = :password_hash;")) {
    qDebug() << "get user prep:" << q.lastError();
    return nullopt;
  }
  q.bindValue(":username", ulog);
  q.bindValue(":password_hash", hashPassword(upas));
  if (!q.exec()) {
    qDebug() << "get user exec:" << q.lastError();
    return nullopt;
  }
  else if (q.next()) {
    User user;
    user.id = q.value("id").toInt();
    user.username = q.value("username").toString();
    user.password = q.value("password_hash").toString();
    return user;
  }
  return nullopt;
}

QVector<Org> SqliteStorage::getAllUserOrgs(int id) {
  QVector<Org> os;
  QSqlQuery q;
  if (!q.prepare("SELECT * FROM orgs WHERE user_id = :user_id")) {
    qDebug() << "get user orgs prep:" << q.lastError();
    return os;
  }
  q.bindValue(":user_id", id);
  if (!q.exec()) {
    qDebug() << "get user_id: " << q.lastError();
  }
  while (q.next()) {
    os.push_back(OrgFields(q));
  }
  return os;
}

QVector<Fndr> SqliteStorage::getAllOrgsFndrs(int id) {
  QVector<Fndr> fs;
  QSqlQuery q;
  if (!q.prepare("SELECT * FROM links WHERE org_id = :org_id")) {
    qDebug() << "get org fndr prep" << q.lastError();
    throw q.lastError();
  }
  q.bindValue(":org_id", id);
  q.exec();
  while (q.next()) fs.push_back(getFndrById(q.value("fndr_id").toInt()).value());
  return fs;
}

bool SqliteStorage::insertOrgFndr(int oid, int fid) {
  QSqlQuery q;
  if (!q.prepare("INSERT INTO links (org_id, fndr_id) VALUES (:org_id, "
                     ":fndr_id)")) {
    qDebug() << "ins org fndr prep" << q.lastError();
    return 0;
  }
  q.bindValue(":org_id", oid);
  q.bindValue(":fndr_id", fid);
  if (!q.exec()) {
    qDebug() << "ins org fndr exec" << q.lastError();
    return 0;
  } return 1;
}
bool SqliteStorage::removeOrgFndr(int oid, int fid) {
  QSqlQuery q;
  if (!q.prepare("DELETE FROM links WHERE org_id = :org_id AND fndr_id "
                     "= :fndr_id")) {
    qDebug() << "del org fndr prep:" << q.lastError();
    return 0;
  }
  q.bindValue(":book_id", oid);
  q.bindValue(":fndr_id", fid);
  if (!q.exec()) {
    qDebug() << "del org fndr exec:" << q.lastError();
    return 0;
  } else if (q.numRowsAffected() == 0) {
    qDebug() << "0 rows";
    return 0;
  } return 1;
}

bool SqliteStorage::fndrOrgExec(int oid, int fid) {
  QSqlQuery q;
  if (!q.prepare("SELECT * FROM links WHERE org_id = :org_id AND "
                     "fndr_id = :fndr_id")) {
    qDebug() << "org fndr exec prep:" << q.lastError();
    return 0;
  }
  q.bindValue(":org_id", oid);
  q.bindValue(":fndr_id", fid);
  if (!q.exec()) {
    qDebug() << "org fndr exec:" << q.lastError();
    return 0;
  }
  if (q.next()) return 1;
  else return 0;
}
