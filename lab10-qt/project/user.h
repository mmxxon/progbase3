#pragma once
#include <QDebug>

using namespace std;

struct User
{
    int id;
    QString username;
    QString password;


};
Q_DECLARE_METATYPE(User)
