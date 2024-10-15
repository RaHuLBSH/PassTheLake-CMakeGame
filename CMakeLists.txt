cmake_minimum_required(VERSION 3.10)
project(MyCMakeProject)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Specify the path to SFML
set(SFML_DIR "~/.local/lib/SFML-2.6.1/lib/cmake/SFML")

# Find the SFML package
find_package(SFML 2.6.1 COMPONENTS graphics window system REQUIRED)

# Find the X11 libraries
find_package(X11 REQUIRED)

# Add the SFML library path
link_directories("~/.local/lib/SFML-2.6.1/lib")

# Add your executable
add_executable(MyCMakeProject main.cpp)

# Link X11 and SFML libraries to your target
target_link_libraries(MyCMakeProject 
    ${X11_LIBRARIES} 
    Xrandr Xi Xinerama Xcursor Xxf86vm Xrender
    sfml-window 
    sfml-graphics 
    sfml-system 
    sfml-audio
)
