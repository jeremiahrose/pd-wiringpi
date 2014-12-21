pd-wiringpi
===========

PureData externals for accessing the Raspberry Pi GPIO pins using wiringpi.

Forked from Jaime Oliver La Rosa's Sourceforge repository:
http://nyu-waverlylabs.org/rpi-gpio/

To compile these objects cd into each of their directories and write something like "make wiringPi_gpio.l_arm". As expected you need to change the name of each object that you compile to correspond to the one that you are actually trying to compile.
Finally, it's been reported that B+ boards need to be compiled with the extension .pd_linux instead of .l_arm 
