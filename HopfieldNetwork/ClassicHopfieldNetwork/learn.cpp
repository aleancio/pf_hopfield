#include <iostream>

#include "../HopfieldImagePattern/HopfieldImagePattern.hpp"
#include "ClassicHopfieldNetwork.hpp"

// ./build/Debug/ClassicLearn

int main() {
  unsigned int dimension{0};

  std::cout << "---- Welcome to the Classic Hopfield Network ----\n";
  std::cout << "---- This is the learning section. Please follow the "
               "instructions below ----\n\n";

  try {
    std::cout << "Let's configure your neural network.\n\n";

    std::cout << "First, set the dimension of your neural network. "
                 "Larger sizes will require more time to learn but can store "
                 "more information.\n";
    std::cout << "Enter the size of your neural network [10, 100]:\t";
    std::cin >> dimension;
    if (dimension < 10 || dimension > 100) {
      throw std::runtime_error(
          "Invalid size! Please choose a value within the range.");
    }
    std::cout << "\n\n";

    abc::ClassicHopfieldNetwork net(dimension * dimension);
    std::string folderPath;
    std::cout << "For demonstration purposes, a set of images has already been "
                 "provided\n"
                 "in a folder named 'images'.\n";
    std::cout << "Please enter the absolute path to the folder containing your "
                 "images (type '#' to use the default):\t";
    std::cin >> folderPath;
    if (folderPath == "#") {
      folderPath = "ClassicHopfieldNetwork/images";
    }

    std::cout << "Selected folder: " << folderPath << "\n";
    std::vector<std::string> images{abc::setDirectory(folderPath)};

    std::cout << "STARTING LEARNING PROCESS\n" << std::flush;

    for (const auto& path : images) {
      std::cout << "Processing image:\t" << path << '\n';
      abc::HopfieldImagePattern pattern(path, dimension);
      pattern.adaptImage_withBilinearInterpolation();

      net.learnPattern(pattern.getPattern());
    }

    net.save("ClassicMatrixValues.txt");
    std::cout << "LEARNING COMPLETED\n" << std::flush;

  } catch (std::exception const& e) {
    std::cerr << "Caught exception: '" << e.what() << "'\n";
    return EXIT_FAILURE;
  } catch (...) {
    std::cerr << "Caught unknown exception\n";
    return EXIT_FAILURE;
  }
}
