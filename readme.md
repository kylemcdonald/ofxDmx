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


## Enttec USB PRO Mk2 

To run the 2 DMX universes of the [Enttec USB PRO Mk2](http://www.enttec.com/?main_menu=Products&pn=70314&show=description) you need to first activate the second universe with `dmx.activateMk2()` and then specify which universe you address with `dmx.setLevel(channel, value, universe)`. 


## Running the Example Projects

Project files for the examples are not included so you will need to generate the project files for your operating system and development environment using the OF ProjectGenerator which is included with the OpenFrameworks distribution.

To (re)generate project files for an *existing* project:

* click the "Import" button in the ProjectGenerator
* navigate the to base folder for the project ie. "example"
* click the "Update" button

If everything went Ok, you should now be able to open the generated project and build/run the example.

## Dev Notes

Development Notes for updating to include the Enttec OpenDMX module

http://www.enttec.com/download/examples/OpenDMX.cs
