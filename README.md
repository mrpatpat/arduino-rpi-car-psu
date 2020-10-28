This is a simple power supply unit for Raspberry Pi usage in cars. It is made of stuff I had at home and is not the most efficient design in any way.

* takes 12V, GND and IGN as inputs
* steps down 12V to 5.15V suitable for the Arduino Nano and Raspberry Pi
* steps the ignition signal down to 3.3V suitable for the input on the Arduino Nano
* outputs a triggered 5.15V source for powering the Raspberry Pi
* the source is turned on when the ignition gets turned on
* the source stays on for some time even when the ignition turns off to debounce quick changes
* before cutting power to the Raspberry completely off the Arduino grounds an output so the Raspberry Pi can shutdown correctly before losing power

# Parts 

* Arduino Nano
* 3A DC-DC Buck Converter
* generic Mosfet
* LDV33
* some connectors and a perf board

# TODOs
* pictures
* add some circuit drawings and integrations with the Raspberry
* add Raspberry shutdown script
* add reverse polarity protection through some diodes
* add voltage spike protection through some varistor
* add current protection through a 3A automotive fuse
* create a 3D-printable housing
* create a gerber file and schematics for a nicer design

