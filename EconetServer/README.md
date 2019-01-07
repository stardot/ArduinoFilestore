# Building the software
The project was built with the Arduino IDE 1.8.5, with the DUE library package installed. Earlier versions of the Arduino IDE
were found to produce poorly optimised code, that broke the tight timing cycles required to interface with the ADLC chip.

The following non-bundled libaries are also required to compile:
- SdFat library https://github.com/greiman/SdFat
- Time library https://github.com/PaulStoffregen/Time 

## Running for the first time
Check that your Arduino Serial Console is set to 115200 baud, or change the Serial.begin line to match your setup. As this is
still a development build, the console is very chatty. If the baud rate is wrong, the server will hang until the console connection is established.

The server will default to DHCP on the ethernet connection, and station 170 (0xAA) on the Econet. I don't have a bridge available, so I'm unable 
to add anything other than single Econet subnet functionality at the moment.

I've used a 4GB FAT32 formatted SD card in development. I'm assuming smaller and FAT16 cards will work, but it's not tested. As
the server starts up it will tell you the detected format and capacity of the card.

The default user SYST will be created with System Privilige and a blank password as the user profile directory is created on the 
SD card.

You can find more information in the docs folder in the repo.

## Malformed frames seen in *NETMON
There seems to be an issue introduced with some versions of the Arduino SAM support libraries / compiler which breaks the very tight timing loops
this project depends on to talk to the ADLC. This manifests itself as the device being able to mostly decode frames correctly, but any transmission starts 
with one or more 0xFF bytes, and 0xFF bytes appear in unexpected places during the frame.

I'm currently building the sources with the 1.6.6 SAM Cortex support libraries. Newer libraries may be compatible (1.6.13 isn't), but I've not had
a chance to pin down at which revision the problem is introduced. If you are seeing malformed frames being generated in Netmon, then this should be one 
of the first things to be checked.