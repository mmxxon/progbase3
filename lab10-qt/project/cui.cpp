#include "cui.hpp"

using namespace std;

void Cui::OrgsMainMenu() {
  system("clear");
  vector<Organisation> orgs = storage_->getAllOrgs();
  int l1 = 3;
  for (auto& i: orgs) {
    if ((int) to_string(i.id).length() > l1) {
      l1 = to_string(i.id).length() + 3;
    }
  }
  cout << yel_f << left << setw(l1) << "id" << yel_f "label" res << endl;
  for (auto& org: orgs) {
    cout << cya_f << left << setw(l1) << org.id << mag_f << org.label << res
         << endl;
  }
  cout << cont << endl;
  getch();
  system("clear");
}
void Cui::OrgsMenu(int org_id) {
  system("clear");
  optional<Organisation> org = storage_->getOrgById(org_id);
  if (!org) {
    cout << red_f "Wrong id" res << endl;
    cout << cont << endl;
    getch();
    return;
  }
  int l1(2), l2(10), l3(8);
  if ((int) to_string(org.value().id).length() >= l1)
    l1 = to_string(org.value().id).length() + 3;
  if ((int) org.value().country.length() >= l2)
    l2 = org.value().country.length() + 3;
  if ((int) org.value().label.length() >= l3)
    l3 = org.value().label.length() + 3;
  cout << cya_f "Found organisation:" res << endl;
  cout << yel_f << left << setw(l1) << "id" << yel_f << left << setw(l2)
       << "country" << yel_f << left << setw(l3) << "label" << yel_f << left
       << "founders" res << endl;
  cout << blu_f << left << setw(l1) << org.value().id << mag_f << left
       << setw(l2) << org.value().country << blu_f << left << setw(l3)
       << org.value().label << mag_f << left << org.value().founders << res
       << endl;
  cout << cont << endl;
  getch();
  system("clear");
}
void Cui::OrgsUpdateMenu(int org_id) {
  int key;
  optional<Organisation> org = storage_->getOrgById(org_id);
  if (!org) {
    system("clear");
    cout << red_f "Wrong id" res << endl;
    cout << cont << endl;
    getch();
    return;
  }
  while (1) {
    system("clear");
    Organisation o = org.value();
    cout << yel_f << left << setw(10) << "Id: " cya_f + to_string(o.id) << yel_f
         << right << setw(45) << "Label: " mag_f + o.label << res << endl
         << yel_f << left << setw(10) << "Country: " cya_f + o.country << yel_f
         << right << setw(45) << "Founders: " mag_f + o.founders << res << endl;
    cout << yel_f "Actions" << endl << endl;
    cout << yel_f "[1]" res " Change country" << endl;
    cout << yel_f "[2]" res " Change label" << endl;
    cout << yel_f "[3]" res " Change founder(s)" << endl;
    cout << yel_f "[4]" res " Return back" << endl;
    do {
      key = getch();
    } while (49 > key || 52 < key);
    system("clear");
    if (key == 52) {
      break;
    } else if (key == 49) {
      cout << yel_f "New country" << endl << mag_f "> " blu_f;
      getline(cin, o.country);
    } else if (key == 50) {
      cout << yel_f "New label" << endl << mag_f "> " blu_f;
      getline(cin, o.label);
    } else if (key == 51) {
      cout << yel_f "New founders info " << endl << mag_f "> " blu_f;
      getline(cin, o.founders);
    }
    storage_->updateOrg(o);
    org = storage_->getOrgById(o.id);
    o = org.value();
  }
}
void Cui::OrgsDeleteMenu(int org_id) {
  optional<Organisation> org = storage_->getOrgById(org_id);
  system("clear");
  if (!storage_->removeOrg(org_id)) {
    cout << red_f "Wrong id" res << endl;
  } else {
    cout << yel_f "Deleted" mag_f " > " red_f << org.value().label << res
         << endl;
  }
  cout << cont << endl;
  getch();
  system("clear");
  return;
}
void Cui::OrgsCreateMenu() {
  system("clear");
  Organisation org;
  cout << yel_f "Label " << endl << mag_f "> " blu_f;
  getline(cin, org.label);
  cout << res;
  system("clear");
  cout << yel_f "Country " << endl << mag_f "> " blu_f;
  getline(cin, org.country);
  cout << res;
  system("clear");
  cout << yel_f "Founder(s) " << endl << mag_f "> " blu_f;
  getline(cin, org.founders);
  cout << res;
  system("clear");
  cout << yel_f "NEW ID:" << blu_f << storage_->insertOrg(org) << res << endl;
  cout << cont << endl;
  getch();
}

