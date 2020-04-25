#include "csvstorage.hpp"

static vector<Organisation> tableToOrg(StringTable& table) {
  vector<Organisation> orgs;
  for (int i = 1; i < (int) table.size_rows(); i++) {
    int row = -1;
    Organisation org;
    org.id = atoi(table.at(i, ++row).c_str());
    org.country = table.at(i, ++row);
    org.label = table.at(i, ++row);
    org.founders = table.at(i, ++row);
    orgs.push_back(org);
  }
  return orgs;
}
static vector<Founder> tableToFndr(StringTable& table) {
  vector<Founder> fndrs;
  for (int i = 1; i < (int) table.size_rows(); i++) {
    int row = -1;
    Founder fndr;
    fndr.id = atoi(table.at(i, ++row).c_str());
    fndr.name = table.at(i, ++row);
    fndr.age = atoi(table.at(i, ++row).c_str());
    fndr.wealth = table.at(i, ++row);
    fndrs.push_back(fndr);
  }
  return fndrs;
}
static StringTable OrgToTable(vector<Organisation> orgs) {
  StringTable table {orgs.size() + 1, 4};
  table.at(0, 0) = "id";
  table.at(0, 1) = "country";
  table.at(0, 2) = "label";
  table.at(0, 3) = "founder(s)";
  for (int i = 0; i < (int) orgs.size(); i++) {
    int row = -1;
    table.at(1 + i, ++row) = to_string(orgs[i].id);
    table.at(1 + i, ++row) = orgs[i].country;
    table.at(1 + i, ++row) = orgs[i].label;
    table.at(1 + i, ++row) = orgs[i].founders;
  }
  return table;
}
static StringTable FndrToTable(vector<Founder> fndrs) {
  StringTable table {fndrs.size() + 1, 4};
  table.at(0, 0) = "id";
  table.at(0, 1) = "name";
  table.at(0, 2) = "age";
  table.at(0, 3) = "wealth";
  for (int i = 0; i < (int) fndrs.size(); i++) {
    int row = -1;
    table.at(1 + i, ++row) = to_string(fndrs[i].id);
    table.at(1 + i, ++row) = fndrs[i].name;
    table.at(1 + i, ++row) = to_string(fndrs[i].age);
    table.at(1 + i, ++row) = fndrs[i].wealth;
  }
  return table;
}

vector<Organisation> CsvStorage::loadOrgs() {
  if (!open(1)) {
    fprintf(stderr, "Error on opening");
    exit(1);
  }
  string s, temp;
  while (getline(orgs_file_, temp)) {
    s.append(temp + "\n");
  }
  close();
  StringTable table = Csv_parse(s);
  return tableToOrg(table);
}
void CsvStorage::saveOrgs(const vector<Organisation>& orgs) {
  StringTable table = OrgToTable(orgs);
  string str = Csv_toString(table);
  if (!open(1)) {
    fprintf(stderr, "Error on opening");
    exit(1);
  }
  close();
  orgs_file_.open(name() + "orgs.csv", ios::out);
  orgs_file_ << str;
  close();
}
int CsvStorage::getNewOrgId() {
  fstream id;
  int tmp = 0;
  id.open(name() + "orgs_id", ios::in);
  if (!id.is_open()) {
    vector<Organisation> orgs = getAllOrgs();
    for (auto& i: orgs) {
      if (i.id >= tmp) tmp = i.id + 1;
    }
  } else {
    id >> tmp;
    id.close();
    id.open(name() + "orgs_id", ios::out);
    id << ++tmp;
    id.close();
  }
  return tmp;
}

vector<Founder> CsvStorage::loadFndrs() {
  if (!open(1)) {
    fprintf(stderr, "Error on opening");
    exit(1);
  }
  string s, temp;
  while (getline(fndrs_file_, temp)) {
    s.append(temp + "\n");
  }
  close();
  StringTable table = Csv_parse(s);
  return tableToFndr(table);
}
void CsvStorage::saveFndrs(const vector<Founder>& fndr) {
  StringTable table = FndrToTable(fndr);
  string str = Csv_toString(table);
  if (!open(1)) {
    fprintf(stderr, "Error on opening");
    exit(1);
  }
  close();
  fndrs_file_.open(name() + "fndrs.csv", ios::out);
  fndrs_file_ << str;
  close();
}
int CsvStorage::getNewFndrId() {
  fstream id;
  int tmp = 0;
  id.open(name() + "fndrs_id", ios::in | ios::out);
  if (!id.is_open()) {
    vector<Founder> fndrs = getAllFndrs();
    for (auto& i: fndrs) {
      if (i.id >= tmp) tmp = i.id + 1;
    }
  } else {
    id >> tmp;
    id << ++tmp;
    id.close();
  }
  return tmp;
}
