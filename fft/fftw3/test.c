#include <iostream>
#include <array>

#include "api/fftw3.h"

int 
main() {
	
	
	int N = 8;

/*
	double * d = new double[N];
	std::array<double, 8> a = {1,2,3,4,5,6,7,8};
	
	for ( int x = 0 ; x < N ; ++x ) {
		d[x] = a[x];
	}
	for ( int x = 0 ; x < N ; ++x ) {
		std::cout << d[x] << std::endl;
	}

	delete d;
	return 1;
*/


	fftw_complex *in, *out;
	fftw_plan p;

	in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
	out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

	for ( int x = 0 ; x < N ; ++x ){
		in[x][0] = 0;
		in[x][1] = 0;
	}

	//in[0][0] = 1;

	p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
	fftw_execute(p); 

	for ( int x = 0 ; x < N ; ++x ){
		printf("Real: %f\n", out[x][0]);
		printf("Imag: %f\n", out[x][1]);
	}

	fftw_destroy_plan(p);
	fftw_free(in); fftw_free(out);	
}
