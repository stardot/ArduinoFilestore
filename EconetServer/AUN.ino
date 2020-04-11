// Acorn Universal Networking (AUN) UDP packet handling routines

 void aunRXframe(int bytes){
  printTime();

  if (bytes>BUFFSIZE) {
    Serial.println (" Ignoring oversize AUN frame.");
    return;
  }
  if (bytes<8) {
    Serial.println (" Ignoring runt AUN frame.");
    return;
  }
  
  aunUdp.read(rxBuff,BUFFSIZE);

  int type=rxBuff[0];
  int rxPort=rxBuff[1];
  int controlByte=rxBuff[2];
  unsigned long aunSeq=(rxBuff[7]<<24)+(rxBuff[6]<<16)+(rxBuff[5]<<8)+rxBuff[4];
  
  Serial.print (" Received AUN frame of ");
  Serial.print (bytes);
  Serial.print (" bytes from ");
  IPAddress remIP = aunUdp.remoteIP();
  for (int i =0; i < 4; i++)
    {
      Serial.print(remIP[i], DEC);
      if (i < 3)
      {
        Serial.print(".");
      }
    }
  Serial.print (", transaction type = ");
  Serial.print (type,HEX);  

  if (type==0 || type>6) { 
    Serial.println (" - invalid transaction type.");
    return;
  }
  Serial.print (", control byte = ");
  Serial.print (controlByte,HEX);
  Serial.print (", port = ");
  Serial.print (rxPort,HEX);
  Serial.print (", sequence = ");
  Serial.print (aunSeq);

  if (bytes>24) bytes=24; // 16 bytes of data after 8 byte header
  
  Serial.print(", first 16 data bytes: "); 
  for (int ptr1=8; ptr1 < bytes ; ptr1++){
    Serial.print (rxBuff[ptr1],HEX);
    Serial.print (" ");
  }
  Serial.print ("= ");
  
  for (int ptr1=8; ptr1 < bytes ; ptr1++){
    if (rxBuff[ptr1]<32 || rxBuff[ptr1]> 126){
      Serial.print (".");
    } else {
      Serial.write (rxBuff[ptr1]);
    }
    Serial.print (" ");
  } 
  Serial.println (" ");
  
 }
