#include <iostream>
#include "fftw3handler.h"

extern "C" {
#include <time.h>
}

//#define n 	(512)
#define SR	(16e3)

int
main(void) {
	

	//2**14 = 16384 Hz -> nyquist = 8192 Hz
	int n = 0x4000;
	//int n = 32;
	ffthandler h(0x4000, 0x4000);	
	//ffthandler h(32, 32);	

	//ffthandler h(16, 16e3);	
	
	double sr = 0x4000;
	//double sr = 16;
	double tf = 1/sr;
	double d[n]= {0};
	double v = 0;
	for ( int x = 0 ; x < n; ++x, v+= tf) {
		//d[x] = 0;

		//n == should be same as == sr	
		//fmax <= n/2 - 1
		//fmax <= sr/2 - 1
		d[x] = std::sin(2*M_PI*v*8190);
	}
	//d[0] = 1;
	
	h.read_data(d);
	h.hann_window();
	clock_t s = clock();
	h.make_fft();
	clock_t e = clock();
	float sec = (float)(e - s)/CLOCKS_PER_SEC;

	//h.print_data();

	const auto& fdb = h.filter_todb(67.0);
	
	for( const auto& x : fdb) {
		std::cout << x[0] << " -> " << x[1] << "\n";
	}

	/*
	const auto& db = h.todb();
	int c = 0;
	for( const auto& x : db) {
		
		std::cout << c++ << ". ";
		if (x > .0)
		std::cout << "##### DB: " << x << "\n";
		else
		std::cout << "DB: " << x << "\n";
	}
	*/
	std::cout << "SECS: " << sec << "\n";
	
}
