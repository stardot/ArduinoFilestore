# Building the software
The project was built with the Arduino IDE 1.8.5, with the DUE library package installed. Earlier versions of the Arduino IDE
were found to produce poorly optimised code, that broke the tight timing cycles required to interface with the ADLC chip.

The following non-bundled libaries are also required to compile:
- SdFat library https://github.com/greiman/SdFat
- Time library https://github.com/PaulStoffregen/Time 

