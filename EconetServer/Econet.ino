// Native Econet frame handling routines

void rxFrame(){
  int ptr=0;
  byte stat=0;
  boolean frame=true;
//  static boolean gotScout=false; // This maintains our state in the 4 way handshake between calls.
  static unsigned long scoutTimeout=0;
  

  // First byte should be address
  rxBuff[0]=readFIFO();

  if (rxBuff[0] != config_Station && rxBuff[0] != 255 ) {
      // If frame is not for me, or a broadcast, then bail out
      resetIRQ();
      return;
  }

  // Wait for second byte (net address) 
  do {
    stat=readSR2();
  } while (!(stat & 250)); // While no errors, or available data - keep polling
  
  rxBuff[1]=readFIFO();
  
  if (rxBuff[1] != config_Net && rxBuff[1] != 0 && !(rxBuff[1] >252) ) {
      // If frame is not addressed to my network, or a broadcast, then bail out
      // Nets: 255 - global short broadcast (8 bytes), 254 - global large broadcast (1020/1024 maximum), 253 - local large broadcast  
      resetIRQ();
      return;
  }
  
  ptr=2;

  if (millis()>scoutTimeout && gotScout==true) { gotScout=false; Serial.println("rS!");} ;

  while (frame){     
   // Now check the status register
 
    do {
      stat=readSR2();
    } while (!(stat & 250)); // While no errors, or available data - keep polling  

    rxBuff[ptr]=readFIFO();
    if (ptr < BUFFSIZE) {ptr++;}; //TODO: keep overwriting last byte for now until I do this properly and abort the rx!
    
    if (stat & 122 ) frame=false; // If error or end of frame bits set, drop out
  } // End of while in frame - Data Available with no error bits or end set
 

  if (stat &2 ){ 
    // Frame is valid
  
    if (rxBuff[0]==255 || rxBuff[0]==0){
      //Broadcast frame - treat specially, then drop out of receive flow
      rxBroadcast(ptr);        
      return;
    };

    // Still here so a frame addressed to me - flag fill while we work out what to do with it
    
    flagFill(); // Flag fill and reset statuses - seems to need calling twice to clear everything
    flagFill();
    
    if (gotScout==false){
    
      // Am expecting a scout here, but could be an immediate op
      if (ptr>7){
        // Too big for a scout so must be an immediate operation
        doImmediateOpRX(ptr);      
      } else {
        // Acknowledge the scout, and set flag for next run
        
        // rxBuff[2] = Sender station
        // rxBuff[3] = Sender network
        // rxBuff[4] = ControlByte
        // rxBuff[5] = Port
        
        // I'm only interested in port 0x99 (FS protocol), and any bulk transfer ports open
        
        // Make a note of these, as we won't get them again in the payload
        rxControlByte=rxBuff[4];
        rxPort=rxBuff[5];

        if (rxPort==0x99 || fHandleActive[rxPort-129]){ 
          ackRX();
          gotScout=true;
          scoutTimeout=millis()+scoutTimeout;
        }
      }
     
    } else {

        // Have got a payload after the scout, acknowledge and process
        ackRX();
        gotScout=false;
        if (rxPort==0x99) { fsOperation(ptr); } else { fsBulkRXArrived(rxPort,ptr); };

    }
    
    // Do a final clear of any outstanding status bits
    delayMicroseconds(1);  
    rxReset();

  } else {
    // Frame not valid - what happened?

    Serial.print("RxFrame ended due to error: ");
    printSR2(stat);
    Serial.println("");

    // Reset the RXStatus bit
    delayMicroseconds(1);  
    rxReset();
    gotScout=false; 
  }  
}

void ackRX(){
  // Generate an acknowledgement packet 
  
  // Build the ack frame from the data in rx buffer  
  txBuff[0]=rxBuff[2]; 
  txBuff[1]=rxBuff[3];
  txBuff[2]=rxBuff[0];
  txBuff[3]=rxBuff[1];
  
  txFrame(4,false,false,false);  // And transmit it

  return;
}

