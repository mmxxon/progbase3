#include "storage.h"

Storage::Storage(const QString &dir_name)
{
    dir_name_ = dir_name;
}

void Storage::setName(const QString& dir_name)
{
    dir_name_=dir_name;
}
QString Storage::name() const
{
    return dir_name_;
}
