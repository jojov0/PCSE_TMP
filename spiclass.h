extern "C" {
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
}

#define BLOCK_SZ	(0x80)

static void pabort(const char *s)
{
	perror(s);
	abort();
}

static const char *device = "/dev/spidev1.0";

static uint8_t bits = 8;
static uint32_t speed = 1e6;

static uint8_t mode;
static uint16_t delay;

class spihandler {

	int fd;
	uint8_t tx[BLOCK_SZ];
	uint8_t rx[BLOCK_SZ];
	struct spi_ioc_transfer tr;

public:
	spihandler();
	~spihandler();
	void spi_tx(uint8_t *);
};

spihandler::spihandler() {
	int ret{0};
	for ( int x = 0 ; x < BLOCK_SZ ; ++x) {
		tx[x] = rx[x] = 0;
	}

	tr = {
		.tx_buf = (unsigned long)tx,
		.rx_buf = (unsigned long)rx,
		.len = BLOCK_SZ,
		.speed_hz = speed,
		.delay_usecs = delay,
		.bits_per_word = bits,
	};

	fd = open(device, O_RDWR);
	if (fd < 0)
		pabort("can't open device");

	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if (ret == -1)
		pabort("can't set spi mode");

	ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
	if (ret == -1)
		pabort("can't get spi mode");

	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't set bits per word");

	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't get bits per word");

	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't set max speed hz");

	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't get max speed hz");
}

spihandler::~spihandler() {
	close(fd);
}

void
spihandler::spi_tx(uint8_t * rx) {

	if (ioctl(fd, SPI_IOC_MESSAGE(1), &tr) < 1)
		pabort("can't send spi message");

	*rx = *(this->rx);
	for (int ret = 0; ret < BLOCK_SZ; ret++) {
		if (!(ret % 6))
			puts("");
		printf("%.2X ", rx[ret]);
	}
	puts("");
}