void rxBroadcast (int bytes){
  if (bytes<6) return; // Spurious zero frame
  int controlByte=rxBuff[4];
  int rxPort=rxBuff[5];
  printTime();
  Serial.print (" Received broadcast of ");
  Serial.print (bytes-6);
  Serial.print (" bytes from ");
  Serial.print (rxBuff[3]);
  Serial.print (".");
  Serial.print (rxBuff[2]);
  Serial.print (", control byte = ");
  Serial.print (controlByte,HEX);
  Serial.print (", port = ");
  Serial.print (rxPort,HEX);
  Serial.print(", bytes: "); 
  for (int ptr1=6; ptr1 < bytes ; ptr1++){
    Serial.print (rxBuff[ptr1],HEX);
    Serial.print (" ");
  }
  Serial.print ("= ");
  for (int ptr1=6; ptr1 < bytes ; ptr1++){
    if (rxBuff[ptr1]<32 || rxBuff[ptr1]> 126){
      Serial.print (".");
    } else {
      Serial.write (rxBuff[ptr1]);
    }
    Serial.print (" ");
  } 
  Serial.println (" ");

  if (controlByte==0x80 && rxPort==0x99){
    // This is a server command broadcast - probably a ListFS
    printTime();
    Serial.print (" Sending ListFS response ");
    if (txBuff[7]=14) fsReadDiskInfo(rxBuff[6]);
  }
}

void doImmediateOpRX(int rxSize){
  // Perform the operation and generate an acknowledgement packet if valid

  byte opCode=rxBuff[4];

  printTime();
  Serial.print (" Immediate operation from ");
  Serial.print (rxBuff[3]);
  Serial.print (".");
  Serial.print (rxBuff[2]);
  Serial.print (", operation type  = ");
  Serial.print (opCode,HEX);

  // Now work out what has actually been requested!
  
  switch (opCode){
    case 0x81:
      // Peek
      Serial.println (" (Peek) - Ignoring!");
      // Don't acknowledge
      break;
      
    case 0x82:
      // Poke
      Serial.println (" (Poke) - Ignoring!");
      // Don't acknowledge
      break;

    case 0x83:
      // JSR
      Serial.println (" (JSR) - Ignoring!");
      // Don't acknowledge
      break;

    case 0x84:
      // User procedure call
      Serial.println (" (User procedure call) - Ignoring!");
      // Don't acknowledge
      break;

    case 0x85:
      // OS procedure call
      Serial.println (" (OS procedure call) - Ignoring!");
      // Don't acknowledge
      break;

    case 0x86:
      // Halt
      Serial.println (" (Halt) - Ignoring!");
      // Don't acknowledge
      break;

    case 0x87:
      // Continue
      Serial.println (" (Continue) - Ignoring!");
      // Don't acknowledge
      break;

    case 0x88:
      // Machine peek
      Serial.print (" (Machine peek)");

      // Build a response
      txBuff[0]=rxBuff[2]; 
      txBuff[1]=rxBuff[3];
      txBuff[2]=rxBuff[0];
      txBuff[3]=rxBuff[1];
/*
      txBuff[4]=0x01; // I am a BBC Micro (really!)
      txBuff[5]=0x00;
      txBuff[6]=0x3C; // Running NFS 3.60
      txBuff[7]=0x03;
      Serial.print (" - Replying BBC Micro NFS 3.60.....");
*/
      txBuff[4]=0x0B;
      txBuff[5]=0x00;
      txBuff[6]=0x02;
      txBuff[7]=0x00;
      
      if (txFrame(8,false,false,true)) { Serial.println (" - Replied Filestore V2.00!"); } else { Serial.println (" - Reply failed!"); };
      
      break;
      
    default:
      Serial.println (" (Unknown) ");
  }
    
  return;
}

void rxReset(){
  delayMicroseconds(2); // Give the last byte a moment to drain
  writeCR2(B00100001); // Clear RX status, prioritise status 
}

boolean txWithHandshake(int lastByte, int port, int controlByte){
  int attempt=0;
  while (attempt<txRetries){    
    if (txWithHandshakeInner(lastByte, port, controlByte)) return(true);
    attempt++;
    Serial.print("R!");
    delay(txRetryDelay);
  }
  return(false);
}

