#include "string_table.hpp"

using namespace std;
StringTable::StringTable(size_t rows, size_t cols) {
  cells_ = new string[rows * cols];
  rows_ = rows;
  cols_ = cols;
}
StringTable::~StringTable() { delete[] cells_; }
size_t StringTable::size_rows() { return rows_; }
size_t StringTable::size_columns() { return cols_; }
string& StringTable::at(int rowIndex, int colIndex) {
  return cells_[rowIndex * cols_ + colIndex];
}
void StringTable::print() {
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_ - 1; j++) {
      cout << cells_[i * cols_ + j] << " ";
    }
    cout << cells_[i * cols_ + cols_ - 1];
    cout << endl;
  }
  cout << endl;
}
