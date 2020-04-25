#include "xmlstorage.hpp"

#include <QtDebug>
#include <QtXml>

static QDomElement orgXml(QDomDocument& doc, const Organisation& org) {
  QDomElement elOrg = doc.createElement("orgs");
  elOrg.setAttribute("id", org.id);
  elOrg.setAttribute("country", QString::fromStdString(org.country));
  elOrg.setAttribute("label", QString::fromStdString(org.label));
  elOrg.setAttribute("founders", QString::fromStdString(org.founders));
  return elOrg;
}

static Organisation xmlOrg(QDomElement& elOrg) {
  Organisation org;
  org.id = (elOrg.attribute("id").toInt());
  org.country = elOrg.attribute("country").toStdString();
  org.label = elOrg.attribute("label").toStdString();
  org.founders = elOrg.attribute("founders").toStdString();
  return org;
}

static QDomElement fndrXml(QDomDocument& doc, const Founder& fndr) {
  QDomElement fndrOrg = doc.createElement("fndrs");
  fndrOrg.setAttribute("id", fndr.id);
  fndrOrg.setAttribute("name", QString::fromStdString(fndr.name));
  fndrOrg.setAttribute("age", fndr.age);
  fndrOrg.setAttribute("wealth", QString::fromStdString(fndr.wealth));
  return fndrOrg;
}

static Founder xmlFndr(QDomElement& elFndr) {
  Founder fndr;
  fndr.id = (elFndr.attribute("id").toInt());
  fndr.name = elFndr.attribute("name").toStdString();
  fndr.age = elFndr.attribute("age").toInt();
  fndr.wealth = elFndr.attribute("wealth").toStdString();
  return fndr;
}

void XmlStorage::saveOrgs(const vector<Organisation>& orgs) {
  QDomDocument doc;
  QDomElement root = doc.createElement("org");
  for (const Organisation& org: orgs) {
    QDomElement orgEl = orgXml(doc, org);
    root.appendChild(orgEl);
  }
  doc.appendChild(root);
  if (!open()) {
    fprintf(stderr, "Error on opening");
    exit(1);
  }
  fstream orgs_file_(name() + "orgs.xml", ios::out);
  QString str = doc.toString(2);
  orgs_file_ << str.toStdString();
  close();
}
vector<Organisation> XmlStorage::loadOrgs() {
  if (!open()) {
    fprintf(stderr, "Error on opening");
    exit(1);
  }
  string s, temp;
  while (getline(orgs_file_, temp)) {
    s.append(temp + "\n");
  }
  close();
  QString str = QString::fromStdString(s);
  vector<Organisation> orgs;
  QDomDocument doc;
  QString errorMessage;
  if (!doc.setContent(str, &errorMessage)) {
    qDebug() << "Parse error" << errorMessage;
    exit(1);
  }
  QDomElement root = doc.documentElement();
  QDomNodeList rootCh = root.childNodes();
  for (int i = 0; i < rootCh.length(); i++) {
    QDomNode node = rootCh.at(i);
    QDomElement orgEl = node.toElement();
    Organisation org = xmlOrg(orgEl);
    orgs.push_back(org);
  }
  close();
  return orgs;
}

int XmlStorage::getNewOrgId() {
  int tmp = 0;
  vector<Organisation> orgs = getAllOrgs();
  for (auto& i: orgs) {
    if (i.id >= tmp) tmp = i.id + 1;
  }
  return tmp;
}

void XmlStorage::saveFndrs(const vector<Founder>& fndrs) {
  QDomDocument doc;
  QDomElement root = doc.createElement("fndr");
  for (const Founder& fndr: fndrs) {
    QDomElement fndrEl = fndrXml(doc, fndr);
    root.appendChild(fndrEl);
  }
  doc.appendChild(root);
  if (!open()) {
    fprintf(stderr, "Error on opening");
    exit(1);
  }
  fstream fndrs_file_(name() + "fndrs.xml", ios::out);
  QString str = doc.toString(2);
  fndrs_file_ << str.toStdString();
  close();
}
vector<Founder> XmlStorage::loadFndrs() {
  if (!open()) {
    fprintf(stderr, "Error on opening");
    exit(1);
  }
  string s, temp;
  while (getline(fndrs_file_, temp)) {
    s.append(temp + "\n");
  }
  close();
  QString str = QString::fromStdString(s);
  vector<Founder> fndrs;
  QDomDocument doc;
  QString errorMessage;
  if (!doc.setContent(str, &errorMessage)) {
    qDebug() << "Parse error" << errorMessage;
    exit(1);
  }
  QDomElement root = doc.documentElement();
  QDomNodeList rootCh = root.childNodes();
  for (int i = 0; i < rootCh.length(); i++) {
    QDomNode node = rootCh.at(i);
    QDomElement fndrEl = node.toElement();
    Founder fndr = xmlFndr(fndrEl);
    fndrs.push_back(fndr);
  }
  close();
  return fndrs;
}

int XmlStorage::getNewFndrId() {
  int tmp = 0;
  vector<Founder> fndrs = getAllFndrs();
  for (auto& i: fndrs) {
    if (i.id >= tmp) tmp = i.id + 1;
  }
  return tmp;
}
