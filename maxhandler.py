#!/bin/python3

import time

from PIL import Image, ImageDraw
from luma.led_matrix.device import max7219
from luma.core.interface.serial import spi, noop
from luma.core.render import canvas

class maxhandler:
    
    """
            Parameters
            devices: nr. of cascaded MAX7219 devices
            nyquist: nyquist shannon frequency
            maxdb: scale db value to a specific treshhold
    """
    def __init__(self, devices, nyquist, maxdb):
        if devices < 1: devices = 1
        self.serial = spi(port=0, device=0, gpio=noop())
        self.device = max7219(self.serial, cascaded=devices, block_orientation=-90, rotate=0, blocks_arranged_in_reverse_order=0)

        self.img = Image.new(self.device.mode, self.device.size)
        self.cnvs = ImageDraw.Draw(self.img)

        self.sz = (devices << 3)

        #play around with
        self.device.contrast((1 << 8) - 1)

        self.nyq = nyquist 
        #each device has 8 columns
        self.delta = nyquist/self.sz
        self.db = maxdb

        #self.last_write = [0] * (0x40 << (devices - 1))

    def flush(self):
        self.device.display(self.img)

    #clear all devices
    def clear(self):
        self.cnvs.rectangle((0, 0, self.sz - 1, self.sz - 1), fill="black")
        self.device.display(self.img)

    def write_freq(self, freq, db):
        col = freq//self.delta
        pixels = (db << 3)//self.db

        for x in range(pixels):
            self.cnvs.point((col, 7 - x), fill="white")

        self.device.display(self.img)

    def write_freq_noflush(self, freq, db):
        col = freq//self.delta
        pixels = (db << 3)//self.db

        for x in range(pixels):
            self.cnvs.point((col, 7 - x), fill="white")

        #self.device.display(self.img)
        """
        with canvas(self.device) as draw:
            draw.point((0,0), fill="white")
        time.sleep(1)
        with canvas(self.device) as draw:
            draw.point((0,0), fill="black")
        with canvas(self.device) as draw:
            for x in range(8 - (pixels - 1), pixels, - 1):
                draw.point((col, x), fill="white")
        """

if __name__ == "__main__":
    
    #check maxdb
    maxdb = 60
    mh = maxhandler(4, 0x2000, maxdb)
    mh.clear()
    
    #
    time.sleep(2)
    #
    while(True):
        try:
            val = [int(x) for x in input().split(',')]
            sz = len(val)

#            for x in range(sz):
#                mh.write_freq(x*250, val[x])
#            time.sleep(0.02)
#            mh.clear()

            for x in range(sz):
                mh.write_freq_noflush(x*250, val[x])
            mh.flush()
            #time.sleep(0.4)
            mh.clear()
        except:
            pass
    """
    mh.write_freq(100, 60)
    time.sleep(1)
    mh.write_freq(1000, 50)
    time.sleep(1)
    mh.write_freq(4000, 20)
    time.sleep(1)
    mh.write_freq(7000, 30)
    time.sleep(10)
    """
