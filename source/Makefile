.PHONY = all clean
CXX = g++
CXXFLAGS = -Wall -Wextra -O2 -static
LIB = RAYLIB

ifeq ($(LIB),RAYLIB)
	LIBDEFINE = RAYLIB
	LIBNAME = raylib
	LIBPATH = C:\Libraries\raylib\src
	INCLUDEPATH = C:\Libraries\raylib\src
else ifeq ($(LIB),SDL2)
	LIBDEFINE = SDL2
	LIBNAME = SDL2
	LIBPATH = C:\Libraries\SDL2\SDL2-2.0.10\x86_64-w64-mingw32\lib
	INCLUDEPATH = C:\Libraries\SDL2\SDL2-2.0.10\x86_64-w64-mingw32\include\SDL2
endif

all: Snake.exe SnakeClassic.exe

Snake.exe: main.o game.o libraryBindings.o
	$(CXX) $(CXXFLAGS) -o Snake.exe -L$(LIBPATH) main.o game.o libraryBindings.o -l$(LIBNAME) -lopengl32 -lgdi32 -lwinmm

SnakeClassic.exe: main.o gameClassic.o libraryBindings.o
	$(CXX) $(CXXFLAGS) -o SnakeClassic.exe -L$(LIBPATH) main.o gameClassic.o libraryBindings.o -l$(LIBNAME) -lopengl32 -lgdi32 -lwinmm

main.o: main.cpp game.h libraryBindings.h
	$(CXX) $(CXXFLAGS) -o main.o -c main.cpp

libraryBindings.o: libraryBindings.cpp libraryBindings.h
	$(CXX) $(CXXFLAGS) -D$(LIBDEFINE) -I$(INCLUDEPATH) -o libraryBindings.o -c libraryBindings.cpp

game.o: game.cpp game.h libraryBindings.cpp libraryBindings.h
	$(CXX) $(CXXFLAGS) -o game.o -c game.cpp

gameClassic.o: gameClassic.cpp game.h libraryBindings.cpp libraryBindings.h
	$(CXX) $(CXXFLAGS) -o gameClassic.o -c gameClassic.cpp

clean:
	del Snake.exe SnakeClassic.exe main.o libraryBindings.o game.o gameClassic.o
