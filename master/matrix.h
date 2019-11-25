#include <StandardCplusplus.h>
#include <vector>

class Matrix {
private:
  std::vector<std::vector<float>>
      _matrix; // the matrix itself, a two-dimensional std::vector
public:
  Matrix(size_t rows = 1, size_t cols = 1)
      : _matrix(std::vector<std::vector<float>>(rows, std::vector<float>(cols))) {
  } // declaration of empty matrix
  explicit Matrix(
      const std::initializer_list<std::initializer_list<float>> &matrixAsAList) {
    _matrix.assign(matrixAsAList.begin(), matrixAsAList.end());
  }

  size_t rows() const { return _matrix.size(); }    // returns number of rows
  size_t cols() const { return _matrix[0].size(); } // returns number of columns
  void print() const;                               // prints the matrix
  int &operator()(size_t, size_t); // access 2d indices i.e.: matrix(x,y)
};

int &Matrix::operator()(size_t row, size_t column) {
  //return _matrix.at(row).at(column);
    return _matrix[row][column];
}

void Matrix::print() const {
  for (size_t i = 0; i < rows(); i++) { // iterate over rows
    printf("[ %d", _matrix[i][0]);
    for (size_t j = 1; j < cols(); j++) // iterate over columns
      printf("\t%d", _matrix[i][j]);
    printf(" ]\n");
  }
}
