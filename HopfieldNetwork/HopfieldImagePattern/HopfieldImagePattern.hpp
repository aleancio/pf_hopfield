#ifndef HOPFIELDNEURALNETWORK_HOPFIELDIMAGEPATTERN_H
#define HOPFIELDNEURALNETWORK_HOPFIELDIMAGEPATTERN_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

//il class invariant è la dimensione di pattern una volta creato

namespace abc{
class HopfieldImagePattern {
 private:
  sf::Image inImage_;
  unsigned int imageDimension_{100};
  std::vector<int> pattern_;


  //private methods
  double getAdaptedPixel(const sf::Color &pixel) const; //tested the consequences
  void createBinaryPattern(const sf::Image &img); //tested the consequences
  void checkPatternDimension() const ; //check class invariant


 public:
  // constructor
  HopfieldImagePattern(const std::string &filepath);
  HopfieldImagePattern(const std::string &filepath, unsigned int sqrtPattrnDimension);

  // setter
  void setInImage(const std::string &filepath);
  void setInImage(const sf::Image &image);
  void setImageDimension(unsigned int val);

  
  //elaborator
  std::vector<int>& elaboratePattern(){ return pattern_; }


  //getter
  const sf::Image& get_inImage() const;
  unsigned int getImageDimension() const;
  long unsigned int getPatternDimension() const;
  const std::vector<int>& getPattern() const;
  std::vector<int> getPattern_for_testing() const;

  sf::Image printPattern() const;

  //other methods
  void cutPattern(unsigned int pixel_x, unsigned int pixel_y); //cut pattern fromWhere - 1 till end (set pixel to black)
  void corrupt(long unsigned int nPixel);

  //these methods adapt inImage to the size given and they use createBinaryPattern() to load pattern_
  sf::Image adaptImage_withSFML(); 
  void adaptImage_withBilinearInterpolation();


};
} //end of namespace

#endif