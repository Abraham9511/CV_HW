stitching:
	$(CC) stitching.cc $(LDFLAGS) -std=c++11 -g -Wall -o stitching

.PHONY: clean

clean:
	rm -rf stiching

CC = g++
CFLAGS = 
LDFLAGS = `pkg-config --libs opencv` -lm
#ALL = vision

#all: $(ALL)

#vision: vision.o
#    $(CC) $(LDFLAGS) -o $@ $^

#vision.o: vision.cpp
#    $(CC) $(LDFLAGS) -c $<

