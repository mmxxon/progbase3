#ifndef CUI_H
#define CUI_H
#include "csvstorage.hpp"
#include "xmlstorage.hpp"

#include <iomanip>
#include <termios.h>
#include <unistd.h>
#include <vector>

#define blu_f "\033[32m"
#define yel_f "\033[33m"
#define cya_f "\033[36m"
#define red_f "\033[31m"
#define mag_f "\033[35m"
#define res "\033[0m"
#define cont "\n\033[32mPress any key to continue..\033[0m"

using namespace std;

class Cui {
  FileStorage* const storage_;
  void OrgsMainMenu();
  void OrgsMenu(int org_id);
  void OrgsUpdateMenu(int org_id);
  void OrgsDeleteMenu(int org_id);
  void OrgsCreateMenu();

  void FndrsMainMenu();
  void FndrsMenu(int fndr_id);
  void FndrsUpdateMenu(int fndr_id);
  void FndrsDeleteMenu(int fndr_id);
  void FndrsCreateMenu();

public:
  Cui(FileStorage* storage): storage_ {storage} {}
  void show();
};

char getch();

#endif    // CUI_H
