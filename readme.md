# ofxDmx wraps DMX over serial control for [openFrameworks](http://openframeworks.cc/)

ofxDmx is based on DmxPro from Erik Sjodin, later adapted by Marek Bereza.

ofxDmx is targeted at the [Enttec DmxPro](http://www.enttec.com/index.php?main_menu=Products&prod=70304&show=description) running the virtual serial port drivers [available from FTDI](http://www.ftdichip.com/Drivers/VCP.htm).

## About Channel Numbering

DMX is an unusual specification in that channels begin at `1`. There is no `0` channel. This means that if you have 24 channels, they will be numbered `1, 2, 3... 24`. They will not be numbered `0,1,2... 23`. Because of this, you have to more careful when you write for loops. This is correct:

	int total = 16;
	dmx.connect("tty.usbserial-ENS8KBA6", total);
	for(int channel = 1; channel <= total; channel++) {
		dmx.setLevel(channel, ...);
	}

Notice that the for loop starts at `1`, and the stopping condition is `channel <= total` rather than the usual `<` operator.