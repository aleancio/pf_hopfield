
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "ModernHopfieldNetwork.hpp"

#include "../doctest.h"
TEST_CASE("Testing constructor") {
  CHECK_THROWS_WITH_AS(abc::ModernHopfieldNetwork{0},
                       "Invalid pattern dimension, it must be greater than 0",
                       std::runtime_error);
}
TEST_CASE(
    "Testing restorePattern - it converges to stored pattern in one "
    "iteration") {
  abc::ModernHopfieldNetwork net(4);
  std::vector<int> testPattern1 = {-1, 1, 1, -1};
  std::vector<int> testPattern2 = {1, -1, -1, 1};
  std::vector<int> corrupted_pat = {1, -1, 1, -1};

  net.learnPattern(testPattern1);
  net.learnPattern(testPattern2);

  net.restorePattern(corrupted_pat);
  CHECK(corrupted_pat == testPattern1);

  CHECK(net.restorePattern(corrupted_pat));
}
TEST_CASE("Testing setting T0") {
  abc::ModernHopfieldNetwork net;
  double energy{10.0};
  net.setTemp0(energy);
  CHECK(net.CoolingSchedule(0) == 45);
}
TEST_CASE("Testing energyPerState") {
  abc::ModernHopfieldNetwork net;

  std::vector<int> pattern1{1, -1, 1};
  std::vector<int> pattern2{-1, 1, -1};
  std::vector<int> state = {1, 1, 1};

  SUBCASE("Return real values") {
    net.learnPattern(pattern1);
    net.learnPattern(pattern2);
    // Dot products:
    // dot({1,-1,1}, {1,1,1}) = 1 - 1 + 1 = 1
    // dot({-1,1,-1}, {1,1,1}) = -1 + 1 -1 = -1

    // Supponiamo CoolingSchedule(0) = 1.0 per semplicità
    double expectedEnergy = -std::exp(1.0 / net.CoolingSchedule(0)) -
                            std::exp(-1.0 / net.CoolingSchedule(0));
    double computedEnergy = net.energyPerState(state, 0);
    CHECK(doctest::Approx(computedEnergy).epsilon(0.0001) == expectedEnergy);
  }
  SUBCASE("Handles empty matrix") {
    double computedEnergy = net.energyPerState(state, 0);

    CHECK(computedEnergy == 0.0);
  }
  SUBCASE("ModernHopfieldNetwork::energyPerState throws on size mismatch") {
    net.learnPattern(pattern1);

    std::vector<int> bad_state = {1, 1};  // size mismatch

    CHECK_THROWS_WITH_AS(net.energyPerState(bad_state, 0),
                         "Energy: state size mismatch", std::runtime_error);
  }
}
TEST_CASE("Testing totalEnergy - computes correct energy") {
  abc::ModernHopfieldNetwork net(3);  // 3 dimensioni

  std::vector<int> p1 = {1, -1, 1};
  std::vector<int> p2 = {-1, 1, -1};
  std::vector<int> p3 = {1, 1, 1};

  net.learnPattern(p1);
  net.learnPattern(p2);
  net.learnPattern(p3);

  const double invSqrtDim = 1.0 / std::sqrt(3.0);
  // Calcolo manuale delle interazioni:
  // p1 ⋅ p2 = -3  => exp(-3 / sqrt(9)) = exp(-1/3)
  // p1 ⋅ p3 = 1   => exp(1 / sqrt(9))
  // p2 ⋅ p3 = -1  => exp(-1 / sqrt(9))
  double expectedEnergy =
      -(std::exp(-3.0 * invSqrtDim) + std::exp(1.0 * invSqrtDim) +
        std::exp(-1.0 * invSqrtDim));

  CHECK(doctest::Approx(net.totalSystemEnergy()).epsilon(1e-6) ==
        expectedEnergy);
}
TEST_CASE("restorePattern_withAnnealing converges to a stable pattern") {
  abc::ModernHopfieldNetwork net(5);
  std::vector<int> pattern = {1, -1, 1, -1, 1};
  net.learnPattern(pattern);
  std::vector<int> corrupted = {1, -1, -1, -1, 1};

  bool done = false;
  int iter = 0;
  while (!done && iter < 1000) {
    done = net.restorePattern_withAnnealing(corrupted, iter);
    ++iter;
  }

  CHECK(corrupted == pattern);  // convergenza al pattern originale
}