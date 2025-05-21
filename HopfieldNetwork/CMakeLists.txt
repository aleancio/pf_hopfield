cmake_minimum_required(VERSION 3.28)
project(progettopf_final VERSION 0.1.0)

# abilita il supporto per i test, tra cui l'opzione BUILD_TESTING usata sotto
include(CTest)

# esporta il compilation database, utile per altri strumenti, a partire dall'editor
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# richiedi l'uso di C++20 (volendo anche C++23), senza estensioni non-standard offerte dal compilatore usato
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# abilita warning
string(APPEND CMAKE_CXX_FLAGS
      " -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion"
      " -Wshadow -Wimplicit-fallthrough -Wextra-semi -Wold-style-cast"
      " -fno-omit-frame-pointer")

# abilita asserzioni della standard library
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  string(APPEND CMAKE_CXX_FLAGS " -D_GLIBCXX_ASSERTIONS")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  string(APPEND CMAKE_CXX_FLAGS " -D_LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_EXTENSIVE")
endif()

# abilita address sanitizer e undefined-behaviour sanitizer in Debug mode
string(APPEND CMAKE_CXX_FLAGS_DEBUG " -fsanitize=address,undefined")
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  string(APPEND CMAKE_CXX_FLAGS " -D_GLIBCXX_SANITIZE_STD_ALLOCATOR")
endif()
string(APPEND CMAKE_EXE_LINKER_FLAGS_DEBUG " -fsanitize=address,undefined")

# se usato, richiedi il componente graphics della libreria SFML (versione 2.6 in Ubuntu 24.04)
find_package(SFML 2.6 COMPONENTS graphics REQUIRED)

# dichiara un eseguibile chiamato "progetto", prodotto a partire dai file sorgente indicati
# sostituire "progetto" con il nome del proprio eseguibile e i file sorgente con i propri (con nomi sensati!)


#Classic
add_executable(ClassicLearn ClassicHopfieldNetwork/learn.cpp ClassicHopfieldNetwork/ClassicHopfieldNetwork.cpp HopfieldImagePattern/HopfieldImagePattern.cpp)
target_link_libraries(ClassicLearn PRIVATE sfml-graphics sfml-window sfml-system)


add_executable(ClassicRecog ClassicHopfieldNetwork/restorePattern.cpp ClassicHopfieldNetwork/ClassicHopfieldNetwork.cpp HopfieldImagePattern/HopfieldImagePattern.cpp)
target_link_libraries(ClassicRecog PRIVATE sfml-graphics sfml-window sfml-system)



#Modern
add_executable(ModernLearn ModernHopfieldNetwork/learn.cpp ModernHopfieldNetwork/ModernHopfieldNetwork.cpp HopfieldImagePattern/HopfieldImagePattern.cpp)
target_link_libraries(ModernLearn PRIVATE sfml-graphics sfml-window sfml-system)

add_executable(ModernRecog ModernHopfieldNetwork/restorePattern.cpp ModernHopfieldNetwork/ModernHopfieldNetwork.cpp HopfieldImagePattern/HopfieldImagePattern.cpp)
target_link_libraries(ModernRecog PRIVATE sfml-graphics sfml-window sfml-system)


# il testing e' abilitato di default
# per disabilitarlo, passare -DBUILD_TESTING=OFF a cmake durante la fase di configurazione
if (BUILD_TESTING)

  # aggiungi l'eseguibile progetto.t
  add_executable(Pattern.t HopfieldImagePattern/HopfieldImagePattern.cpp HopfieldImagePattern/HopfieldImagePattern.test.cpp)
  target_link_libraries(Pattern.t PRIVATE sfml-graphics sfml-window sfml-system)


  add_executable(Matrix.t Matrix/Matrix.test.cpp)
  add_executable(Classic.t ClassicHopfieldNetwork/ClassicHopfieldNetwork.cpp ClassicHopfieldNetwork/ClassicHopfieldNetwork.test.cpp)
  add_executable(Modern.t ModernHopfieldNetwork/ModernHopfieldNetwork.cpp ModernHopfieldNetwork/ModernHopfieldNetwork.test.cpp)





  # aggiungi l'eseguibile progetto.t alla lista dei test
  add_test(NAME Pattern.t COMMAND Pattern.t)
  add_test(NAME Matrix.t COMMAND Matrix.t)
  add_test(NAME Classic.t COMMAND Classic.t)
  add_test(NAME Modern.t COMMAND Modern.t)


endif()