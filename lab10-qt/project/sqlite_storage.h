#pragma once

#include <QDir>
#include <QSqlDatabase>
#include <QtSql>

#include "storage.h"

using namespace std;

class SqliteStorage : public Storage {
protected:
  QSqlDatabase database_;

public:
  explicit SqliteStorage(const QString &dir_name = "");

  bool isOpen() const;
  bool exists() const;
  bool newStorage();
  bool open();
  void close();

  QVector<Org> getAllOrgs();
  optional<Org> getOrgById(int);
  bool updateOrg(const Org &);
  bool removeOrg(int);
  int insertOrg(const Org &);

  // fndrs
  QVector<Fndr> getAllFndrs(void);
  optional<Fndr> getFndrById(int);
  bool updateFndr(const Fndr &);
  bool removeFndr(int);
  int insertFndr(const Fndr &);

  // users
  bool registration_user(const QString &, const QString &);
  bool change_user_options(int &, const QString &, const QString &);
  optional<User> getUserAuth(const QString &, const QString &);
  QVector<Org> getAllUserOrgs(int);

  // links
  QVector<Fndr> getAllOrgsFndrs(int );
  bool insertOrgFndr(int, int);
  bool removeOrgFndr(int, int);
  bool fndrOrgExec(int, int);
};

QString hashPassword(QString const &);
