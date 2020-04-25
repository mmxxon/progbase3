#ifndef XMLSTORAGE_HPP
#define XMLSTORAGE_HPP

#include "filestorage.hpp"

class XmlStorage: public FileStorage {
  vector<Organisation> loadOrgs();
  void saveOrgs(const vector<Organisation>& orgs);
  int getNewOrgId();

  vector<Founder> loadFndrs();
  void saveFndrs(const vector<Founder>& fndrs);
  int getNewFndrId();

public:
  explicit XmlStorage(const string& dir_name_ = ""): FileStorage(dir_name_) {}
};

#endif    // XMLSTORAGE_HPP
