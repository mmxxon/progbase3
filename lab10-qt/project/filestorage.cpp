#include "filestorage.hpp"

using namespace std;

FileStorage::FileStorage(const string& dir_name) { dir_name_ = dir_name; }
string FileStorage::name() const { return dir_name_; }

bool FileStorage::open(bool type) {
  string typestr = ".csv";
  if (!type) typestr = ".xml";
  orgs_file_.open(name() + "orgs" + typestr);
  fndrs_file_.open(name() + "fndrs" + typestr);
  return isOpen();
}
void FileStorage::close() {
  orgs_file_.close();
  fndrs_file_.close();
}

bool FileStorage::isOpen() const {
  if (orgs_file_.is_open() && fndrs_file_.is_open()) { return 1; }
  return 0;
}
void FileStorage::setName(const string& dir_name) { dir_name_ = dir_name; }

vector<Organisation> FileStorage::getAllOrgs() { return loadOrgs(); }
optional<Organisation> FileStorage::getOrgById(int org_id) {
  vector<Organisation> orgs = loadOrgs();
  for (auto o: orgs) {
    if (o.id == org_id) return o;
  }
  return nullopt;
}
bool FileStorage::updateOrg(const Organisation& org) {
  vector<Organisation> orgs = loadOrgs();
  for (auto& o: orgs) {
    if (o.id == org.id) {
      o = org;
      saveOrgs(orgs);
      return 1;
    }
  }
  return 0;
}
bool FileStorage::removeOrg(int org_id) {
  vector<Organisation> orgs = loadOrgs();
  auto bit = orgs.begin();
  auto eit = orgs.end();
  for (; bit != eit; bit++) {
    if ((*bit).id == org_id) {
      orgs.erase(bit);
      saveOrgs(orgs);
      return 1;
    }
  }
  return 0;
}
int FileStorage::insertOrg(const Organisation& org) {
  int id = getNewOrgId();
  Organisation norg = org;
  norg.id = id;
  vector<Organisation> orgs = loadOrgs();
  orgs.push_back(norg);
  saveOrgs(orgs);
  return id;
}

vector<Founder> FileStorage::getAllFndrs() { return loadFndrs(); }
optional<Founder> FileStorage::getFndrById(int fndr_id) {
  vector<Founder> fndrs = loadFndrs();
  for (auto f: fndrs) {
    if (f.id == fndr_id) return f;
  }
  return nullopt;
}
bool FileStorage::updateFndr(const Founder& fndr) {
  vector<Founder> fndrs = loadFndrs();
  for (auto& f: fndrs) {
    if (f.id == fndr.id) {
      f = fndr;
      saveFndrs(fndrs);
      return 1;
    }
  }
  return 0;
}
bool FileStorage::removeFndr(int fndr_id) {
  vector<Founder> fndrs = loadFndrs();
  auto bit = fndrs.begin();
  auto eit = fndrs.end();
  for (; bit != eit; bit++) {
    if ((*bit).id == fndr_id) {
      fndrs.erase(bit);
      saveFndrs(fndrs);
      return 1;
    }
  }
  return 0;
}
int FileStorage::insertFndr(const Founder& fndr) {
  int id = getNewFndrId();
  Founder nfndr = fndr;
  nfndr.id = id;
  vector<Founder> fndrs = loadFndrs();
  fndrs.push_back(nfndr);
  saveFndrs(fndrs);
  return id;
}
