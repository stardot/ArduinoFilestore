# Building the software
The project was built with the Arduino IDE 1.8.5, with the DUE library package installed. Earlier versions of the Arduino IDE
were found to produce poorly optimised code, that broke the tight timing cycles required to interface with the ADLC chip.

The following non-bundled libaries are also required to compile:
- SdFat library https://github.com/greiman/SdFat
- Time library https://github.com/PaulStoffregen/Time 

## Running for the first time
Check that your Arduino Serial Console is set to 115200 baud, or change the Serial.begin line to match your setup. As this is
still a development build, the console is very chatty. If the baud rate is wrong, the server will hang until the console connection is established.

You will need to set the IP addresses at the top of the EconetServer.ino file to something suitable for your network before
compiling, along with a staton number. I don't have a bridge available, so I'm unable to add anything other than single Econet
subnet functionality at the moment.

I've used a 4GB FAT32 formatted SD card in development. I'm assuming smaller and FAT16 cards will work, but it's not tested. As
the server starts up it will tell you the detected format and capacity of the card.

The default user SYST will be created with System Privilige and a blank password as the user profile directory is created on the 
SD card.

You can find more information in the docs folder in the repo.
