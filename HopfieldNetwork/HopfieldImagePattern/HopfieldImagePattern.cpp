
#include "HopfieldImagePattern.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <random>
 
// Costructors
namespace abc {
HopfieldImagePattern::HopfieldImagePattern(const std::string& filepath) {
  setInImage(filepath);
}
HopfieldImagePattern::HopfieldImagePattern(const std::string& filepath,
                                           unsigned int sqrtDim) {
                                            
  setInImage(filepath);
  setImageDimension(sqrtDim);
}

// setter
void HopfieldImagePattern::setInImage(const sf::Image& image) {
  inImage_ = image;
  pattern_.clear();
}
void HopfieldImagePattern::setInImage(const std::string& filepath) {
  if (!inImage_.loadFromFile(filepath)) {
    throw std::runtime_error(
        "Input image not loaded, please check input path.");
  }
  pattern_.clear();
}
void HopfieldImagePattern::setImageDimension(unsigned int val) {
  imageDimension_ = val;
}

// getter
const sf::Image& HopfieldImagePattern::get_inImage() const { return inImage_; }
unsigned int HopfieldImagePattern::getImageDimension() const {
  return imageDimension_;
}
long unsigned int HopfieldImagePattern::getPatternDimension() const {
  return pattern_.size();
}

const std::vector<int>& HopfieldImagePattern::getPattern() const {
  return pattern_;
}

std::vector<int> HopfieldImagePattern::getPattern_for_testing() const {
  return pattern_;
}

// other methods
void HopfieldImagePattern::checkPatternDimension() const {
  if (pattern_.empty()) {
    throw std::runtime_error(
        "Cannot access or modify the pattern because it has not been "
        "initialized. Please create it first.");
  }
  assert(pattern_.size() == imageDimension_ * imageDimension_);
}
sf::Image HopfieldImagePattern::printPattern() const {
  checkPatternDimension();

  sf::Image patternVisualization;
  patternVisualization.create(imageDimension_, imageDimension_,
                              sf::Color::Transparent);

  for (unsigned int y = 0; y < imageDimension_; ++y) {
    for (unsigned int x = 0; x < imageDimension_; ++x) {
      if (pattern_[y * imageDimension_ + x] == 1) {
        patternVisualization.setPixel(x, y, sf::Color::Black);
      } else {
        patternVisualization.setPixel(x, y, sf::Color::White);
      }
    }
  }

  return patternVisualization;
}
double HopfieldImagePattern::getAdaptedPixel(const sf::Color& pixel) const {
  return (pixel.r + pixel.g + pixel.b) / 3.0;
}

// methods to elaborate the pattern
void HopfieldImagePattern::cutPattern(unsigned int pixel_x,
                                      unsigned int pixel_y) {
  size_t patternInSize{pattern_.size()};
  unsigned int fromWhere{pixel_y * imageDimension_ + pixel_x};
  checkPatternDimension();
  if (static_cast<size_t>(fromWhere) >= patternInSize) {
    throw std::runtime_error(
        "invalid starting point, it exceeds the pattern dimension!");
  }

  for (unsigned int i = fromWhere; i < pattern_.size(); ++i) {
    pattern_[i] = 1;
  }
  checkPatternDimension();
}
void HopfieldImagePattern::corrupt(long unsigned int nPixel) {
  checkPatternDimension();
  if (nPixel > imageDimension_ * imageDimension_) {
    throw std::runtime_error(
        "Cannot corrupt more pixels than the pattern contains.");
  }

  std::vector<unsigned int> indices(pattern_.size());
  std::iota(indices.begin(), indices.end(), 0);

  std::random_device rd;   // a seed source for the random number engine
  std::mt19937 gen(rd());  // mersenne_twister_engine seeded with rd()
  std::shuffle(indices.begin(), indices.end(), gen);

  for (long unsigned int i = 0; i < nPixel; ++i) {
    pattern_[indices[i]] *= -1;
  }

  checkPatternDimension();
}

// methods to create pattern(private)
void HopfieldImagePattern::createBinaryPattern(const sf::Image& img) {
  for (unsigned int y = 0; y < img.getSize().y; ++y) {
    for (unsigned int x = 0; x < img.getSize().x; ++x) {
      pattern_.push_back(getAdaptedPixel(img.getPixel(x, y)) < 127 ? 1 : -1);
    }
  }
}

// methods to shrink images (questi non hanno throws perchè sono già ceckatti
// prima in setInImage)
sf::Image HopfieldImagePattern::adaptImage_withSFML() {
  sf::Texture o_Texture;
  o_Texture.loadFromImage(
    inImage_); // per essere caricata l'immagine passa da setInImage, dove
                   // sono già presenti dei throw
  o_Texture.setSmooth(true);

  sf::Sprite sprite;
  sprite.setTexture(o_Texture);
  sprite.setScale(static_cast<float>(imageDimension_) /
                      static_cast<float>(inImage_.getSize().x),
                  static_cast<float>(imageDimension_) /
                      static_cast<float>(inImage_.getSize().y));

  sf::RenderTexture final_render;
  final_render.create(imageDimension_, imageDimension_);
  final_render.clear(sf::Color::Transparent);
  final_render.draw(sprite);
  final_render.display();

  createBinaryPattern(final_render.getTexture().copyToImage());

  // assert to avoid error on size of the pattern
  assert(pattern_.size() == imageDimension_ * imageDimension_);

  return final_render.getTexture().copyToImage();
}
void HopfieldImagePattern::adaptImage_withBilinearInterpolation() {
  unsigned int img_width = inImage_.getSize().x;
  unsigned int img_height = inImage_.getSize().y;

  double x_ratio = (img_width - 1) / static_cast<double>(imageDimension_ - 1);
  double y_ratio = (img_height - 1) / static_cast<double>(imageDimension_ - 1);

  for (unsigned int y = 0; y < imageDimension_; ++y) {
    for (unsigned int x = 0; x < imageDimension_; ++x) {
      double gx = x_ratio * x;
      double gy = y_ratio * y;

      unsigned int x_l = static_cast<unsigned int>(std::floor(gx));
      unsigned int x_h = std::min(static_cast<unsigned int>(std::ceil(gx)),
                                  static_cast<unsigned int>(img_width - 1));
      unsigned int y_l = static_cast<unsigned int>(std::floor(gy));
      unsigned int y_h = std::min(static_cast<unsigned int>(std::ceil(gy)),
                                  static_cast<unsigned int>(img_height - 1));

      double x_weight{gx - x_l};
      double y_weight{gy - y_l};

      sf::Color a = inImage_.getPixel(x_l, y_l);
      sf::Color b = inImage_.getPixel(x_h, y_l);
      sf::Color c = inImage_.getPixel(x_l, y_h);
      sf::Color d = inImage_.getPixel(x_h, y_h);

      double pixel = getAdaptedPixel(a) * (1 - x_weight) * (1 - y_weight) +
                     getAdaptedPixel(b) * (x_weight) * (1 - y_weight) +
                     getAdaptedPixel(c) * (1 - x_weight) * (y_weight) +
                     getAdaptedPixel(d) * (x_weight) * (y_weight);

      pattern_.push_back(pixel < 127 ? 1 : -1);
    }
  }

  // assert to avoid error making pattern
  assert(pattern_.size() == imageDimension_ * imageDimension_);
}
}  // namespace abc