// Acorn Universal Networking (AUN) UDP packet handling routines

 void aunRXframe(int bytes){
  etherSelect(); // Select ethernet interface on SPI bus
  
  printTime();

  if (bytes>(BUFFSIZE+8)) {
    Serial.println (F(" Ignoring oversize AUN frame."));
    return;
  }
  if (bytes<8) {
    Serial.println (F(" Ignoring runt AUN frame."));
    return;
  }
  
  aunUdp.read(workBuff,BUFFSIZE+8);

  int type=workBuff[0];
  int rxPort=workBuff[1];
  int controlByte=workBuff[2];
  aunSeqNo=(workBuff[7]<<24)+(workBuff[6]<<16)+(workBuff[5]<<8)+workBuff[4];
  
  Serial.print (F(" Received AUN frame of "));
  Serial.print (bytes);
  Serial.print (F(" bytes from "));
  currentAUNrxIP = aunUdp.remoteIP();
  for (int i =0; i < 4; i++)
    {
      Serial.print(currentAUNrxIP[i], DEC);
      if (i < 3)
      {
        Serial.print(".");
      }
    }

  // Process the frame
  // &01 = Broadcast &02=Data packet, &03=ACK, &04=NAK, &05=Immediate, &06 Imm.Reply
  switch (type){
    case 2:
      Serial.println();
      rxAUNframe(bytes);
      break;
    
    case 5:
      doImmediateAUNOp();
      break;
      
    default:
      Serial.print (F(", unhandled transaction type = "));
      Serial.print (type,HEX);  
      Serial.print (F(", control byte = "));
      Serial.print (controlByte,HEX);
      Serial.print (F(", port = "));
      Serial.print (rxPort,HEX);
      Serial.print (F(", sequence = "));
      Serial.print (aunSeqNo);

      if (bytes>24) bytes=24; // 16 bytes of data after 8 byte header
  
      Serial.print(F(", first 16 data bytes: ")); 
      for (int ptr1=8; ptr1 < bytes ; ptr1++){
        Serial.print (workBuff[ptr1],HEX);
        Serial.print (" ");
      }
      Serial.print ("= ");
      
      for (int ptr1=8; ptr1 < bytes ; ptr1++){
        if (workBuff[ptr1]<32 || workBuff[ptr1]> 126){
          Serial.print (".");
        } else {
          Serial.write (workBuff[ptr1]);
        }
        Serial.print (" ");
      } 
      Serial.println (" ");
      break;
  }
}

 void doImmediateAUNOp(){
  etherSelect(); // Select ethernet interface on SPI bus 
  
  Serial.print(F(" Immediate operation "));

  switch(workBuff[2]){ // control byte in buffer
    case 8:
      Serial.print(F("Machine Peek"));
      workBuff[0]=0x06; // Immediate operation reply
      workBuff[8]=0x0B; // Machine type 11 (Filestore) 
      workBuff[9]=0x00;
      workBuff[10]=0x02; // Version
      workBuff[11]=0x00;
      
      if (aunUdp.beginPacket(currentAUNrxIP, aunUDPsrcPort)){
        aunUdp.write(workBuff, 12);
        aunUdp.endPacket();
        Serial.println (F(" - Replied Filestore V2.00!"));   
      } else { 
        Serial.println (F(" - Reply tx failed.")); 
      };   
      break;

    default:
      Serial.print(F("- Unsupported operation type "));
      Serial.println(workBuff[2]);
      break;
  }
}

void rxAUNframe(int frameSize){
  etherSelect();  // Select ethernet interface on SPI bus
  
  if (!(workBuff[1]==0x99 || fHandleActive[workBuff[1]-129])) return; // Not expecting a frame on that port

  workBuff[0]=3; //ACK frame
  
  if (!aunUdp.beginPacket(currentAUNrxIP, aunUDPsrcPort)){
    Serial.print(F("- AUN ACK send failed "));
    return;
  }
    
  aunUdp.write(workBuff, 8);
  aunUdp.endPacket(); 
  
  // Translate AUN frame into Econet frame and process

  rxPort=workBuff[1]; // Set globals
  rxControlByte=workBuff[2];
  
  rxBuff[0]=0; // Address 0.0 isn't valid, this will be picked up later to indicate an AUN reception was involved
  rxBuff[1]=0;
  rxBuff[2]=0;
  rxBuff[3]=0;
  
  memcpy(rxBuff+4,workBuff+8,frameSize-8);

  if (workBuff[1]==0x99) fsOperation(frameSize-4); // 0x99 FS port
  if (workBuff[1]>129 && fHandleActive[workBuff[1]-129]) fsBulkRXArrived(workBuff[1],frameSize-2);
}

boolean txAUNframe(int port, int controlByte, int ecoFrameSize){

  long timeout;
  boolean gotAck;

  etherSelect(); // Select ethernet interface on SPI bus

  aunSeqNo+=4; // Increment AUN sequence no
  
  //Translate from Econet back to AUN
  workBuff[0]=2; // Data frame
  workBuff[1]=port;
  workBuff[2]=controlByte;
  workBuff[3]=0;
  
  memcpy(workBuff+8,txBuff+4,ecoFrameSize-4);

  aunSeqNo=aunSeqNo+4; // Increment AUN sequence no, and write to buffer

  workBuff[4]=aunSeqNo;
  workBuff[5]=aunSeqNo >> 8;
  workBuff[6]=aunSeqNo >> 16;
  workBuff[7]=aunSeqNo >> 24;
  
  if (aunUdp.beginPacket(currentAUNrxIP, aunUDPsrcPort)){

    // Send the packet
    aunUdp.write(workBuff, ecoFrameSize+4);
    aunUdp.endPacket();

    // Wait for acknowledgement
    timeout=millis()+1000;
    gotAck=false;

    while (timeout>millis() && !gotAck){
      int udpPacketSize=aunUdp.parsePacket();

      if (udpPacketSize==8){
        aunUdp.read(ackBuff,8);
        if (ackBuff[0]==3 && ackBuff[4]==workBuff[4] && ackBuff[5]==workBuff[5] && ackBuff[6]==workBuff[6] && ackBuff[7]==workBuff[7]) gotAck=true;
      }
    }

    if (!gotAck) return (false); // Failed to get ACK
    
    // Success   
    Serial.println (".");   
  } else { 
    Serial.println (F(" - AUN send failed."));
    return (false); 
  };    

  return (true);
}
