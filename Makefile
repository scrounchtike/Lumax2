
#makefile

SOURCES = $(wildcard *.cpp world/*.cpp applications/*.cpp rendering/*.cpp utils/*.cpp radiosity/*.cpp rendering/*.c baked_lighting/*.cpp resource_management/*.cpp DOOM/*.cpp engine/*.cpp)  
HEADERS = $(wildcard *.hpp world/*.hpp applications/*.hpp rendering/*.hpp utils/*.hpp radiosity/*.hpp rendering/*.h baked_lighting/*.hpp resource_management/*.hpp DOOM/*.hpp engine/*.hpp)

all: clean make

make: $(SOURCES)
	g++ -I/usr/local/Cellar/libpng/1.6.32/include/ -lpng -I/usr/local/Cellar/boost/1.65.0/include -L/usr/local/Cellar/boost/1.65.0/lib/ -lboost_filesystem -lboost_system -lboost_filesystem -std=c++1z -framework OpenGL -lglfw3 $(SOURCES)

clean:
	rm -rf *.o
	rm -rf *.out
