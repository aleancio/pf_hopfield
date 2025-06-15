#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "ClassicHopfieldNetwork.hpp"

#include "../doctest.h"

TEST_CASE("Costructor") {
  std::size_t dim = 10;
  abc::ClassicHopfieldNetwork net(dim);

  CHECK(net.getMatrix().getMatrix()[0].size() == dim);
  CHECK(net.getMatrix().size() == dim);

  for (std::size_t i = 0; i < dim; ++i) {
    for (std::size_t j = 0; j < dim; ++j) {
      CHECK(net.getMatrix().getElement(i, j) == doctest::Approx(0.0));
    }
  }
  SUBCASE("Testing Throw") {
    CHECK_THROWS_WITH_AS(abc::ClassicHopfieldNetwork(0),
                         "Invalid pattern dimension, it must be greater than 0",
                         std::runtime_error);
  }
}
TEST_CASE("Testing learning pattern") {
  abc::ClassicHopfieldNetwork net(4);

  SUBCASE("Testing learning pattern - single pattern") {
    std::vector<int> testPattern = {1, -1, 1, 1};

    net.learnPattern(testPattern);

    auto& mat = net.getMatrix();

    CHECK(mat.size() == testPattern.size());

    for (unsigned int i = 0; i < mat.getMatrix().size(); ++i) {
      CHECK(mat.size() == testPattern.size());
      for (unsigned int j = 0; j < mat.size(); ++j) {
        if (i == j) {
          CHECK(mat.getElement(i, j) == doctest::Approx(0.0));
        } else {
          double expected =
              static_cast<double>(testPattern[i] * testPattern[j]) / 4.0;
          CHECK(mat.getElement(i, j) == doctest::Approx(expected));
          CHECK(mat.getElement(i, j) ==
                doctest::Approx(mat.getElement(j, i)));  // simmetry
        }
      }
    }
  }
  SUBCASE("Testing learning pattern - multiple pattern") {
    std::vector<int> testPattern1 = {-1, 1, 1, -1};
    std::vector<int> testPattern2 = {1, -1, -1, 1};

    net.learnPattern(testPattern1);
    net.learnPattern(testPattern2);

    auto& mat = net.getMatrix();

    CHECK(mat.getElement(0, 0) == 0);
    CHECK(mat.getElement(1, 0) == -0.5);
    CHECK(mat.getElement(2, 0) == -0.5);
    CHECK(mat.getElement(3, 0) == 0.5);
    CHECK(mat.getElement(1, 1) == 0);
    CHECK(mat.getElement(2, 1) == 0.5);
    CHECK(mat.getElement(3, 1) == -0.5);
    CHECK(mat.getElement(2, 3) == -0.5);
  }
  SUBCASE("Testing learning pattern - Learning non-suitable pattern") {
    std::vector<int> testPattern1 = {-1, 1, 1, -1};
    std::vector<int> testPattern2 = {1, -1, -1};

    net.learnPattern(testPattern1);

    CHECK_THROWS_WITH_AS(net.learnPattern(testPattern2),
                         "matrix and pattern sizes do not match!",
                         std::runtime_error);
  }
}

// MAIN TEST
TEST_CASE("Testing restore pattern") {
  abc::ClassicHopfieldNetwork net(4);
  std::vector<int> testPattern1 = {-1, 1, 1, -1};
  std::vector<int> testPattern2 = {1, -1, -1, 1};
  std::vector<int> corrupted_pat = {1, -1, 1, -1};

  net.learnPattern(testPattern1);
  net.learnPattern(testPattern2);

  SUBCASE(
      "Testing restore pattern - corrupted pattern one iteration and "
      "end_condition") {
    net.restorePattern(corrupted_pat);
    CHECK(corrupted_pat == testPattern1);
    CHECK(net.restorePattern(corrupted_pat));
  }
  SUBCASE("Testing restore pattern - corrupted pattern with wrong dimension") {
    std::vector<int> corrupted_pat_wd = {1, -1, 1};

    CHECK_THROWS_WITH_AS(net.restorePattern(corrupted_pat_wd),
                         "matrix and pattern sizes do not match!",
                         std::runtime_error);
  }
}
TEST_CASE("Testing Energy Functions") {
  abc::ClassicHopfieldNetwork net(3);
  auto& weights = net.getMatrix();
  weights.setElement(0, 0, 0.0);
  weights.setElement(0, 1, 1.0);
  weights.setElement(0, 2, -1.0);
  weights.setElement(1, 0, 1.0);
  weights.setElement(1, 1, 0.0);
  weights.setElement(1, 2, 1.0);
  weights.setElement(2, 0, -1.0);
  weights.setElement(2, 1, 1.0);
  weights.setElement(2, 2, 0.0);

  CHECK(net.getMatrix().getElement(0, 1) == 1);

  std::vector<int> pattern = {1, -1, 1};
  // calcolato a mano

  CHECK(doctest::Approx(net.totalEnergy(pattern)) == 3.0);
}
TEST_CASE("Testing Annealing") {
  SUBCASE("CoolingSchedule behaves correctly") {
    abc::ClassicHopfieldNetwork net;

    CHECK(net.CoolingSchedule(0) == doctest::Approx(50));
    CHECK(net.CoolingSchedule(1) > net.CoolingSchedule(10));
    CHECK(net.CoolingSchedule(100000) < 5);
  }
  SUBCASE("probability returns true more often for high temperature") {
    abc::ClassicHopfieldNetwork net;

    int accept_count_high = 0;
    int accept_count_low = 0;
    double dE = -5.0;

    // Alta temperatura: ~probabilità alta
    for (int i = 0; i < 2000; ++i)
      if (net.probability(dE, 1000)) ++accept_count_high;

    // Bassa temperatura: ~probabilità bassa
    for (int i = 0; i < 2000; ++i)
      if (net.probability(dE, 0.01)) ++accept_count_low;

    CHECK(accept_count_high > accept_count_low);
  }
  SUBCASE("restorePattern_withAnnealing converges to a stable pattern") {
    abc::ClassicHopfieldNetwork net(5);
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
}