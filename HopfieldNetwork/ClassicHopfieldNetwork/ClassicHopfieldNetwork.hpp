#ifndef HOPFIELDNEURALNETWORK_CLASSICHOPFIELDNETWORK_H
#define HOPFIELDNEURALNETWORK_CLASSICHOPFIELDNETWORK_H

#include "../Matrix/Matrix.hpp"

namespace abc {
class ClassicHopfieldNetwork {
 private:
  Matrix<double> weightMatrix_;
  std::vector<int> originalPattern_;

  void checkPatternDimension(const std::vector<int>& pattern) const;
  double energyPerElement(size_t i, const std::vector<int>& pattern) const;

 public:
  // costructor
  ClassicHopfieldNetwork() {}
  ClassicHopfieldNetwork(std::size_t dimension);

  //working with memory
  void learnPattern(const std::vector<int>& pattern);
  void save(const std::string& filepath) const;
  void loadMemory(const std::string& filepath);

  // elaborator
  Matrix<double>& getMatrix() { return weightMatrix_; }

  // getter
  const Matrix<double>& getMatrix() const;

  // PatternUpdater
  bool restorePattern(std::vector<int>& pattern);
  bool restorePattern_withAnnealing(std::vector<int>& pattern, int n);

  // annealing functions
  double totalEnergy(const std::vector<int>& pattern) const;
  bool probability(double dE, double temp) const;
  double CoolingSchedule(int iter) const;
};
}  // namespace abc

#endif