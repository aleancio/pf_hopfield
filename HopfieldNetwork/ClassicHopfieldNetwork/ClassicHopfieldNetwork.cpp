#include "ClassicHopfieldNetwork.hpp"

#include <random>
#include <iostream>

namespace abc {
void ClassicHopfieldNetwork::checkPatternDimension(
    const std::vector<int>& pattern) const {
  if (weightMatrix_.size() !=
      pattern.size()) {  // l'exception la sollevo anche in setElememt 
    throw std::runtime_error(
        "matrix and pattern sizes do not match!");  // necessario per il
                                                    // caricamento da file
  }
}

ClassicHopfieldNetwork::ClassicHopfieldNetwork(std::size_t dimension)
    : weightMatrix_(dimension, dimension, 0) {
  if (dimension == 0) {
    throw std::runtime_error(
        "Invalid pattern dimension, it must be greater than 0");
  }
  weightMatrix_.setFixedSize(true);
}

void ClassicHopfieldNetwork::learnPattern(const std::vector<int>& pattern) {
  checkPatternDimension(pattern);
  for (unsigned int i = 0; i < pattern.size(); ++i) {
    for (unsigned int j = i; j < pattern.size(); ++j) {
      if (i != j) {
        weightMatrix_.setElement(
            i, j,
            weightMatrix_.getElement(i, j) +
                static_cast<double>(pattern[i] * pattern[j]) /
                    static_cast<double>(pattern.size()));
        weightMatrix_.setElement(j, i, weightMatrix_.getElement(i, j));
      }
    }
  }
}

void ClassicHopfieldNetwork::save(const std::string& filepath) const {
  weightMatrix_.saveOnFile(filepath);
}

void ClassicHopfieldNetwork::loadMemory(const std::string& filepath) {
  weightMatrix_.loadMatrixFromFile(filepath);
}

const Matrix<double>& ClassicHopfieldNetwork::getMatrix() const {
  return weightMatrix_;
}

bool ClassicHopfieldNetwork::restorePattern(std::vector<int>& pattern) {
  checkPatternDimension(pattern);
  std::cout << '#' << std::flush;
  for (unsigned int i = 0; i < pattern.size(); ++i) {
    double signSum = 0;
    for (unsigned int j = 0; j < pattern.size(); ++j) {
      signSum += weightMatrix_.getElement(i, j) * pattern[j];
    }
    pattern[i] = signSum > 0.0 ? 1 : -1;
  }
  if (originalPattern_ == pattern) {
    return true;
  }
  originalPattern_ = pattern;
  return false;
}

double ClassicHopfieldNetwork::energyPerElement(
    size_t i, const std::vector<int>& pattern) const {
  double energy = {0.0};
  for (unsigned int k = 0; k < pattern.size(); ++k) {
    energy += static_cast<double>((-0.5) * weightMatrix_.getElement(k, i) *
                                  pattern[i] * pattern[k]);
  }

  return energy;
}

double ClassicHopfieldNetwork::totalEnergy(
    const std::vector<int>& pattern) const {
  double totalEnergy{0.0};

  for (size_t i{0}; i < pattern.size(); ++i) {
    totalEnergy += energyPerElement(i, pattern);
  }

  return totalEnergy;
}

bool ClassicHopfieldNetwork::probability(double dE, double temp) const {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> distrib(0.0, 1.0);

  long double prob = std::exp((-std::abs(dE)/ (temp)));
  double r = distrib(gen);

  return r < prob;
}

double ClassicHopfieldNetwork::CoolingSchedule(int iter) const {
  double temp;
  double temp_0 = 50;
  if (iter == 0) {
    temp = temp_0;
    return temp;
  }

  // temp = static_cast<float>(temp_0 * std::pow(0.8, iter));
  temp = static_cast<float>(temp_0 * (1 / std::log(2 + iter)));

  if (temp < 0.01) {
    temp = 0.0;
  }
  return temp;
}

bool ClassicHopfieldNetwork::restorePattern_withAnnealing(
    std::vector<int>& pattern, int n) {
  checkPatternDimension(pattern);
  std::cout << '#' << std::flush;

  for (unsigned int i = 0; i < pattern.size(); ++i) {
    double signSum = 0;
    for (unsigned int j = 0; j < pattern.size(); ++j) {
      signSum += weightMatrix_.getElement(i, j) * pattern[j];
    }

    double e0 = energyPerElement(i, pattern);
    int candidate = signSum > 0.0 ? 1 : -1;
    pattern[i] = candidate;
    double dE = energyPerElement(i, pattern) - e0;
    if (probability(dE, CoolingSchedule(n))) {
      pattern[i] = candidate;
    } else {
      pattern[i] = -candidate;
    }
  }
  if (originalPattern_ == pattern) {
    return true;
  }
  originalPattern_ = pattern;
  return false;
}
}  // namespace abc
