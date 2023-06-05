run: build
	./build/GLFW-CMake-starter

build: CMakeLists.txt glfw main.cpp
	cmake -S . -B build
	cmake --build build
