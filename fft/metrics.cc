#include <iostream>

#include "fftw3handler.h"

extern "C"
{
#include <time.h>
}

#define SR	((unsigned long)(1ul << 14))
#define NYQ	((unsigned long)(SR >> 1))

int
main(void) {
	
	const int& n = SR;
	ffthandler h(0x4000, 0x4000);	
	
	for ( int x = 0 ; x < NYQ; ++x) {

	}
	
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
	std::cout << "SECS: " << sec << "\n";
	
}
