#pragma once

#include <QString>
#include <vector>

#include <optional>
#include "founder.h"
#include "organisation.h"
#include "user.h"


using namespace std;

class Storage
{
private:
    QString dir_name_;

public:
    Storage();
    explicit Storage(const QString &);
    virtual ~Storage() {}

    void setName(const QString &);
    QString name() const;

    virtual bool isOpen() const = 0;
    virtual bool open() = 0;
    virtual void close() = 0;
    virtual bool exists() const = 0;
    virtual bool newStorage() = 0;

    // students
    virtual QVector<Org> getAllOrgs() = 0;
    virtual optional<Org> getOrgById(int) = 0;
    virtual bool updateOrg(const Org &) = 0;
    virtual bool removeOrg(int) = 0;
    virtual int insertOrg(const Org &) = 0;

    // courses
    virtual QVector<Fndr> getAllFndrs() = 0;
    virtual optional<Fndr> getFndrById(int) = 0;
    virtual bool updateFndr(const Fndr &) = 0;
    virtual bool removeFndr(int) = 0;
    virtual int insertFndr(const Fndr &) = 0;

    // users
    virtual bool registration_user(const QString &, const QString &) = 0;
    virtual bool change_user_options(int &, const QString &, const QString &) = 0;
    virtual optional<User> getUserAuth( const QString &, const QString &) = 0;
    virtual QVector<Org> getAllUserOrgs(int) = 0;

    // links
    virtual QVector<Fndr> getAllOrgsFndrs(int) = 0;
    virtual bool insertOrgFndr(int, int) = 0;
    virtual bool removeOrgFndr(int, int) = 0;
    virtual bool fndrOrgExec(int, int) = 0;
};

