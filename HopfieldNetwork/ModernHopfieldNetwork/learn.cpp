#include <iostream>

#include "../HopfieldImagePattern/HopfieldImagePattern.hpp"
#include "ModernHopfieldNetwork.hpp"

//./build/Debug/ModernLearn

int main() {
  int dimension{0};
  std::cout << "---- Welcome to the Modern Hopfield Network ----\n";
  std::cout << "---- This is the learning section. Please follow the instructions below ----\n\n";

  try {
    std::cout << "You are now going to configure your neural network.\n\n";

    std::cout << "First, choose the size of the neural network. "
                 "Larger sizes result in longer learning times but improved accuracy.\n";
    std::cout << "Enter the network size [10, 100]:\t";
    std::cin >> dimension;
    if (dimension < 10 || dimension > 100) {
      throw std::runtime_error("Invalid size! Please choose a value within the specified range.");
    }
    std::cout << "\n\n";

    abc::ModernHopfieldNetwork net(dimension*dimension);
    std::string folderPath;
    std::cout << "For demonstration purposes, some images are already provided in a folder named 'images'.\n";
    std::cout << "Please enter the absolute path to your image folder (type '#' to use the default folder):\t";
    std::cin >> folderPath;
    if(folderPath == "#")
    {
        folderPath = "ModernHopfieldNetwork/images";
    }

    std::cout << "Selected folder: " << folderPath << "\n";
    std::cout << folderPath << "\n"; 
    std::vector<std::string> images{abc::setDirectory(folderPath)};
    
    std::cout << "LOADING STARTED\n" << std::flush;

    for (const auto& path : images) {
      std::cout << "working on:\t " << path << '\n';
      abc::HopfieldImagePattern pattern(path, static_cast<unsigned int>(dimension));
      pattern.adaptImage_withBilinearInterpolation();

      net.learnPattern(pattern.getPattern());
    }


    net.save("ModernMatrixValues.txt");
    std::cout << "LEARNING COMPLETE\n" << std::flush;

  } catch (std::exception const& e) {
    std::cerr << "Caught exception: '" << e.what() << "'\n";
    return EXIT_FAILURE;
  } catch (...) {
    std::cerr << "Caught unknown exception\n";
    return EXIT_FAILURE;
  }
}