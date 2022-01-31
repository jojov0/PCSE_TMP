CC		= g++
CCFLAGS		= -lfftw3 -lm -Wall -std=c++17

.PHONY: clean

bin: metrics.cc
	$(CC) $< $(CCFLAGS) -o $@

clean:
	rm -f metrics
