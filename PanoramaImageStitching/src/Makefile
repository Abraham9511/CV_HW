opencv:
	$(CC) opencv_test.cc $(LDFLAGS) -std=c++11 -g -Wall -o opencv

.PHONY: clean

clean:
	rm -rf opencv

CC = g++
CFLAGS = 
LDFLAGS = `pkg-config --libs opencv` -lm
#ALL = vision

#all: $(ALL)

#vision: vision.o
#    $(CC) $(LDFLAGS) -o $@ $^

#vision.o: vision.cpp
#    $(CC) $(LDFLAGS) -c $<

