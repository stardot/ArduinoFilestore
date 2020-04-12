// Acorn Universal Networking (AUN) UDP packet handling routines

 void aunRXframe(int bytes){
  printTime();

  if (bytes>BUFFSIZE) {
    Serial.println (F(" Ignoring oversize AUN frame."));
    return;
  }
  if (bytes<8) {
    Serial.println (F(" Ignoring runt AUN frame."));
    return;
  }
  
  aunUdp.read(workBuff,BUFFSIZE);

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
  Serial.print (F(", transaction type = "));
  Serial.print (type,HEX);  

  if (type==0 || type>6) { 
    Serial.println (F(" - invalid transaction type."));
    return;
  }
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
    if (rxBuff[ptr1]<32 || rxBuff[ptr1]> 126){
      Serial.print (".");
    } else {
      Serial.write (workBuff[ptr1]);
    }
    Serial.print (" ");
  } 
  Serial.println (" ");

  // Process the frame
  switch (type){
    case 5:
      doImmediateAUNOp(controlByte);
      break;
      
    default:
      Serial.println (F("Unhandled transaction type"));
      break;
  }
}

 void doImmediateAUNOp(int optype){
  Serial.print(F("Immediate operation "));

  switch(optype){
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
      Serial.println(optype);
      break;
  }
}
