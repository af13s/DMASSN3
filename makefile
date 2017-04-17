CC      = g++
#CFLAGS  = -I/home/scale/g++Projects/gLib/
#LDFLAGS = -lfltk
CXXFLAGS = -std=c++11

all: NaiveBayes clean

octhecdec: NaiveBayes.o
	$(CC) -o $@ $^ $(LDFLAGS)

octhecdec.o: NaiveBayes.cpp
	$(CC) -c $(CFLAGS) $(CXXFLAGS) $<

.PHONY: clean cleanest

clean:
	rm *.o

cleanest: clean
	rm NaiveBayes
