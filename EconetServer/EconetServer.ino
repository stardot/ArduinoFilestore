// Arduino Due Econet Module interface

#include <SPI.h>
#include <Ethernet.h>
#include "SdFat.h"
#include <time.h>
#include <TimeLib.h>

// Enter a MAC address for your controller below.
byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02};

byte ip[]{172,16,128,170};
byte dnsAddr[]{172,16,129,3};
byte gateway[]{172,16,128,254};
byte subnet[]{255,255,255,0};
IPAddress timeServer(172,16,129,2);

EthernetClient client;
EthernetUDP Udp;
unsigned int localUDPPort = 8888;  // local port to listen for UDP packets

#define MYSTATION 170 // Hex AA
#define MYNET 0 // TODO: Need to do bridge discovery and fix this
#define FSNAME "ArduinoFS" // Probably should use the volume name here
#define FSROOT "/export"
#define METAROOT "/meta"
#define PROFILEROOT "/users"

//Profile structure
#define PROFILE_PRIV 0
#define PROFILE_OPTS 1
#define PROFILE_ENABLED 2
#define PROFILE_PASSWORD 3
#define PROFILE_QUOTA 26
#define PROFILE_BALANCE 30
#define PROFILE_URD 34

// Protocol retries and timeouts set here!
#define SCOUTTIMEOUT 750 // Milliseconds
#define TXTIMEOUT 2000 // milliseconds
#define TXRETRIES 5 // Number of times to retry a failed frame TX
#define TXRETRYDELAY 100 //mS to wait between frame retries

#define BUFFSIZE 4096 // Size of TX, RX and work buffers
#define MAXUSERS 10 // Total number of user sessions
#define MAXDEPTH 20 // Maximum directory depth supported in CSD/LIB path
#define DIRENTRYSIZE (MAXDEPTH*11+2) // Each directory has 10 chars plus a seperator. Also complete string has a root and terminating character.
#define MAXFILES 100 // Total maximum number of files  and folders open - note the reply port for block operations is 129+filehandle - keep this in mind when expanding!
#define MAXUSERFILES 10 // Maximum files open per login

// Map the 17 Econet module pins to Arduino GPIO pins. D0-D8 are aligned with 8 contiguous bits on a port 
#define PIN_IRQ 28  // Econet module pin 1
#define PIN_RnW 29  // Econet module pin 2
#define PIN_Cs 30   // Econet module pin 3
#define PIN_Cf 41   // Econet module pin 4 - Clock feedback pin, also attached to clock on DAC1
#define PIN_Rs0 31  // Econet module pin 5
#define PIN_Rs1 32  // Econet module pin 6
#define PIN_D0 33   // Econet module pin 7
#define PIN_D1 34   // Econet module pin 8
#define PIN_D2 35   // Econet module pin 9 
#define PIN_D3 36   // Econet module pin 10
#define PIN_D4 37   // Econet module pin 11
#define PIN_D5 38   // Econet module pin 12
#define PIN_D6 39   // Econet module pin 13
#define PIN_D7 40   // Econet module pin 14
                    // Econet module pin 15 reset - linked to board reset line
                    // Econet module pin 16 gnd
                    // Econet module pin 17 +5v


#define SD_SPI_PIN 4
#define ETHER_SPI_PIN 10
#define SPI_SPEED SD_SCK_MHZ(24) // TODO: Need to fine tune this


byte rxBuff[BUFFSIZE];
byte txBuff[BUFFSIZE];
byte scoutBuff[8];
byte workBuff[BUFFSIZE];
char pathBuff1[DIRENTRYSIZE];
char pathBuff2[DIRENTRYSIZE];
char pathBuff3[DIRENTRYSIZE];
char pathBuff4[DIRENTRYSIZE];
boolean busWrite = false; // Global flag to indicate data bus direction

byte rxPort = 255;
byte rxControlByte=255;

// Reserve space for server tables
char userURD[DIRENTRYSIZE*MAXUSERS];
char userCSD[DIRENTRYSIZE*MAXUSERS];
char userLib[DIRENTRYSIZE*MAXUSERS];
byte netaddress[MAXUSERS];
byte stataddress[MAXUSERS];
byte userCSDhdl[MAXUSERS];
byte userLibhdl[MAXUSERS];
int userOpenFiles[MAXUSERS];
char userName[MAXUSERS*22]; // two level username of 10 chars, plus seperator, plus string termination
 
