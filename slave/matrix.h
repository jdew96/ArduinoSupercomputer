#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


class Matrix {
private:
  std::vector<std::vector<int>>
      _matrix; // the matrix itself, a two-dimensional std::vector
public:
  Matrix(size_t rows = 1, size_t cols = 1)
      : _matrix(std::vector<std::vector<int>>(rows, std::vector<int>(cols))) {
  } // declaration of empty matrix
  explicit Matrix(
      const std::initializer_list<std::initializer_list<int>> &matrixAsAList) {
    _matrix.assign(matrixAsAList.begin(), matrixAsAList.end());
  }

  Matrix(std::string file_name) {
    std::ifstream in(file_name);
    if (!in) {
      std::cout << "Cannot open file.\n";
      exit(-1);
    }
    size_t M, N;
    in >> M;
    in >> N;
    _matrix.resize(M, std::vector<int>(N));
    for (int i = 0; i < M; i++)
      for (int j = 0; j < N; j++)
        in >> _matrix[i][j]; // std::cout << _matrix[i][j] << std::endl;
    in.close();
  }

  Matrix(const Matrix &matrixToCopy) : _matrix(matrixToCopy._matrix) {}

  std::vector<std::vector<int>> fullMatrix() const {
    return _matrix;
  } // returns the full matrix in vector<vector<int>> datatype
  size_t rows() const { return _matrix.size(); }    // returns number of rows
  size_t cols() const { return _matrix[0].size(); } // returns number of columns
  void print() const;                               // prints the matrix
  void getFromFile(std::string);   // returns matrix generated using input file
  void removeColumn(size_t);       // removes a column (probably won't use this)
  int &operator()(size_t, size_t); // access 2d indices i.e.: matrix(x,y)
};

void Matrix::removeColumn(size_t column) {
  if (column >= cols())
    exit(-1);
  else {
    for (size_t i = 0; i < rows(); i++)              // iterate over rows
      _matrix[i].erase(_matrix[i].begin() + column); // delete element in column
  }
}
int &Matrix::operator()(size_t row, size_t column) {
  return _matrix.at(row).at(column);
}

void Matrix::print() const {
  for (size_t i = 0; i < rows(); i++) { // iterate over rows
    printf("[ %d", _matrix[i][0]);
    for (size_t j = 1; j < cols(); j++) // iterate over columns
      printf("\t%d", _matrix[i][j]);
    printf(" ]\n");
  }
}
