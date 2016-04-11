LIBS=-lboost_system -lboost_filesystem -lboost_thread -L/opt/boost/lib -L./ -lpthread -ldl -lrt  

CXXFLAGS = -g -Wall -fPIC -I/home/parallels/git/boost

CXX := g++

all: main

# build
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< 

# link
main : main.o
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(LIBS)

clean:
	rm -f main *.o *.so *.a
