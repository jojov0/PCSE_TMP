#include "fft/fftw3handler.h"
#include "spiclass.h"

#define SR	((unsigned long)(1ul << 14))
#define NYQ	((unsigned long)(SR >> 1))

#define SAMPLES (SR)

static const char * pipe_format = "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d";

int main() {

	//FFT interface setup
	ffthandler fft(SAMPLES, SR);

	//ADC interface setup
	spihandler sh;
	short sample_cnt {0};
	uint8_t block[BLOCK_SZ];

	//latch count
	unsigned lcnt {0};
	const unsigned int& latch = 0x80;

	//block position count
	unsigned bpos {0};
	
	std::array<double, maxx> matrix_data;
	double fftdata[SAMPLES];

	//TEST

	/*
	while(1)  {					
		if (lcnt >= SAMPLES) {
			//execute FFT
			fft.read_data(fftdata);
			fft.make_fft();
			matrix_data = fft.db2spi();
			//pipe data to MATRIX handler
			printf(pipe_format,
				matrix_data[0x00], matrix_data[0x01],
				matrix_data[0x02], matrix_data[0x03],
				matrix_data[0x04], matrix_data[0x05],
				matrix_data[0x06], matrix_data[0x07],
				matrix_data[0x08], matrix_data[0x09],
				matrix_data[0x0a], matrix_data[0x0b],
				matrix_data[0x0c], matrix_data[0x0d],
				matrix_data[0x0e], matrix_data[0x0f],
				matrix_data[0x10], matrix_data[0x11],
				matrix_data[0x12], matrix_data[0x13],
				matrix_data[0x14], matrix_data[0x15],
				matrix_data[0x16], matrix_data[0x17],
				matrix_data[0x18], matrix_data[0x19],
				matrix_data[0x1b], matrix_data[0x1c],
				matrix_data[0x1d], matrix_data[0x1e],
				matrix_data[0x1f]);
			lcnt = bpos = 0;
		} else {
			sample_cnt = 0;
			sh.spi_tx(block);
			for ( unsigned int x = bpos ; x < (bpos + latch) ; ++x) {
				fftdata[x] = block[sample_cnt++];
			}
			bpos += latch;
		}
		++lcnt;
	}
	*/
}
