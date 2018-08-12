INCLUDE = -I../../ \

CPPFLAGS = -Wall -fPIC $(INCLUDE) $(DEBUG_DEF) -std=c++11 -g -O0
LIBS = libgrid.a
EXE = hashtable_test

all : clean $(LIBS) $(EXE)

clean:
	rm -rf *.o *.a

libgrid.a: lbs_hashtable.o lbs_grid.o lbs_bitmap.o lbs_heap.o lbs_index.o
	ar cr $@ $^

hashtable_test: lbs_hashtable_test.o
	g++ $(CPPFLAGS) -o $@ $^ $(LIBS)
	
.o : .cpp
	g++ $(CPPFLAGS) -c $^