boolean txWithHandshakeInner(int lastByte, int port, int controlByte){
  //First generate the scout  
  scoutBuff[0]=txBuff[0];
  scoutBuff[1]=txBuff[1];
  scoutBuff[2]=txBuff[2];
  scoutBuff[3]=txBuff[3];
  scoutBuff[4]=controlByte;
  scoutBuff[5]=port;

  // Wait for network to become idle, or bail out if network error
  if (!waitIdle()) { Serial.println("Line Jammed"); return(false);};

  // Send the scout (waitforack=true, scout=true, immediate=false)
  if (!txFrame(6,true,true,false)) { 
    Serial.print("S!"); 
    return(false); 
  } 

//  if (!waitForAck()) {return(false);};
  
  // Now send the payload data (waitforack=true, scout=false, immediate=false)
  if (!txFrame(lastByte,true,false,false)) return(false);

//  if (!waitForAck()) {return(false);};
    
  return(true);
}

boolean waitIdle(){
  //Wait for network to become idle, returns false if network error or if not idle in 2 seconds.
  unsigned int byte1;
  int sr1,sr2; 

//  digitalWriteDirect(PIN_LED,1);
  
  unsigned long timeOut=millis()+txBeginTimeout;


  sr2=0; // Force while loop to run first time
  while (!(sr2 & 4)){
    delay(50);
    sr1=readSR1();
    sr2=readSR2();

    if (sr2 & 32){ //DCD loss (no clock)
      Serial.print("Error: ");
      printSR1(sr2);
      Serial.println();
      return (false);
    }
    
    if (millis()>timeOut){
      Serial.println("Network not idle for txBeginTimeout milliseconds - Line Jammed");
      return(false);
    }

    resetIRQ();

  }
//  digitalWriteDirect(PIN_LED,0);
  
  return(true);
  
}

boolean txFrame(int bytes, boolean getAck, boolean scout, boolean immediate){
  int sr1,sr2;
  unsigned long timeOut;
  boolean ackResult=false, done=false;
  
  writeCR1(B00000000); // Disable RX interrupts

  timeOut=millis()+txBeginTimeout;
  
  while(!(readSR1() & 64)){ // If we don't have TDRA, clear status until we do!  
    writeCR2(B11100101); // Raise RTS, clear TX and RX status, flag fill and Prioritise status
  }

  delayMicroseconds(10); // Give the other clients a moment to notice the flag fill 
 
  for(int buffPtr=0;buffPtr<bytes;buffPtr+=1){
   
    while(true){ // While not TDRA set, loop until it is - or we get an error
      sr1=readSR1();
      if (sr1 & 64) break; // We have TDRA
      if (sr1 & 192) { // Some other error
        printSR1(sr1); 
        resetIRQ(); 
        return(false);
        };
      if (millis()>timeOut){Serial.print("TX timeout on frame "); Serial.print(buffPtr); resetIRQ(); return(false);}
    }
  
    // Now we are ready, write the byte.
    if (scout) writeFIFO(scoutBuff[buffPtr]); else writeFIFO(txBuff[buffPtr]);
    
  } // End of for loop to tx bytes

  writeCR2(B00111001); // Tell the ADLC that was the last byte, and clear flag fill modes and RTS. 
  writeCR1(B00000100); // Tx interrupt enable
  
 // Do a last check for errors
 while (digitalReadDirect(PIN_IRQ)){}; // Wait for IRQ
 
 sr1=readSR1();
 if (!sr1 & 64){ // Something other than Frame complete happened
   printSR1(sr1);
   resetIRQ();
   return(false);                
 }
 
  writeCR2(B01100001); //Clear any pending status, prioritise status
  writeCR1(B00000010); //Suppress tx interrupts, Enable RX interrupts

  //  return(true);

  if (!getAck) return (true); // If ack not expected,  return now

  if (waitForAck()) return(true);
  return (false);
}

void flagFill(){
  writeCR2(B11100100); // Set CR2 to RTS, TX Status Clear, RX Status clear, Flag fill on idle)
}