void Cui::FndrsMainMenu() {
  system("clear");
  vector<Founder> fndrs = storage_->getAllFndrs();
  int l1 = 3;
  for (auto& i: fndrs) {
    if ((int) to_string(i.id).length() > l1) {
      l1 = to_string(i.id).length() + 3;
    }
  }
  cout << yel_f << left << setw(l1) << "id" << yel_f "name" res << endl;
  for (auto& fndr: fndrs) {
    cout << cya_f << left << setw(l1) << fndr.id << mag_f << fndr.name << res
         << endl;
  }
  cout << cont << endl;
  getch();
  system("clear");
}
void Cui::FndrsMenu(int fndr_id) {
  system("clear");
  optional<Founder> fndr = storage_->getFndrById(fndr_id);
  if (!fndr) {
    cout << red_f "Wrong id" res << endl;
    cout << cont << endl;
    getch();
    return;
  }
  int l1(2), l2(10), l3(8);
  if ((int) to_string(fndr.value().id).length() >= l1)
    l1 = to_string(fndr.value().id).length() + 3;
  if ((int) fndr.value().name.length() >= l2)
    l2 = fndr.value().name.length() + 3;
  if ((int) to_string(fndr.value().age).length() >= l3)
    l3 = to_string(fndr.value().age).length() + 3;
  cout << cya_f "Found organisation:" res << endl;
  cout << yel_f << left << setw(l1) << "id" << yel_f << left << setw(l2)
       << "name" << yel_f << left << setw(l3) << "age" << yel_f << left
       << "wealth" << res << endl;
  cout << blu_f << left << setw(l1) << fndr.value().id << mag_f << left
       << setw(l2) << fndr.value().name << blu_f << left << setw(l3)
       << fndr.value().age << mag_f << left << fndr.value().wealth << res
       << endl;
  cout << cont << endl;
  getch();
  system("clear");
}
void Cui::FndrsUpdateMenu(int fndr_id) {
  int key;
  optional<Founder> fndr = storage_->getFndrById(fndr_id);
  if (!fndr) {
    system("clear");
    cout << red_f "Wrong id" res << endl;
    cout << cont << endl;
    getch();
    return;
  }
  while (1) {
    system("clear");
    Founder f = fndr.value();
    cout << yel_f << left << setw(10) << "Id: " cya_f + to_string(f.id) << yel_f
         << right << setw(45) << "Name: " mag_f + f.name << res << endl
         << yel_f << left << setw(10) << "Age: " cya_f + to_string(f.age)
         << yel_f << right << setw(45) << "Wealth: " mag_f + f.wealth << res
         << endl;
    cout << yel_f "Actions" << endl << endl;
    cout << yel_f "[1]" res " Change name" << endl;
    cout << yel_f "[2]" res " Change age" << endl;
    cout << yel_f "[3]" res " Change wealth" << endl;
    cout << yel_f "[4]" res " Return back" << endl;
    do {
      key = getch();
    } while (49 > key || 52 < key);
    system("clear");
    if (key == 52) {
      break;
    } else if (key == 49) {
      cout << yel_f "New name" << endl << mag_f "> " blu_f;
      getline(cin, f.name);
    } else if (key == 50) {
      cout << yel_f "New age" << endl << mag_f "> " blu_f;
      string tmp;
      getline(cin, tmp);
      if (isdigit(tmp[0]))
        f.age = stoi(tmp);
      else {
        cout << red_f "Wrong age" res << endl;
        cout << cont << endl;
        getch();
        return;
      }
    } else if (key == 51) {
      cout << yel_f "New wealth" << endl << mag_f "> " blu_f;
      getline(cin, f.wealth);
    }
    storage_->updateFndr(f);
    fndr = storage_->getFndrById(f.id);
    f = fndr.value();
  }
}
void Cui::FndrsDeleteMenu(int fndr_id) {
  optional<Founder> fndr = storage_->getFndrById(fndr_id);
  system("clear");
  if (!storage_->removeFndr(fndr_id)) {
    cout << red_f "Wrong id" res << endl;
  } else {
    cout << yel_f "Deleted" mag_f " > " red_f << fndr.value().name << res
         << endl;
  }
  cout << cont << endl;
  getch();
  system("clear");
  return;
}
void Cui::FndrsCreateMenu() {
  system("clear");
  Founder fndr;
  cout << yel_f "Name " << endl << mag_f "> " blu_f;
  getline(cin, fndr.name);
  cout << res;
  system("clear");
  cout << yel_f "Age " << endl << mag_f "> " blu_f;
  string tmp;
  getline(cin, tmp);
  if (isdigit(tmp[0]))
    fndr.age = stoi(tmp);
  else {
    cout << red_f "Wrong age" res << endl;
    cout << cont << endl;
    getch();
    return;
  }
  system("clear");
  cout << yel_f "Wealth " << endl << mag_f "> " blu_f;
  getline(cin, fndr.wealth);
  cout << res;
  system("clear");
  cout << yel_f "NEW ID:" << blu_f << storage_->insertFndr(fndr) << res << endl;
  cout << cont << endl;
  getch();
}

