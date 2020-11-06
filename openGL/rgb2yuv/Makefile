CXXFLAGS=-g -O0 -Wall -std=c++11  -I "/usr/include/drm" -I "/usr/include"
LIBS=-lepoxy -lgbm -ldrm 

OBJ = main.o

all:rgb2nv12.out

%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

rgb2nv12.out: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f rgb2nv12.out $(OBJ)
