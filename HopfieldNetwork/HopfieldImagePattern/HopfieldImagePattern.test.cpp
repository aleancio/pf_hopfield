#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "HopfieldImagePattern.hpp"

#include "../doctest.h"

TEST_CASE("Testing Constructors") {
  SUBCASE("Failure on constructors") {  // test for setInImage() already passed
                                        // with them

    CHECK_THROWS_WITH_AS(abc::HopfieldImagePattern("nullFile.png"),
                         "Input image not loaded, please check input path.",
                         std::runtime_error);

    CHECK_THROWS_WITH_AS(abc::HopfieldImagePattern("nullFile.png", 20),
                         "Input image not loaded, please check input path.",
                         std::runtime_error);
  };
  SUBCASE("Constructors working - setting only image") {
    abc::HopfieldImagePattern pattern("../HopfieldImagePattern/images/orecchino.png");
    CHECK(pattern.get_inImage().getSize().x >= 0);
  };
  SUBCASE("Constructors working - setting image and dimension") {
    abc::HopfieldImagePattern pattern{"../HopfieldImagePattern/images/orecchino.png", 20};
    REQUIRE(pattern.get_inImage().getSize().x >= 0);
    CHECK(pattern.getImageDimension() == 20);
  };
}
TEST_CASE("Testing HopfieldImagePattern functions") {
  abc::HopfieldImagePattern pattern(
      "../HopfieldImagePattern/images/orecchino.png",
      2);  // setting dimension to 2 so no alghorithm will change that ratio = 1

  sf::Image testImage;
  testImage.create(2, 2, sf::Color::Black);

  // Settiamo manualmente i pixel
  testImage.setPixel(0, 0, sf::Color(255, 255, 255));  // Bianco
  testImage.setPixel(1, 0, sf::Color(0, 0, 0));        // Nero
  testImage.setPixel(0, 1, sf::Color(130, 130, 130));  // Grigio chiaro
  testImage.setPixel(1, 1, sf::Color(100, 100, 100));  // Grigio scuro

  pattern.setInImage(testImage);

  std::vector<int> expectedPattern = {-1, 1, -1, 1};

  SUBCASE("Testing adapting algorithm - with SFML") {
    pattern.adaptImage_withSFML();
    CHECK(pattern.getPatternDimension() == 4);
    std::vector<int> createdPattern = pattern.getPattern();
    CHECK(createdPattern ==
          expectedPattern);  // using overload of operator== for std::vector
  }
  SUBCASE("Testing adapting algorithm - with bilinear interpolation") {
    pattern.adaptImage_withBilinearInterpolation();
    CHECK(pattern.getPatternDimension() == 4);
    std::vector<int> createdPattern = pattern.getPattern();
    CHECK(createdPattern ==
          expectedPattern);  // using overload of operator== for std::vector
  }
  SUBCASE(
      "Testing printPattern - whether the function actually print the "
      "pattern") {
    pattern.adaptImage_withBilinearInterpolation();
    std::vector<int> createdPattern = pattern.getPattern();

    sf::Image result = pattern.printPattern();

    CHECK(result.getSize().x == 2);
    CHECK(result.getSize().y == 2);
    CHECK(result.getPixel(0, 0) == sf::Color::White);
    CHECK(result.getPixel(1, 0) == sf::Color::Black);
    CHECK(result.getPixel(0, 1) == sf::Color::White);
    CHECK(result.getPixel(1, 1) == sf::Color::Black);
  }
  SUBCASE("Testing printPattern - error pattern empty while printing it") {
    CHECK_THROWS_WITH_AS(pattern.printPattern(),
                         "Cannot access or modify the pattern because it has "
                         "not been initialized. Please create it first.",
                         std::runtime_error);
  }

  // cutting pattern
  SUBCASE(
      "Testing cutPattern - whether the function actually cut the pattern") {
    pattern.adaptImage_withBilinearInterpolation();
    std::vector<int> createdPattern = pattern.getPattern();
    REQUIRE(pattern.getPatternDimension() == 4);
    REQUIRE(createdPattern == expectedPattern);

    pattern.cutPattern(1, 0);
    sf::Image result = pattern.printPattern();

    CHECK(result.getSize().x == 2);
    CHECK(result.getSize().y == 2);
    CHECK(result.getPixel(0, 0) == sf::Color::White);
    CHECK(result.getPixel(1, 0) == sf::Color::Black);
    CHECK(result.getPixel(0, 1) == sf::Color::Black);
    CHECK(result.getPixel(1, 1) == sf::Color::Black);
  }
  SUBCASE("Testing cutPattern - overcome pattern size") {
    pattern.adaptImage_withBilinearInterpolation();
    REQUIRE(pattern.getPatternDimension() == 4);

    CHECK_THROWS_WITH_AS(
        pattern.cutPattern(2, 1),
        "invalid starting point, it exceeds the pattern dimension!",
        std::runtime_error);
  }
  SUBCASE("Testing cutPattern - pattern empty") {
    CHECK_THROWS_WITH_AS(pattern.cutPattern(1, 1),
                         "Cannot access or modify the pattern because it has "
                         "not been initialized. Please create it first.",
                         std::runtime_error);
  }

  // corrupting pattern
  SUBCASE("Testing corrupt - corrupting 0 pixel does nothing") {
    pattern.adaptImage_withSFML();
    auto original = pattern.getPattern();
    pattern.corrupt(0);
    CHECK(pattern.getPattern() == original);
  }
  SUBCASE("Testing corrupt - corrupt n pixels modifies exactly n values") {
    pattern.adaptImage_withSFML();
    auto original = pattern.getPattern_for_testing();
    pattern.corrupt(3);
    auto corrupted = pattern.getPattern_for_testing();

    REQUIRE(corrupted != original);

    int diffCount = 0;
    for (unsigned int i = 0; i < pattern.getPatternDimension(); ++i) {
      if (original[i] != corrupted[i]) {
        ++diffCount;
      }
    }

    CHECK(diffCount == 3);
  }
  SUBCASE("Testing corrupt - corrupt more pixels than allowed throws") {
    pattern.adaptImage_withSFML();
    CHECK_THROWS_WITH_AS(
        pattern.corrupt(pattern.getPatternDimension() + 1),
        "Cannot corrupt more pixels than the pattern contains.",
        std::runtime_error);
  }

  // pattern elaborator
  SUBCASE("Testing patterElaborator - passing the right pattern ") {
    pattern.adaptImage_withSFML();
    const std::vector<int>& ref = pattern.elaboratePattern();
    CHECK(ref == expectedPattern);
  }
  SUBCASE("Testing patterElaborator - modifying the pattern ") {
    pattern.adaptImage_withSFML();
    std::vector<int>& ref = pattern.elaboratePattern();
    ref[2] = -1;
    CHECK(pattern.getPattern()[2] == -1);
  }
}