// Reserve space for filesystem tables
SdFat sd; // Card file system object.
FatFile fHandle[MAXFILES]; // Table of open filehandles
boolean fHandleActive[MAXFILES]; // Is this file handle currently active in the table?
char fHandlePath[DIRENTRYSIZE*MAXFILES]; // Path of the open file / folder
int fHandleUser[MAXFILES]; // Which user ID owns this filehandle
boolean fHandleIsFolder[MAXFILES]; // Is this an open folder, not a file?
byte fSequence[MAXFILES]; // Sequence number used in file transfers
byte dataAckPort[MAXFILES]; // Acknowledge port used in bulk uploads
byte dataReplyPort[MAXFILES]; // Reply port used in bulk uploads
int expectingBytes[MAXFILES]; // No of bytes to expect in a bulk upload
int gotBytes[MAXFILES]; // No if bytes so far in a bulk upload
boolean isSave[MAXFILES]; // Is the bulk upload a save (function 1) operation?


void setup() {
  
  Serial.begin(115200); // Make sure this matched your console, otherwise you will get no further running this!
  
  while (!Serial) {
    ; // wait for serial console to connect
  }

  Serial.println("Reset!");
  
  // Set initial pin config
  pinMode (PIN_LED, OUTPUT);
  digitalWriteDirect(PIN_LED,0);
  pinMode (PIN_RnW, OUTPUT);
  pinMode (PIN_IRQ, INPUT);
  pinMode (PIN_Cs, OUTPUT);
  digitalWriteDirect(PIN_Cs,1); //Deselect controller (active low)
  pinMode (PIN_Rs0, OUTPUT);
  pinMode (PIN_Rs1, OUTPUT);
  pinMode (PIN_Cf, INPUT);
  
  busWrite=false; // set busWrite to force busWriteMode to execute
  busWriteMode(); // Set databus to output mode 
  
  // PWM 2Mhz Clock Set-up on pin DAC1 for ADLC clock
  REG_PMC_PCER1 |= PMC_PCER1_PID36;                     // Enable PWM
  REG_PIOB_ABSR |= PIO_ABSR_P16;                        // Set pin perhipheral type B
  REG_PIOB_PDR |= PIO_PDR_P16;                          // Set PWM pin to an output - PIO_PB16X1_DAC1
  REG_PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(1);      // Set the PWM clock rate to 84MHz (84MHz/1)
  REG_PWM_CMR0 = PWM_CMR_CPRE_CLKA;                     // Enable single slope PWM and set the clock source as CLKA
  REG_PWM_CPRD0 = 42;                                   // Set the PWM frequency 84MHz/2Mhz = 42
  REG_PWM_CDTY0 = 21;                                   // Set the PWM duty cycle 50% (42/2=21)
  REG_PWM_ENA = PWM_ENA_CHID0;                          // Enable the PWM channel     

  //Set up the SPI pins
  pinMode (SD_SPI_PIN,OUTPUT);
  pinMode (ETHER_SPI_PIN,OUTPUT);
  digitalWriteDirect(SD_SPI_PIN,1);
  digitalWriteDirect(ETHER_SPI_PIN,1);

  // Now do the SD card stuff
  sdSelect();
  if (!sd.begin(SD_SPI_PIN, SPI_SPEED)) {
    // Check for possible errors
    if (sd.card()->errorCode()) {
      Serial.print("Serial initialisation failed error code: ");
      Serial.print (sd.card()->errorCode(),HEX);
      Serial.print(", data: ");
      Serial.print (sd.card()->errorData(),HEX);
      Serial.println();    
    }
    if (sd.vol()->fatType() == 0) Serial.println("Can't find a valid FAT16/FAT32 partition.");

    if (!sd.vwd()->isOpen()) Serial.println("Can't open root directory.\n");      

   Serial.println("Halting due to SD card initialisation error");
   while(1){ };
  }


  uint32_t size = sd.card()->cardSize();
  if (size == 0) {
    Serial.println("Can't determine card size");
  } else {
    uint32_t sizeMB = 0.000512 * size + 0.5;
    Serial.print("Card size is "); 
    Serial.print(sizeMB);
    Serial.print("MB, Volume is FAT");
    Serial.print(sd.vol()->fatType());
    Serial.print(", Cluster size (bytes): ");
    Serial.print(512L * sd.vol()->blocksPerCluster());
    Serial.print(", Free space (bytes): ");
    uint32_t freeBytes=(512L * sd.vol()->blocksPerCluster())*sd.vol()->freeClusterCount();
    Serial.println(freeBytes);
  }

  if(!sd.exists(FSROOT)){
    if(sd.mkdir(FSROOT)) Serial.println("Created FS Root directory"); else Serial.println("FS root not present, and failed to mkdir");  
  }

  if(!sd.exists(METAROOT)){
    if(sd.mkdir(METAROOT)) Serial.println("Created FS metadata directory"); else Serial.println("FS metadata folder not present, and failed to mkdir");  
  }


  if(!sd.exists(PROFILEROOT)){
    if(sd.mkdir(PROFILEROOT)) {
      Serial.println("Created FS user profile directory");
      String userProfile=PROFILEROOT;
      userProfile=userProfile+"/SYST";
      userProfile.toCharArray(pathBuff1, DIRENTRYSIZE);
      if (fHandle[0].open(pathBuff1, (O_RDWR | O_CREAT))){
        Serial.println("Creating default user");

        workBuff[PROFILE_PRIV]=255;     // Privilege is System
        workBuff[PROFILE_OPTS]=0;       // boot options
        workBuff[PROFILE_ENABLED]=1;    // Can login
        workBuff[PROFILE_PASSWORD]=0;   // No password
        // Allow 22 characters for the password to expand 
        workBuff[PROFILE_QUOTA]=0;      // Quota not used
        workBuff[PROFILE_QUOTA+1]=0;    // Quota not used
        workBuff[PROFILE_QUOTA+2]=0;    // Quota not used
        workBuff[PROFILE_QUOTA+3]=0;    // Quota not used
        workBuff[PROFILE_BALANCE]=0;    // Quota not used
        workBuff[PROFILE_BALANCE+1]=0;  // Quota not used
        workBuff[PROFILE_BALANCE+2]=0;  // Quota not used
        workBuff[PROFILE_BALANCE+3]=0;  // Quota not used
        workBuff[PROFILE_URD]=47;       // User directory is / (ASCII 47)
        workBuff[PROFILE_URD+1]=0;      // User directory termination
        
        fHandle[0].write(&workBuff,PROFILE_URD+2);
        fHandle[0].close();
      } else {
        Serial.println("Failed to create default user");
      }
    }
    else Serial.println("FS metadata folder not present, and failed to mkdir");  
  }

  Serial.println("SD card initialisation completed");

  // start the Ethernet connection:
  etherSelect();
  Ethernet.begin(mac,ip,dnsAddr,gateway,subnet);
  
  // Print local IP address:
  Serial.print("Ethernet config complete - IP address: ");
  Serial.print(Ethernet.localIP());

  Serial.println("");

  Udp.begin(localUDPPort);
  Serial.println("Waiting for NTP time sync...");

  int attempt=1;
  while (year()==1970 && attempt<11){
    setSyncProvider(getNtpTime);
    attempt++;
  }
  if (year()==1970) Serial.print(" No reply!");

  setSyncInterval(86400); // Once a day
  
  printTime();
  Serial.print(" ");
  printDate();
  Serial.println("");
  
  // Attaching SD filesystem callback for RTC
  SdFile::dateTimeCallback(dateTimeCB);
  
  // Initialise the Econet interface
  initADLC();
  
  int ptr=0;

  // Initialise the user tables
  for (ptr=0; ptr<MAXUSERS; ptr++){
    stataddress[ptr]=0;
    netaddress[ptr]=0;
    fHandleActive[ptr]=false;    
    fHandleUser[ptr]=-1;    
  }  
  
}


