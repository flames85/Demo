LIBS=-lboost_system -lboost_filesystem -lboost_thread -L/opt/boost/lib -L./ -lpthread -ldl -lrt  

CXXFLAGS = -O2 -Wall -fPIC -I/home/parallels/git/boost

CXX := g++

all: main2

# build
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< 

# link
main2 : main2.o
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(LIBS)

clean:
	rm -f main *.o *.so *.a
