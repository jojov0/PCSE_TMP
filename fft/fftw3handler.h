#ifndef __FFTH__
#define __FFTH__

extern "C"
{
#include "fftw3/api/fftw3.h"
}

#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <cmath>

//matrix LED configurations

#define cdevs		(4)
#define maxx		(cdevs << 3)

class ffthandler {

	const int& n;
	int nyquist;
	const double& sampling_rate;

	double Tf;
	double window_len;

	fftw_plan p;
	fftw_complex * out;

	double * fftwin;
	
public:
	ffthandler() = delete;
	ffthandler(int&&, double&&);
	~ffthandler();

	void hann_window();
	void dwindow();

	void read_data(const double * cont);
	
	void print_data() const;

	void make_fft();

	const fftw_complex * ret_raw_out() const;
	std::vector<double> todb() const;
	std::vector<std::array<double,2>> filter_todb(const int&) const;
	std::array<double, maxx> db2spi() const;
};

ffthandler::ffthandler(int&& n, double&& SR)
: n(std::move(n)), sampling_rate(std::move(SR)) {
	
	Tf = 1.0/sampling_rate;	
	window_len = n * Tf;

	nyquist = (n >> 1) + 1;

	//allocate first -> fftw_plan dereferences pointer
	fftwin = new double[n];

	//half of output is complex conjugated
	out = static_cast<fftw_complex*>(fftw_malloc(sizeof(fftw_complex) * nyquist));
	p = fftw_plan_dft_r2c_1d(n, fftwin, out, 0);
	if (out == NULL || p == NULL) {
		std::cout << "[!] Error on allocating memory in constructor!\n";
	}
}

ffthandler::~ffthandler() {
	fftw_destroy_plan(p);
	fftw_free(out);
	delete fftwin;
}

void
ffthandler::hann_window() {
	double hannv {0};
	//create map ?
	for ( int x = 0 ; x < n ; ++x ) {
		hannv = std::sin((M_PI * x)/n);
		fftwin[x] *= hannv * hannv;
	}
}

void
ffthandler::read_data(const double * cont) {
	for ( int x = 0 ; x < n ; ++x ) fftwin[x] = cont[x];
}

void
ffthandler::make_fft() {
	fftw_execute(p);
}

const fftw_complex *
ffthandler::ret_raw_out() const {
	return out;
}

void
ffthandler::print_data() const {
	for (int x = 0 ; x < nyquist ; ++x ) {
		std::cout << out[x][0] << " - " << out[x][1] << "\n";
	}
}

std::vector<double>
ffthandler::todb() const {
	//calculate absolute complex values	
	double abs_cmplx {1};
	std::vector<double> ret(nyquist, .0);
	for (int x = 0 ; x < nyquist ; ++x ) {
		abs_cmplx = (out[x][0] * out[x][0]) + (out[x][1] * out[x][1]);
		ret[x] = 10 * log10(abs_cmplx);
	}	
	return ret;
}

std::vector<std::array<double,2>>
ffthandler::filter_todb(const int& treshhold) const {
	//calculate absolute complex values	
	double abs_cmplx {1}, tmp{0};
	std::vector<std::array<double,2>> filtered;
	for (int x = 0 ; x < nyquist ; ++x ) {
		abs_cmplx = (out[x][0] * out[x][0]) + (out[x][1] * out[x][1]);
		tmp = 10 * log10(abs_cmplx);
		if (tmp > treshhold) {
			filtered.push_back({x, tmp});
		}	
	}	
	return filtered;
}

std::array<double, maxx>
ffthandler::db2spi() const {

	double cmax {0};
	std::array<double, maxx> ret {0, };

	double abs_cmplx {1}, tmp{0};
	for (int x = 0 ; x < nyquist ; ++x ) {
		abs_cmplx = (out[x][0] * out[x][0]) + (out[x][1] * out[x][1]);
		tmp = 10 * log10(abs_cmplx);
		if (!(x % maxx)) {
			ret[x/maxx] = cmax;	
			cmax = 0;
		} else {
			if (tmp > cmax) cmax = tmp;
		}
	}	
	return ret;
}

#endif