boolean waitForAck(){
  byte statReg1, statReg2;
  boolean ackResult=false, inLoop=true;
 
  unsigned long timeOut=millis()+ackTimeout;

  resetIRQ();

  while (inLoop){ // Enter IRQ polling loop
    if (millis()>timeOut) { 
      inLoop=false;
    }

    if (!digitalReadDirect(PIN_IRQ)){
      //There is an IRQ to service
      statReg1=readSR1();
      
      if (statReg1 & 2){
        statReg2=readSR2();

        if (statReg2 & 1) { ackResult=checkAck();};     // Address present in FIFO, so fetch it and the rest of frame
        if (statReg2 & 2 ) { readFIFO(); resetIRQ(); }; // Frame complete - not expecting a frame here
        if (statReg2 & 4 ) { resetIRQ(); };             // Inactive idle 
        if (statReg2 & 8 ) { resetIRQ(); };             // TX abort received 
        if (statReg2 & 16 ) { resetIRQ(); };            // Frame error 
        if (statReg2 & 32) { Serial.println("No clock!"); resetIRQ(); }; // Carrier loss
        if (statReg2 & 64) { resetIRQ(); };             // Overrun  
        if (statReg2 & 128) { readFIFO(); };            // RX data available 
      } else {
        //Something else in SR1 needs servicing        
        if (statReg1 & 1) { readFIFO(); }; // Not expecting data, so just read and ignore it!
        resetIRQ(); //Reset IRQ as not expecting anything!
      }// end of if SR2 needs servicing
    } // end of IF IRQ to service
    
    if (ackResult) inLoop=false;
   
  } // end of event loop


  return(ackResult);
}

boolean checkAck(){

  byte theirStation=scoutBuff[2];
  byte theirNet=scoutBuff[3];
  
  int ptr=0;
  byte stat=0;
  boolean frame=true;
  static boolean gotScout=false; // This maintains our state in the 4 way handshake between calls.

  // First byte should be address
  rxBuff[0]=readFIFO();

  if (rxBuff[0] != config_Station) {
      // If frame is not for me, then bail out
      resetIRQ();
      return(false);
  }
  
  ptr++;

  while (frame){
    
    // Now check the status register
    do {
      delayMicroseconds(1); // Even at the fastest clock rate, it still takes several uS to get another byte.      
      stat=readSR2();
    } while (!(stat & 250)); // While no errors, or available data - keep polling  

    rxBuff[ptr]=readFIFO();
    if (ptr < BUFFSIZE) {ptr++;}; //keep overwriting last byte for now until I do this properly!


    if (stat & 122 ) frame=false; // If error or end of frame bits set, drop out
  } // End of while in frame - Data Available with no error bits or end set
 

   // Frame is valid
   if(ptr!=4) return(false); // frame is wrong size for an ack
   if((rxBuff[1]==theirNet) && (rxBuff[0]==theirStation)) return (true);
 
  return (false);   // Not bothering with any other error checking - if we are here the frame is wrong regardless of cause.
}

void listFS(){
  txBuff[0]=0xFF; 
  txBuff[1]=0xFF;
  txBuff[2]=config_Station;
  txBuff[3]=config_Net;
  txBuff[4]=0x80;
  txBuff[5]=0x99;
  txBuff[6]=0x06;
  txBuff[7]=0x0E;
  txBuff[8]=0x00;
  txBuff[9]=0x00;
  txBuff[10]=0x00;
  txBuff[11]=0x00;
  txBuff[12]=0x04;
  txBuff[13]=0x00;

  if (!txFrame(14,false,false,false)){ Serial.println(F("Failed to send ListFS"));};
}

void bridgeProbe(){
  txBuff[0]=0xFF; 
  txBuff[1]=0xFF;
  txBuff[2]=config_Station;
  txBuff[3]=0;
  txBuff[4]=0x82;
  txBuff[5]=0x9C;
  txBuff[6]=0x42;
  txBuff[7]=0x72;
  txBuff[8]=0x69;
  txBuff[9]=0x64;
  txBuff[10]=0x67;
  txBuff[11]=0x65;
  txBuff[12]=0x9C;
  txBuff[13]=0x00;

  if (!txFrame(14,false,false,false)){ Serial.println(F("Failed to send bridge discovery broadcast"));};
}
