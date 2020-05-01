#pragma once
#include <QDebug>
using namespace std;

struct Fndr {
    
    int id;
    QString name;
    int age;
    int wealth;
    
};
Q_DECLARE_METATYPE(Fndr)
