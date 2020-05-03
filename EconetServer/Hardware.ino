// Routines to interface with Econet hardware

void resetIRQ(){
  writeCR1(B00000010); // Enable RX interrupts, select address 1
  writeCR2(B01100001); // Clear RX and TX status, prioritise status 
}; 

void initADLC(){
  // Init Control Register 1 (CR1)
  writeCR1(B11000001); // Put TX and RX into reset, select address 2 for CR3 and 4
  // init Control Register 3 (CR3)
  digitalWriteDirect(PIN_Rs0,1);  //select control register 3 (becuase CR1 address bit =1)
  digitalWriteDirect(PIN_Rs1,0);
  digitalWriteDirect(PIN_D0,0); // No logical control byte
  digitalWriteDirect(PIN_D1,0); // 8 bit control field
  digitalWriteDirect(PIN_D2,0); // No auto address extend
  digitalWriteDirect(PIN_D3,0); // Idle mode all ones
  digitalWriteDirect(PIN_D4,0); // Disable Flag detect (not used in Econet)
  digitalWriteDirect(PIN_D5,0); // Disable Loop mode (not used in Econet)
  digitalWriteDirect(PIN_D6,0); // Disable active on poll (not used in Econet)
  digitalWriteDirect(PIN_D7,0); // Disable Loop on-line (not used in Econet)
 
  do {} while (digitalReadDirect(PIN_Cf)==0) ; //Wait for clock phase to go high
  do {} while (digitalReadDirect(PIN_Cf)==1) ; //Wait for clock phase to go low

  digitalWriteDirect(PIN_Cs,0); //Select
    do {} while (digitalReadDirect(PIN_Cf)==1) ; //Wait for clock phase to go low again
  digitalWriteDirect(PIN_Cs,1); //And deselect   

  // init Control Register 4 (CR4)
  digitalWriteDirect(PIN_Rs0,1);  //select control register 3 (becuase CR1 address bit =1)
  digitalWriteDirect(PIN_Rs1,1);
  digitalWriteDirect(PIN_D0,0); // Flag interframe control (not important in Econet) 
  digitalWriteDirect(PIN_D1,1); // Set TX word length to 8 bit
  digitalWriteDirect(PIN_D2,1); 
  digitalWriteDirect(PIN_D3,1); // Set RX word length to 8 bit
  digitalWriteDirect(PIN_D4,1); 
  digitalWriteDirect(PIN_D5,0); // No transmit abort 
  digitalWriteDirect(PIN_D6,0); // No Extended abort (not used in Econet)
  digitalWriteDirect(PIN_D7,0); // Disable NRZI encoding (not used in Econet)
  
  do {} while (digitalReadDirect(PIN_Cf)==0) ; //Wait for clock phase to go high
  do {} while (digitalReadDirect(PIN_Cf)==1) ; //Wait for clock phase to go low

  digitalWriteDirect(PIN_Cs,0); //Select
    do {} while (digitalReadDirect(PIN_Cf)==1) ; //Wait for clock phase to go low again
  digitalWriteDirect(PIN_Cs,1); //And deselect   


  // As CR1 and CR2 are regularly reset after an IRQ, they have been moved to a seperate routine
  resetIRQ();
}

void writeCR1(char txbyte){
    unsigned int regByte;

    if (!busWrite) { busWriteMode(); }; // Switch to output mode if necessarary  
    // Select appropriate write address
    digitalWriteDirect(PIN_Rs0,0);
    digitalWriteDirect(PIN_Rs1,0);  
  
    regByte=(txbyte <<1); //Load the bits in the right place on the port
   
    PIOC->PIO_OWER = 0x000001FE; // Mask out other addresses on the port
    REG_PIOC_ODSR = regByte; // Write byte directly to hardware registers

    do {} while (digitalReadDirect(PIN_Cf)==0) ; //Wait for clock phase to go high
    do {} while (digitalReadDirect(PIN_Cf)==1) ; //Wait for clock phase to go low

    digitalWriteDirect(PIN_Cs,0); //Select
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    digitalWriteDirect(PIN_Cs,1); //And deselect  
}

void writeCR2(char txbyte){
    unsigned int regByte;

    if (!busWrite) { busWriteMode(); }; // Switch to output mode if necessarary  
    // Select appropriate write address
    digitalWriteDirect(PIN_Rs0,1);
    digitalWriteDirect(PIN_Rs1,0);  
  
    regByte=(txbyte <<1); //Load the bits in the right place on the port
   
    PIOC->PIO_OWER = 0x000001FE; // Mask out other addresses on the port
    REG_PIOC_ODSR = regByte; // Write byte directly to hardware registers

    do {} while (digitalReadDirect(PIN_Cf)==0) ; //Wait for clock phase to go high
    do {} while (digitalReadDirect(PIN_Cf)==1) ; //Wait for clock phase to go low
    digitalWriteDirect(PIN_Cs,0); //Select
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    digitalWriteDirect(PIN_Cs,1); //And deselect  
}

