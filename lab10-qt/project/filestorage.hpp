#ifndef FILESTORAGE_H
#define FILESTORAGE_H

#include "founder.hpp"
#include "organisation.hpp"
#include "string_table.hpp"

#include <fstream>
#include <optional>
#include <vector>

using namespace std;

class FileStorage {
  string dir_name_;

protected:
  fstream orgs_file_;
  fstream fndrs_file_;

  virtual vector<Organisation> loadOrgs() = 0;
  virtual void saveOrgs(const vector<Organisation>& orgs) = 0;
  virtual int getNewOrgId() = 0;

  virtual vector<Founder> loadFndrs() = 0;
  virtual void saveFndrs(const vector<Founder>& fndrs) = 0;
  virtual int getNewFndrId() = 0;

public:
  explicit FileStorage(const string& dir_name = "");
  virtual ~FileStorage() {}

  void setName(const string& dir_name);
  string name() const;
  bool isOpen() const;
  bool open(bool type = 0);
  void close();

  vector<Organisation> getAllOrgs();
  optional<Organisation> getOrgById(int org_id);
  bool updateOrg(const Organisation& org);
  bool removeOrg(int org_id);
  int insertOrg(const Organisation& org);

  // courses
  vector<Founder> getAllFndrs(void);
  optional<Founder> getFndrById(int fndr_id);
  bool updateFndr(const Founder& fndr);
  bool removeFndr(int fndr_id);
  int insertFndr(const Founder& fndr);
};

#endif    // FILESTORAGE_H
