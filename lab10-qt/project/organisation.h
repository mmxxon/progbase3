#pragma once
#include <QDebug>
using namespace std;

struct Org {
    
    int id;
    QString country;
    QString label;
    int founded_date;
    
};
Q_DECLARE_METATYPE(Org)