void Cui::show() {
  int key;
  string tmp;
  int id;
  while (1) {
    cout << yel_f "MAIN MENU:" res << endl;
    cout << cya_f "[1]" res " Organisations" << endl;
    cout << cya_f "[2]" res " Founders" << endl;
    cout << cya_f "[3]" res " Exit" << endl;
    do {
      key = getch();
    } while (49 > key || 51 < key);
    if (key == 51) {
      system("clear");
      exit(0);
    } else if (key == 49) {
      while (1) {
        system("clear");
        cout << yel_f "ORGANISATIONS MENU" res << endl;
        cout << cya_f "[1]" res " Show all" << endl;
        cout << cya_f "[2]" res " Actions with single item" << endl;
        cout << cya_f "[3]" res " Add new" << endl;
        cout << cya_f "[4]" res " Return back" << endl;
        do {
          key = getch();
        } while (49 > key || 52 < key);
        if (key == 52) {
          system("clear");
          break;
        } else if (key == 49) {
          OrgsMainMenu();
        } else if (key == 50) {
          system("clear");
          cout << yel_f "Enter id" << endl << blu_f;
          getline(cin, tmp);
          cout << res;
          if (isdigit(tmp[0])) {
            id = stoi(tmp);
            while (1) {
              system("clear");
              cout << yel_f "[ID:" red_f << id << yel_f "]" res << endl;
              cout << cya_f "[1]" res " Info" << endl;
              cout << cya_f "[2]" res " Change field" << endl;
              cout << cya_f "[3]" res " Delete" << endl;
              cout << cya_f "[4]" res " Return back" << endl;
              do {
                key = getch();
              } while (49 > key || 52 < key);
              if (key == 49)
                OrgsMenu(id);
              else if (key == 50)
                OrgsUpdateMenu(id);
              else if (key == 51) {
                OrgsDeleteMenu(id);
                break;
              } else if (key == 52) {
                system("clear");
                break;
              }
            }
          } else {
            cout << red_f "Wrong input" res << endl;
            cout << cont << endl;
            getch();
          }
        } else if (key == 51) {
          OrgsCreateMenu();
        }
      }
    } else if (key == 50) {
      while (1) {
        system("clear");
        cout << yel_f "FOUNDERS MENU" res << endl;
        cout << cya_f "[1]" res " Show all" << endl;
        cout << cya_f "[2]" res " Actions with single item" << endl;
        cout << cya_f "[3]" res " Add new" << endl;
        cout << cya_f "[4]" res " Return back" << endl;
        do {
          key = getch();
        } while (49 > key || 52 < key);
        if (key == 52) {
          system("clear");
          break;
        } else if (key == 49) {
          FndrsMainMenu();
        } else if (key == 50) {
          system("clear");
          cout << yel_f "Enter id" << endl << blu_f;
          getline(cin, tmp);
          if (isdigit(tmp[0])) {
            id = stoi(tmp);
            while (1) {
              system("clear");
              cout << yel_f "[ID:" red_f << id << yel_f "]" res << endl;
              cout << cya_f "[1]" res " Info" << endl;
              cout << cya_f "[2]" res " Change field" << endl;
              cout << cya_f "[3]" res " Delete" << endl;
              cout << cya_f "[4]" res " Return back" << endl;
              do {
                key = getch();
              } while (49 > key || 52 < key);
              if (key == 49)
                FndrsMenu(id);
              else if (key == 50)
                FndrsUpdateMenu(id);
              else if (key == 51) {
                FndrsDeleteMenu(id);
                break;
              } else if (key == 52) {
                system("clear");
                break;
              }
            }
          } else {
            cout << red_f "Wrong input" res << endl;
            cout << cont << endl;
            getch();
          }
        } else if (key == 51) {
          FndrsCreateMenu();
        }
      }
    }
  }
}
char getch(void) {
  char buf = 0;
  struct termios old = {0};
  fflush(stdout);
  if (tcgetattr(0, &old) < 0) perror("tcsetattr()");
  old.c_lflag &= ~ICANON;
  old.c_lflag &= ~ECHO;
  old.c_cc[VMIN] = 1;
  old.c_cc[VTIME] = 0;
  if (tcsetattr(0, TCSANOW, &old) < 0) perror("tcsetattr ICANON");
  if (read(0, &buf, 1) < 0) perror("read()");
  old.c_lflag |= ICANON;
  old.c_lflag |= ECHO;
  if (tcsetattr(0, TCSADRAIN, &old) < 0) perror("tcsetattr ~ICANON");
  return buf;
}
