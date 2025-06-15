# Hopfield Neural Network

Project developed by:
- Alessio Anceschi  
- Matteo Brigo  
- Marco Vincenzo Capalbo  

## 📚 Description

This project implements two versions of a **Hopfield Neural Network**:

- **Classic**: based on the Hebbian learning rule to build the weight matrix.
- **Modern**: uses an energy function and a *simulated annealing* algorithm to reach a stable configuration.

Both versions aim to **store, corrupt, recognize, and then correct** image patterns.

## 🗂️ Project Structure

The project is organized into four subfolders within the main `HopfieldNetwork` directory:

- `HopfieldImagePattern`: handles image reading, adaptation, and corruption  
- `Matrix`: contains functions for manage the matrix  
- `ClassicHopfieldNetwork`: contains the classic network implementation  
- `ModernHopfieldNetwork`: contains the modern network implementation


## 🛠️ Compilation

Before compiling, make sure you have SFML installed (used for graphical output)


## 🎬 Demo

The project includes **example demo applications** to showcase both the learning and recognition phases of the Hopfield Neural Network, in both its classic and modern versions.

### 📁 Demo files

In the main project directory (`HopfieldNetwork`), you will find the following key source files:

- `ClassicLearn.cpp`: Example program to train the classic Hopfield network.
- `ClassicRecog.cpp`: Program that recovers a corrupted pattern using the classic method.
- `ModernLearn.cpp`: Training program for the modern Hopfield network.
- `ModernRecog.cpp`: Program that applies simulated annealing to recover patterns using the modern method.

### ⚙️ Requirements

Make sure you have **CMake** installed. If not, please install it.

Be sure to be in HopfieldNetwork.

Then execute:
`cmake -S . -B build -G "Ninja Multi-Config"  
cmake --build build --config Debug  
cmake --build build --config Debug --target test  
cmake --build build --config Release  
cmake --build build --config Release --target test` 

And now:
-`./build/Debug(Relaese)/ClassicLearn`: to run ClassicLearn demo. 
-`./build/Debug(Relaese)/ClassicRecog`: to run ClassicRecog demo.
-`./build/Debug(Relaese)/ModernLearn`: to run ModernLearn demo.
-`./build/Debug(Relaese)/ModernRecog`: to run ModernRecog demo.




