#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "Matrix.hpp"

#include <fstream>

#include "../doctest.h"

TEST_CASE("Constructor") {
  abc::Matrix<int> m(2, 3, 0);
  CHECK(m.getMatrix()[1].size() == 3);
  CHECK(m.getMatrix().size() == 2);
}
TEST_CASE("Testing with another type") {
  abc::Matrix<double> m(2, 3, 0.1);
  CHECK(m.getMatrix()[1].size() == 3);
  CHECK(m.getMatrix().size() == 2);
  CHECK(m.getMatrix()[0][1] == 0.1);
}
TEST_CASE("Matrix setElement") {
  abc::Matrix<int> m(2, 3, 0);

  SUBCASE("Setting Element - singol element") {
    m.setElement(0, 0, 1);
    CHECK(m.getMatrix()[0][0] == 1);
  }
  SUBCASE("Setting Element - exception") {
    CHECK_THROWS_WITH_AS(
        m.setElement(3, 1, 1),
        "Point is not inside the matrix, please provide valid coordinates!",
        std::runtime_error);
  }
}
TEST_CASE("Matrix saveOnFile") {
  const std::string filepath = "test_output.txt";

  abc::Matrix<int> m(2, 3, 0);
  m.setElement(0, 0, 1);
  m.setElement(0, 1, 2);
  m.setElement(0, 2, 3);
  m.setElement(1, 0, 4);
  m.setElement(1, 1, 5);
  m.setElement(1, 2, 6);

  m.saveOnFile(filepath);
  std::ifstream in(filepath);
  REQUIRE(in.is_open());
  std::string line;
  std::getline(in, line);
  CHECK(line == "1 2 3 ");
  std::getline(in, line);
  CHECK(line == "4 5 6 ");
  in.close();
}
TEST_CASE("Matrix setRow") {
  abc::Matrix<int> m(3, 4, 0);

  SUBCASE("Matrix setRow - with valid input") {
    std::vector<int> new_row = {1, 2, 3, 4};
    m.setRow(new_row, 1);
    CHECK(m.getMatrix()[1] == new_row);
  }
  SUBCASE("Matrix setRow - with shorter row with matrix size fixed") {
    std::vector<int> new_row = {5, 6};
    m.setRow(new_row, 2);

    CHECK(m.getMatrix()[2][0] == 5);
    CHECK(m.getMatrix()[2][1] == 6);
    CHECK(m.getMatrix()[2][2] == 0);
    CHECK(m.getMatrix()[2][3] == 0);
  }
  SUBCASE("Matrix setRow - throws on too long row") {
    std::vector<int> tooLong = {1, 2, 3, 4, 5};
    CHECK_THROWS_WITH_AS(
        m.setRow(tooLong, 0),
        "New line is too long, please check its size and retry",
        std::runtime_error);
  }
  SUBCASE("Matrix setRow - throws on invalid row index") {
    std::vector<int> validRow = {1, 2, 3, 4};
    CHECK_THROWS_WITH_AS(m.setRow(validRow, 10), "Index is out of bounds!",
                         std::runtime_error);
  }
}
TEST_CASE("Matrix loadMatrixFromFile") {
  const std::string test_filepath = "matrix_input.txt";

  std::ofstream out(test_filepath);
  SUBCASE(
      "Matrix loadMatrixFromFile - reading file with matrix not initialized") {
    out << "1 2 3\n";
    out << "4 5 6\n";
    out.close();
    abc::Matrix<int> m;
    m.loadMatrixFromFile(test_filepath);

    CHECK(m.getMatrix()[0][0] == 1);
    CHECK(m.getMatrix()[0][1] == 2);
    CHECK(m.getMatrix()[0][2] == 3);
    CHECK(m.getMatrix()[1][0] == 4);
    CHECK(m.getMatrix()[1][1] == 5);
    CHECK(m.getMatrix()[1][2] == 6);
  }
  SUBCASE(
      "Matrix loadMatrixFromFile - reading file with matrix initialized all "
      "same "
      "dimension") {
    out << "1 2 3\n";
    out << "4 5 6\n";
    out.close();
    abc::Matrix<int> m(2, 3, 0);
    m.loadMatrixFromFile(test_filepath);

    CHECK(m.getMatrix()[0][0] == 1);
    CHECK(m.getMatrix()[0][1] == 2);
    CHECK(m.getMatrix()[0][2] == 3);
    CHECK(m.getMatrix()[1][0] == 4);
    CHECK(m.getMatrix()[1][1] == 5);
    CHECK(m.getMatrix()[1][2] == 6);
  }
  SUBCASE(
      "Matrix loadMatrixFromFile - reading file with matrix initilize, adding "
      "row") {
    out << "1 2 3\n";
    out << "4 5 6\n";
    out << "7 8 9\n";
    out.close();
    abc::Matrix<int> m;
    m.loadMatrixFromFile(test_filepath);

    CHECK(m.getMatrix()[0][0] == 1);
    CHECK(m.getMatrix()[0][1] == 2);
    CHECK(m.getMatrix()[0][2] == 3);
    CHECK(m.getMatrix()[1][0] == 4);
    CHECK(m.getMatrix()[1][1] == 5);
    CHECK(m.getMatrix()[1][2] == 6);
    CHECK(m.getMatrix()[2][0] == 7);
    CHECK(m.getMatrix()[2][1] == 8);
    CHECK(m.getMatrix()[2][2] == 9);
  }
  SUBCASE(
      "Matrix loadMatrixFromFile - reading file with matrix initilized odd "
      "dimension") {
    out << "1 2 3\n";
    out << "4 5\n";
    out.close();
    abc::Matrix<int> m(2, 3, 0);
    m.loadMatrixFromFile(test_filepath);

    CHECK(m.getMatrix()[0][0] == 1);
    CHECK(m.getMatrix()[0][1] == 2);
    CHECK(m.getMatrix()[0][2] == 3);
    CHECK(m.getMatrix()[1][0] == 4);
    CHECK(m.getMatrix()[1][1] == 5);
    CHECK(m.getMatrix()[1][2] == 0);
  }
  SUBCASE(
      "Matrix loadMatrixFromFile - reading file with matrix not initilized odd "
      "dimension") {
    out << "1 2 3\n";
    out << "4 5\n";
    out.close();
    abc::Matrix<int> m;
    m.loadMatrixFromFile(test_filepath);
    CHECK(m.getMatrix()[0][0] == 1);
    CHECK(m.getMatrix()[0][1] == 2);
    CHECK(m.getMatrix()[0][2] == 3);
    CHECK(m.getMatrix()[1][0] == 4);
    CHECK(m.getMatrix()[1][1] == 5);
  }
  SUBCASE("Matrix loadMatrixFromFile - matrix initilized getting exception") {
    out << "1 2 3 6\n";
    out << "4 5\n";
    out.close();
    abc::Matrix<int> m(2, 3, 0);
    CHECK_THROWS_WITH_AS(
        m.loadMatrixFromFile(test_filepath),
        "New line is too long, please check its size and retry",
        std::runtime_error);
  }
  SUBCASE("Matrix loadMatrixFromFile - filepath corrupted") {
    out << "1 2 3\n";
    out << "4 5\n";
    out.close();
    abc::Matrix<int> m(2, 3, 0);
    CHECK_THROWS_WITH_AS(m.loadMatrixFromFile("nonExistingfile.txt"),
                         "Cannot open the file!", std::runtime_error);
  }
  SUBCASE("Matrix loadMatrixFromFile - matrix size fixed getting exception") {
    out << "1 2 3\n";
    out << "4 5 6\n";
    out << "4 5 6\n";
    out << "4 5 6\n";
    out.close();
    abc::Matrix<int> m(2, 3, 0);
    m.setFixedSize(true);
    CHECK_THROWS_WITH_AS(m.loadMatrixFromFile(test_filepath),
                         "Cannot append new rows: matrix has a fixed size!",
                         std::runtime_error);
  }
}
TEST_CASE("Matrix getElement") {
  abc::Matrix<int> m(3, 4, 0);
  CHECK(m.getElement(1, 1) == 0);
  CHECK_THROWS_WITH_AS(m.getElement(100, 1),
                       "Indices are overcoming matrix dimension",
                       std::runtime_error);
}
TEST_CASE("Matrix append") {
  abc::Matrix<int> m;

  SUBCASE("Matrix append - Append valid row to existing matrix") {
    std::vector<int> row1 = {1, 2, 3};
    std::vector<int> row2 = {4, 5, 6};
    m.append(row1);
    m.append(row2);
    CHECK(m.getMatrix().size() == m.size());
    CHECK(m.getMatrix().size() == 2);
    CHECK(m.getMatrix()[0] == row1);
    CHECK(m.getMatrix()[1] == row2);
  }
  SUBCASE("Matrix append - Append empty vector throws") {
    std::vector<int> empty;
    CHECK_THROWS_WITH_AS(m.append(empty), "Your vector is empty!",
                         std::runtime_error);
  }
  SUBCASE("Matrix append - Append with mismatched size throws") {
    std::vector<int> row1 = {1, 2, 3};
    std::vector<int> rowWrong = {9, 9};  // dimensione diversa
    m.append(row1);
    CHECK_THROWS_WITH_AS(m.append(rowWrong),
                         "Vector and matrix sizes do not match!",
                         std::runtime_error);
  }
  SUBCASE("Matrix append - Append to a fixed sized array") {
    abc::Matrix<int> m1(2, 3, 0);
    std::vector<int> row1 = {1, 2, 3};
    m1.setFixedSize(true);
    CHECK_THROWS_WITH_AS(m1.append(row1),
                         "Cannot append new rows: matrix has a fixed size!",
                         std::runtime_error);
  }
}
TEST_CASE("Matrix nElment")
{
  abc::Matrix m(2, 2, 0);
  CHECK(m.NumberOfElement() == 4);
}