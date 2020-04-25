#ifndef STRING_TABLE_H
#define STRING_TABLE_H

#include <iostream>
#include <string>
using namespace std;
class StringTable {
  string* cells_;
  size_t rows_;
  size_t cols_;

public:
  StringTable(size_t rows, size_t cols);
  ~StringTable();
  size_t size_rows();
  size_t size_columns();
  string& at(int rowIndex, int colIndex);
  void print();
};

#endif    // STRING_TABLE_H
