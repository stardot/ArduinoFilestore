// All RTC code from https://github.com/PaulStoffregen/Time

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets
const int timeZone = 0;     // GMT

time_t getNtpTime(){
  etherSelect();
  Serial.print("getNtpTime called... ");
  while (ntpUdp.parsePacket() > 0) ; // discard any previously received packets
  //Serial.println("NTP Request sent ");
  sendNTPpacket(timeServer);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = ntpUdp.parsePacket();

    if (size >= NTP_PACKET_SIZE) {
      //Serial.println("Receive NTP Response");
      ntpUdp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      Serial.println("Success!");
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No response.");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address){
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:                 
  ntpUdp.beginPacket(address, 123); //NTP requests are to port 123
  ntpUdp.write(packetBuffer, NTP_PACKET_SIZE);
  ntpUdp.endPacket();
}

void printTime(){
  printDigits(hour(),"");
  printDigits(minute(),":");
  printDigits(second(),":");
};

void printDate(){
  printDigits(day(),"");
  printDigits(month(),"/");
  printDigits(year(),"/");  
}
/*
void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 
}
*/
void printDigits(int digits, String seperator){
  // utility for digital clock display: prints preceding colon and leading 0
  Serial.print(seperator);
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

// Callback function for sdfat

void dateTimeCB(uint16_t* date, uint16_t* time) {
  // User gets date and time from GPS or real-time
  // clock in real callback function

  // return date using FAT_DATE macro to format fields
  *date = FAT_DATE(year(), month(), day());

  // return time using FAT_TIME macro to format fields
  *time = FAT_TIME(hour(), minute(), second());
}
