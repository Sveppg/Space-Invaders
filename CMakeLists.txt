cmake_minimum_required(VERSION 3.25)
project(project_name LANGUAGES CXX)

# Setze C++ Standard für das Projekt
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Setze Compiler-Optionen
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g -O3 -pthread -fsanitize=address")

# Definiere die Verzeichnisse, in denen Header-Dateien zu finden sind
include_directories(HEADER_FILES)

# Automatisches Erfassen aller Quell- und Header-Dateien
file(GLOB_RECURSE MAIN
    "src/*.cpp"
    "src/*.hpp"
)

# Erzeuge das ausführbare Programm
add_executable(main ${MAIN})

# Finde die raylib-Bibliothek, falls sie installiert ist
find_package(raylib QUIET) # Wenn raylib über find_package verfügbar ist, nutze es

if(NOT raylib_FOUND) # Falls raylib nicht gefunden wurde, benutze eine manuell definierte Methode
    include_directories("/usr/include/raylib") # Pfad zu den raylib-Header-Dateien
    link_directories("/usr/lib/raylib") # Pfad zum Ordner der raylib-Bibliothek
endif()

# Verlinke gegen die benötigten Libraries
target_link_libraries(main
    raylib
    GL
    m
    pthread
    dl
    rt
    X11
)

