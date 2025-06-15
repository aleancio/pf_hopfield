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

- HopfieldImagePattern: handles image reading, adaptation, and corruption  
- Matrix: contains functions for manage the matrix  
- ClassicHopfieldNetwork: contains the classic network implementation  
- ModernHopfieldNetwork: contains the modern network implementation


## 🛠️ Compilation

Before compiling, make sure you have SFML installed (used for graphical output)


## 🎬 Demo

We included **example demo applications** to showcase the learning and recognition process of the Hopfield Neural Network.

### 📁 Demo folders

In the main project directory (`HopfieldNetwork`), you will find:

- `demo_inputs/`: contains pre-processed or raw input images for both classic and modern demos  
- `demo_outputs/`: created automatically by the programs, showing the recovery process step-by-step  
- `demo_scripts/` (if present): optional helper scripts for launching full demo sequences  

### 🧪 Run Classic Demo

To run the classic Hopfield network demo:




