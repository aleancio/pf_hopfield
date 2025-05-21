#ifndef HOPFIELDNEURALNETWORK_MATRIX_H
#define HOPFIELDNEURALNETWORK_MATRIX_H

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace abc {
template <class T>
class Matrix {
 private:
  std::vector<std::vector<T>> matrix_;
  bool fixedSize_{false};

  void checkEmptiness() const  // class invariant
  {
    if (matrix_.size() == 0) {
      throw std::runtime_error("Matrix is empty!");
    }
  }

 public:
  Matrix() {}
  Matrix(std::size_t rows, std::size_t cols, T default_val) {
    std::vector<T> values(cols, default_val);
    for (unsigned int row = 0; row < rows; ++row) {
      matrix_.push_back(values);
    }
  }
 
  // setter
  void setElement(std::size_t i, std::size_t j, T value) {
    checkEmptiness();
    if (i >= matrix_.size() || j >= matrix_[0].size()) {
      throw std::runtime_error(
          "Point is not inside the matrix, please provide valid coordinates!");
    }
    matrix_[i][j] = value;
  }
  void setRow(const std::vector<T> newRow, std::size_t index) {
    checkEmptiness();
    if (index >= matrix_.size()) {
      throw std::runtime_error("Index is out of bounds!");
    }
    if (newRow.size() > matrix_[index].size()) {
      throw std::runtime_error(
          "New line is too long, please check its size and retry");
    }

    std::copy(newRow.begin(), newRow.end(), matrix_[index].begin());
  }
  void setFixedSize(bool a) {
    checkEmptiness();
    fixedSize_ = a;
  }

  // getter
  const auto& getMatrix() const { return matrix_; }
  auto getElement(std::size_t i, std::size_t j) const {
    if (i >= matrix_.size() || j >= matrix_[0].size()) {
      throw std::runtime_error("Indices are overcoming matrix dimension");
    }
    return matrix_[i][j];
  }
  std::size_t size() const { return matrix_.size(); }

  // other methods
  void append(const std::vector<T>& pattern) {
    if (pattern.empty()) {
      throw std::runtime_error("Your vector is empty!");
    }
    if (matrix_.empty()) {
      matrix_.push_back(pattern);
    } else if (pattern.size() == matrix_[0].size()) {
      if (!fixedSize_) {
        matrix_.push_back(pattern);
      } else {
        throw std::runtime_error(
            "Cannot append new rows: matrix has a fixed size!");
      }
    } else {
      throw std::runtime_error("Vector and matrix sizes do not match!");
    }
  }
  bool saveOnFile(const std::string& filepath) const {
    checkEmptiness();
    std::ofstream file(filepath);
    if (!file.is_open()) {
      throw std::runtime_error("Cannot create the file!");
    }
    for (unsigned int i = 0; i < matrix_.size(); ++i) {
      for (unsigned int j = 0; j < matrix_[0].size(); ++j) {
        file << matrix_[i][j] << " ";
      }
      file << "\n";
    }
    file.close();
    return true;
  }
  void loadMatrixFromFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
      throw std::runtime_error("Cannot open the file!");
    }
    std::string l;
    unsigned int i{0};
    std::vector<T> row;
    T num;

    while (std::getline(file, l)) {
      row.clear();
      std::istringstream iss(l);  // ti permette di leggere i valori
      // come fossero da cin
      while (iss >> num) {
        row.push_back(num);
      }
      if (!row.empty()) {
        if (i >= matrix_.size()) {
          if (!fixedSize_) {
            matrix_.push_back(row);
          } else {
            throw std::runtime_error(
                "Cannot append new rows: matrix has a fixed size!");
          }
        } else {
          setRow(row, i);
        }
      }
      ++i;
    }
    file.close();
  }

  size_t NumberOfElement() const {
    checkEmptiness();
    return matrix_.size() * matrix_[0].size();
  }
};

namespace fs = std::filesystem;

//not tested
inline const std::vector<std::string> setDirectory(const std::string& input) {
  std::vector<std::string> images_path;
  if (!fs::exists(input) || !fs::is_directory(input)) {
    throw std::runtime_error("Not a valid directory.");
  }

  for (const auto& entry : fs::directory_iterator(input)) {
    if (entry.path().extension() == ".png" ||
        entry.path().extension() == ".jpg" ||
        entry.path().extension() == ".jpeg") {
      images_path.push_back(entry.path());
    }
  }
  return images_path;
};
}  // namespace abc

#endif