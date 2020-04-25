#ifndef CSVSTORAGE_HPP
#define CSVSTORAGE_HPP

#include "filestorage.hpp"

#include <csvlab.hpp>

class CsvStorage: public FileStorage {
  vector<Organisation> loadOrgs();
  void saveOrgs(const vector<Organisation>& orgs);
  int getNewOrgId();

  vector<Founder> loadFndrs();
  void saveFndrs(const vector<Founder>& orgs);
  int getNewFndrId();

public:
  explicit CsvStorage(const string& dir_name_ = ""): FileStorage(dir_name_) {}
};

#endif    // CSVSTORAGE_HPP
