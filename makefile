CC		= g++
CCFLAGS		= -lfftw3 -lm -Wall -std=c++17

.PHONY: clean

spectro: spectro.cc
	$(CC) $< $(CCFLAGS) -o $@

clean:
	rm -f metrics