void loop() {
  unsigned long nextEvent,LCDupdate=0;
  byte statReg1, statReg2;
  boolean ledStatus=false, inFrame=false;
  int bufPtr=0, frameAddr=0;
  String lcdTime;

  // TODO: Bridge discovery, and set net address
  busWriteMode();
  listFS();
  busReadMode();

  Serial.println("Entering main loop");

  while (1){ // Enter main event loop

    if (!digitalReadDirect(PIN_IRQ)){
      //There is an IRQ to service
      statReg1=readSR1();
 
      if (statReg1 & 2){
        // SR2 needs servicing
        statReg2=readSR2();

        if (statReg2 & 1) { rxFrame(); };               // Address present in FIFO, so fetch it and the rest of frame
        if (statReg2 & 2 ) { readFIFO(); resetIRQ(); }; // Frame complete - not expecting a frame here, so read and discard 
        if (statReg2 & 4 ) { resetIRQ(); };             // Inactive idle - clear it quietly
        if (statReg2 & 8 ) { resetIRQ(); };             // TX abort received - not inside a frame here, so clear it 
        if (statReg2 & 16 ) { resetIRQ(); };            // Frame error - not inside a frame here, so clear it 
        if (statReg2 & 32) { Serial.println("No clock!"); resetIRQ(); }; // Carrier loss
        if (statReg2 & 64) { resetIRQ(); };             // Overrun - not inside a frame here, so clear it 
        if (statReg2 & 128) { readFIFO(); };            // RX data available - not inside a frame here, so read and discard it
 
      } else {
 
        //Something else in SR1 needs servicing        
        if (statReg1 & 1) { readFIFO(); }; // Not expecting data, so just read and ignore it!
        resetIRQ(); //Reset IRQ as not expecting anything!

      } // end of SR2 servicing
    } // end of IRQ to service 
  } // end of event loop  
} // Program loop






