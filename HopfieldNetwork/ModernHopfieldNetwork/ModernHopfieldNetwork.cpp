#include "ModernHopfieldNetwork.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <random>

namespace abc {

ModernHopfieldNetwork::ModernHopfieldNetwork() {}
ModernHopfieldNetwork::ModernHopfieldNetwork(int dimension) {
  {
    if (dimension <= 0) {
      throw std::runtime_error(
          "Invalid pattern dimension, it must be greater than 0");
    }

    dim_ = static_cast<unsigned int>(dimension);
  }
}

void ModernHopfieldNetwork::learnPattern(const std::vector<int>& pattern) {
  patternMatrix_.append(pattern);
  dim_ = pattern.size();
}

void ModernHopfieldNetwork::save(const std::string& filepath) const {
  patternMatrix_.saveOnFile(filepath);
}

void ModernHopfieldNetwork::loadMemory(const std::string& filepath) {
  patternMatrix_.loadMatrixFromFile(filepath);
  dim_ = patternMatrix_.getMatrix()[0].size();
}

const Matrix<int>& ModernHopfieldNetwork::getMatrix() const {
  return patternMatrix_;
}

double ModernHopfieldNetwork::totalSystemEnergy() const {
  double sum{0.0};

  for (std::size_t i = 0; i < patternMatrix_.size(); ++i) {
    for (std::size_t j = i + 1; j < patternMatrix_.size(); ++j) {
      const auto& v1 = patternMatrix_.getMatrix()[i];
      const auto& v2 = patternMatrix_.getMatrix()[j];
      assert(v1.size() == v2.size());

      double dot = std::inner_product(v1.begin(), v1.end(), v2.begin(), 0.0);
      sum -= std::exp(dot / std::sqrt(dim_));
    }
  }

  return sum;
}

void ModernHopfieldNetwork::setTemp0(double energy) {
  temp_0_ = std::abs(energy * 4.5);
}

double ModernHopfieldNetwork::dot(const std::vector<int>& a,
                                  const std::vector<int>& b) const {
  double sum = 0;
  for (size_t i = 0; i < dim_; ++i) sum += a[i] * b[i];
  return sum;
}

bool ModernHopfieldNetwork::restorePattern(std::vector<int>& input) {
  if (input.size() != dim_) {
    throw std::runtime_error("retrieve: input size mismatch");
  }
  std::vector<int> state = input;
  std::cout << '#' << std::flush;

  for (std::size_t l = 0; l < dim_; ++l) {
    std::vector<int> plus = state;
    std::vector<int> minus = state;
    plus[l] = 1;
    minus[l] = -1;

    double E_plus = energyPerState(plus, 0);
    double E_minus = energyPerState(minus, 0);

    state[l] = (E_plus < E_minus) ? 1 : -1;
  }
  if (state == input) {
    return true;
  } else {
    input = state;
    return false;
  }
}

bool ModernHopfieldNetwork::probability(double dE, double temp) const {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> distrib(0.0, 1.0);

  long double prob = std::exp((-std::abs(dE) / (temp)));
  double r = distrib(
      gen);  // non static_cast a unsigned int, ti serve un double tra 0 e 1

  return r < prob;
}

double ModernHopfieldNetwork::energyPerState(const std::vector<int>& state,
                                             int n) const {
  double e = 0;
  for (const auto& v : patternMatrix_.getMatrix()) {
    if (state.size() != v.size()) {
      throw std::runtime_error("Energy: state size mismatch");
    }
    e -= std::exp(dot(v, state) / CoolingSchedule(n));
  }
  return e;
}

double ModernHopfieldNetwork::CoolingSchedule(int iter) const {
  double temp;
  if (iter == 0) {
    temp = temp_0_;
    return temp;
  }

  // annealing functions
  // temp = static_cast<float>(temp_0_ * std::pow(0.8, iter));
  temp = static_cast<float>(temp_0_ * (1 / std::log(2 + iter)));
  // temp = static_cast<float>(temp_0 - 0.9*iter);

  if (temp < 0.01) {
    temp = 0.0;
  }
  return temp;
}

bool ModernHopfieldNetwork::restorePattern_withAnnealing(
    std::vector<int>& pattern, int n) const {
  if (pattern.size() != dim_) {
    throw std::runtime_error("retrieve: input size mismatch");
  }
  std::vector<int> state = pattern;
  std::cout << '#' << std::flush;

  for (std::size_t l = 0; l < dim_; ++l) {
    std::vector<int> plus = state;
    std::vector<int> minus = state;
    plus[l] = 1;
    minus[l] = -1;

    double E_plus = energyPerState(plus, n);
    double E_minus = energyPerState(minus, n);

    int candidate = (E_plus < E_minus) ? 1 : -1;

    if (candidate != state[l]) {
      //state[l] = candidate;
      double E0 = candidate > 0 ? E_plus : E_minus;
      double dE = energyPerState(state, n) - E0;

      if (probability(dE, CoolingSchedule(n))) {
        state[l] = candidate;
      }
    }
  }
  if (state == pattern) {
    return true;
  } else {
    pattern = state;
    return false;
  }
}

}  // namespace abc