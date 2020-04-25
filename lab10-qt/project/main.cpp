#include "cui.hpp"

#include <stdio.h> /* defines FILENAME_MAX */
#include <string>
#ifdef __unix__
#include <unistd.h>
#define GetCurrentDir getcwd
#elif defined(_WIN32) || defined(WIN32)
#include <direct.h>
#define GetCurrentDir _getcwd
#endif

#include <iostream>

using namespace std;
auto main() -> int {
  int key = 0;
  char buff[FILENAME_MAX];
  getcwd(buff, FILENAME_MAX);
  string wdir(buff);
  cout << "Current dir: " << endl;
  cout << blu_f + wdir + res " |" << endl;
  for (int i = 0; i < (int) wdir.length() + 1; i++)
    cout << '_';
  cout << '|' << endl;
  cout << "Enter location of files " yel_f
          "(Press RETURN to use ../../data/ )" res
       << endl
       << blu_f;
  string str = "";
  getline(cin, str);
  cout << res;
  if (str == "") str.append("../../data/");
  system("clear");
  cout << yel_f "Choose file type" res << endl;
  cout << cya_f "[1] CSV" res << endl;
  cout << cya_f "[2] XML" res << endl;
  do {
    key = getch();
  } while (49 > key || 50 < key);
  if (key == 49) {
    CsvStorage st {str + "csv/"};
    FileStorage* fs = &st;
    Cui cui {fs};
    system("clear");
    cui.show();
  } else {
    XmlStorage st {str + "xml/"};
    FileStorage* fs = &st;
    Cui cui {fs};
    system("clear");
    cui.show();
  }
}
