#ifndef HOPFIELDNEURALNETWORK_MODERNHOPFIELDNETWORK_H
#define HOPFIELDNEURALNETWORK_MODERNHOPFIELDNETWORK_H

#include "../Matrix/Matrix.hpp"

namespace abc {
class ModernHopfieldNetwork {
 private:
  Matrix<int> patternMatrix_;
  std::size_t dim_{10000};
  double temp_0_{500};
 

  double dot(const std::vector<int>& a,
                                    const std::vector<int>& b) const;

 public:
  ModernHopfieldNetwork();
  ModernHopfieldNetwork(int dimension);

  void learnPattern(const std::vector<int>& pattern);
  void save(const std::string& filepath) const;
  void loadMemory(const std::string& filepath);

  // getter
  const Matrix<int>& getMatrix() const;
  double totalSystemEnergy() const;

  //setter
  void setTemp0(double energy);


//modern-classic
  bool restorePattern(std::vector<int>& input);

//modern-annealing
  bool probability(double dE, double temp) const ;
  double CoolingSchedule(int iter) const ;
  bool restorePattern_withAnnealing(
    std::vector<int>& pattern, int n) const ;
  double energyPerState(const std::vector<int>& state, int n) const;

};
}  // namespace abc

#endif