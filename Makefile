CXX=g++
RM=rm -f
# Set this to the output of sdl2-config --cflags
SDL2CFLAGS=-I/opt/homebrew/include/SDL2 -D_THREAD_SAFE
INC_DIR = ./
CXXFLAGS=-O2 -c --std=c++20 -Wall $(SDL2CFLAGS) -I$(INC_DIR) -D DEBUG

# Load dynamic libs here. Add output of sdl2-config --libs
LDFLAGS=-L/opt/homebrew/lib -lSDL2

rt: main_rt.o image.o sdl_viewer.o sdl_timer.o raytrace.o
	$(CXX) $(LDFLAGS) -o rt main_rt.o image.o sdl_viewer.o sdl_timer.o raytrace.o

main_rt.o: main_rt.cpp
	$(CXX) $(CXXFLAGS) main_rt.cpp

image.o: image.cpp image.h
	$(CXX) $(CXXFLAGS) image.cpp

sdl_viewer.o: sdl_viewer.cpp sdl_viewer.h
	$(CXX) $(CXXFLAGS) sdl_viewer.cpp

sdl_timer.o: sdl_timer.cpp sdl_timer.h
	$(CXX) $(CXXFLAGS) sdl_timer.cpp

raytrace.o: raytrace.cpp raytrace.h
	$(CXX) $(CXXFLAGS) raytrace.cpp

clean:
	$(RM) rt *.o