void writeFIFO(char txbyte){
    unsigned int regByte;

    if (!busWrite) { busWriteMode(); }; // Switch to output mode if necessarary  
    // Select appropriate write address
    digitalWriteDirect(PIN_Rs0,0);
    digitalWriteDirect(PIN_Rs1,1);  
  
    regByte=(txbyte <<1); //Load the bits in the right place on the port
   
    PIOC->PIO_OWER = 0x000001FE; // Mask out other addresses on the port
    REG_PIOC_ODSR = regByte; // Write byte directly to hardware registers

    do {} while (digitalReadDirect(PIN_Cf)==0) ; //Wait for clock phase to go high
    do {} while (digitalReadDirect(PIN_Cf)==1) ; //Wait for clock phase to go low
    digitalWriteDirect(PIN_Cs,0); //Select
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    digitalWriteDirect(PIN_Cs,1); //And deselect  
}

byte readSR1(){
  unsigned int ioPort;

  if (busWrite) { busReadMode(); }; // Get bus in input mode
  
  digitalWriteDirect(PIN_Rs0,0);// Select Status Register 0
  digitalWriteDirect(PIN_Rs1,0);
  digitalWriteDirect(PIN_Cs,0); //Select chip (active low)
//  do {} while (digitalReadDirect(PIN_Cf)==0) ; //Wait for clock phase to go high  
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
  ioPort = g_APinDescription[PIN_D0].pPort -> PIO_PDSR ; // I know all data pins are on the same port, so cheating here!
  digitalWriteDirect(PIN_Cs,1); //And deselect

  return (ioPort >>1);
}

byte readSR2(){
  unsigned int ioPort;

  if (busWrite) { busReadMode(); }; // Get bus in input mode
  
  digitalWriteDirect(PIN_Rs0,1);
  digitalWriteDirect(PIN_Rs1,0);
  digitalWriteDirect(PIN_Cs,0); //Select chip (active low)
  // do {} while (digitalReadDirect(PIN_Cf)==0) ; //Wait for clock phase to go high  
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
  ioPort = g_APinDescription[PIN_D0].pPort -> PIO_PDSR ; // I know all data pins are on the same port, so cheating here!
  digitalWriteDirect(PIN_Cs,1); //And deselect

  return (ioPort >>1);
}

byte readFIFO(){
  unsigned int rawPort;

  if (busWrite) { busReadMode(); }; // Get bus in input mode
  
  digitalWriteDirect(PIN_Rs0,0);// Select FIFO read status
  digitalWriteDirect(PIN_Rs1,1);  
  digitalWriteDirect(PIN_Cs,0); //Select
  //do {} while (digitalReadDirect(PIN_Cf)==0) ; //Wait for clock phase to go high
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
  rawPort = g_APinDescription[PIN_D0].pPort -> PIO_PDSR ; // I know all data pins are on the same port, so cheating here!
  digitalWriteDirect(PIN_Cs,1); //And deselect
  return (rawPort >>1);
}

void printSR1(int stat){
    if (stat & 1) { Serial.print ("Data Available - "); };
    if (stat & 2) { Serial.print ("SR2 Read Request - "); };
    if (stat & 4) { Serial.print ("On Loop - "); };
    if (stat & 8) { Serial.print ("Flag detected - "); };
    if (stat & 16) { Serial.print ("CTS present - "); };
    if ((stat & 16)==0) { Serial.print ("CTS loss - "); };
    if (stat & 32) { Serial.print ("TX Underrun - "); };
    if (stat & 64) { Serial.print ("TDRA/FC - "); };
    if (stat & 128) { Serial.print ("IRQ"); };
}

void printSR2(int stat){
   if (stat & 1) { Serial.print ("Address present - "); };
   if (stat & 2) { Serial.print ("Frame Valid - "); };
   if (stat & 4) { Serial.print ("Net Idle - "); };
   if (stat & 8) { Serial.print ("Abort received - "); };
   if (stat & 16) { Serial.print ("Frame error - "); };
   if (stat & 32) { Serial.print ("DCD Lost - "); };
   if ((stat & 32)==0) { Serial.print ("DCD Present - "); };
   if (stat & 64) { Serial.print ("RX Overrun - "); };
   if (stat & 128) { Serial.print ("RX Data Available"); }; 
}

static  void digitalWriteDirect(int pin, boolean val){
  if(val) g_APinDescription[pin].pPort -> PIO_SODR = g_APinDescription[pin].ulPin;
  else    g_APinDescription[pin].pPort -> PIO_CODR = g_APinDescription[pin].ulPin;
}

static  int digitalReadDirect(int pin){
  return !!(g_APinDescription[pin].pPort -> PIO_PDSR & g_APinDescription[pin].ulPin);
}

void busReadMode(){
  
  digitalWriteDirect(PIN_RnW,1); // Read is active high
  PIOC->PIO_ODR =510; // Shortcut to set input mode

  busWrite=false;
}

void busWriteMode(){

  PIOC->PIO_OER =510; // Shortcut to set output mode
  PIOC->PIO_OWER = 0x000001FE; // Mask out other addresses on the port
  REG_PIOC_ODSR = 0; // Clear output registers 
  digitalWriteDirect(PIN_RnW,0); // Write is active low

  busWrite=true;
}  

// SPI bus control routines

void sdSelect(){
  digitalWriteDirect(ETHER_SPI_PIN,HIGH);
  digitalWriteDirect(SD_SPI_PIN,LOW);
}

void etherSelect(){
  digitalWriteDirect(SD_SPI_PIN,HIGH);
  digitalWriteDirect(ETHER_SPI_PIN,LOW);
}
