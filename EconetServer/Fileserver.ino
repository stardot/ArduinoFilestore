void fsOperation (int bytes) {
  printTime();
  Serial.print (F(" "));

  Serial.print (rxBuff[3]);
  Serial.print (".");
  Serial.print (rxBuff[2]);
  Serial.print("  FS ");

  String param1 = "";
  String param1C = "";

  byte replyPort = rxBuff[4];
  byte fsCommand = rxBuff[5];
  byte urd = rxBuff[6];
  byte csd = rxBuff[7];
  byte lib = rxBuff[8];


  switch (fsCommand) {
    case 0:
      Serial.print(F("Execute OSCLI '"));

      for (int ptr1 = 9; ptr1 < (bytes - 1) ; ptr1++) {
        param1 += char(rxBuff[ptr1]);
      }
      Serial.print (param1);
      Serial.print ("' - ");

      param1C = param1;
      param1C.toUpperCase();

      if (param1C.startsWith("I AM")) fsLogin(replyPort, param1, bytes);
      else if (param1C.startsWith("LOGON")) fsLogin(replyPort, param1, bytes);
      else if (param1C.startsWith("DIR")) fsChangeDir(replyPort, param1, false, bytes);
      else if (param1C.startsWith("LIB")) fsChangeDir(replyPort, param1, true, bytes);
      else if (param1C.startsWith("DELETE")) fsDelete(replyPort, true);
      else if (param1C.startsWith("RENAME")) fsRename(replyPort, bytes);
      else if (param1C.startsWith("NEWUSER")) fsNewUser(replyPort, bytes);
      else if (param1C.startsWith("REMUSER")) fsRemUser(replyPort, bytes);
      else if (param1C.startsWith("PRIV")) fsSetUserPriv(replyPort, bytes);
      else if (param1C.startsWith("PASS")) fsChangePassword(replyPort, bytes);
      else if (param1C.startsWith("SDISC")) fsChangeDisc(replyPort, bytes);
      else if (param1C.startsWith("SETURD")) fsSetUserRoot(replyPort, bytes);
      else if (param1C.startsWith("FSSHUTDOWN")) fsShutdown(replyPort, false);
      else if (param1C.startsWith("FSREBOOT")) fsShutdown(replyPort, true);
      else if (param1C.startsWith("BYE")) fsBye(replyPort);  
      else if (param1C.startsWith("CDIR")) fsCdir(replyPort,true);   
      else if (param1C.startsWith("INFO")) fsInfo(replyPort,false);   
      else if (param1C.startsWith("I.")) fsInfo(replyPort,true); 
      else if (param1C.startsWith("FSSTATUS")) fsStatus(replyPort,bytes); 

      // Unimplemented commands are:
      // ACCESS
      // LOGOFF

      else {
        txBuff[0] = rxBuff[2];
        txBuff[1] = rxBuff[3];
        txBuff[2] = rxBuff[0];
        txBuff[3] = rxBuff[1];
        txBuff[4] = 8; // Command code 8 for unrecognised command.
        txBuff[5] = 0; // Unrecognised Command
        writeStringtoTX(param1, 6, (param1.length() + 1));
        if (txWithHandshake(6 + param1.length(), replyPort, 0x80)) Serial.println(F("Unrecognised command"));  else Serial.println(F("Unrecognised Command - Failed to send error"));
      }
      break;

    case 1:
      Serial.print(F("Save "));
      fsSave(replyPort);
      break;

    case 2:
      Serial.print(F("Load "));
      fsLoad(replyPort, false);
      break;

    case 3:
      Serial.print(F("Examine Directory "));
      fsExamine(replyPort);
      break;

    // 4 - Catalogue header

    case 5:
      Serial.print(F("Load as"));
      fsLoad(replyPort, true);
      break;

    case 6:
      Serial.print(F("Open "));
      fsOpen(replyPort);
      break;

    case 7:
      Serial.print(F("Close "));
      fsClose(replyPort);
      break;

    case 8:
      Serial.print(F("Get Byte "));
      fsGetByte(replyPort);
      break;

    case 9:
      Serial.print(F("Put Byte "));
      fsPutByte(replyPort);
      break;

    case 10:
      Serial.print(F("Get Bytes "));
      fsGetBytes(replyPort);
      break;

    case 11:
      Serial.print(F("Put Bytes "));
      fsPutBytes(replyPort);
      break;

    case 12:
      Serial.print(F("Get Random Access Args "));
      fsGetRandomAccessArgs(replyPort);
      break;

    case 13:
      Serial.print(F("Set Random Access Args "));
      fsSetRandomAccessArgs(replyPort);
      break;

    case 14:
      Serial.print(F("Read disk information "));
      fsReadDiskInfo(replyPort);
      break;

    case 15:
      Serial.print(F("Read Current Users "));
      fsReadCurrentUsers(replyPort);
      break;

    case 16:
      Serial.print(F("Read server time "));
      fsGetDateTime(replyPort);
      break;

    case 17:
      Serial.print(F("Read EOF status "));
      fsGetEndOfFile(replyPort);
      break;

    case 18:
      Serial.print(F("Read object information "));
      fsReadObjectInfo(replyPort);
      break;

    case 19:
      Serial.print(F("Set object information "));
      fsSetObjectInfo(replyPort);
      break;

    case 20:
      Serial.print(F("Delete "));
      fsDelete(replyPort, false);
      break;

    case 21:
      Serial.print(F("Read user environment "));
      fsReadUserEnv(replyPort);
      break;

    case 22:
      Serial.print(F("Set boot options "));
      fsSetBootOpts(replyPort);
      break;

    case 23:
      Serial.print(F("Log off "));
      fsBye(replyPort);
      break;

    // TODO: 24 - Read user information

    case 25:
      Serial.print(F("Get FS Version Info"));
      fsInfo(replyPort);
      break;

    case 26:
      Serial.print(F("Read disk free space "));
      fsReaddiskFree(replyPort);
      break;

    case 27:
      Serial.print(F("Create directory "));
      fsCdir(replyPort,false);
      break;

    // TODO: 28 - Set FS Clock

    case 29:
      Serial.print(F("Create file "));
      fsCreateFile(replyPort);
      break;

    case 30:
      Serial.print(F("Read user free space "));
      fsReadUserFree(replyPort);
      break;

    // TODO: 31 Set user free space

    case 32:
      Serial.print(F("Read Client ID "));
      fsReadClientID(replyPort);
      break;

    // TODO: 33 Read current user information (extended)

    // TODO: 34 Read user information (extended)

    // 35 Reserved command

    // TODO: 36 Manager Interface

    // 37 Reserved Command

    default:
      Serial.print (F("Unimplemented command number "));
      Serial.print (fsCommand);
      Serial.print (F(", packet dump: "));
      for (int ptr1 = 0; ptr1 < bytes ; ptr1++) {
        Serial.print (rxBuff[ptr1], HEX);
        Serial.print (F(" "));
      }
      Serial.print (F("= "));
      for (int ptr1 = 0; ptr1 < bytes ; ptr1++) {
        if (rxBuff[ptr1] < 32 || rxBuff[ptr1] > 126) {
          Serial.print (F("."));
        } else {
          Serial.write (rxBuff[ptr1]);
        }
      }
      Serial.println (F(" "));
      if (fsCommand < 100) fsError(0xFF, "FS command unknown", replyPort);
      break;
  }

  etherSelect(); // Reselect EtherNet card
  Serial.print (F("# "));
}

void fsLogin(byte txPort, String command, int bytesRX) {
  int usrHdl = -1;
  int ptr = 0;
  char* uPtr = NULL;
  char userBuf[23];
  FatFile profileHdl;

  sdSelect(); // Make sure SD card is selected on the SPI bus

  String user = getStringFromRX(14, 21);
  String pass = "";
  if (bytesRX > 13 + user.length()) pass = getStringFromRX(15 + user.length(), 22); // Only look for a password if submitted - TODO: need to take into account optional station number
  if (pass == "\"\"") pass = ""; // Fix blank password submitted

  user.toUpperCase();
  String userProfile = config_ProfileRoot;
  userProfile = userProfile + "/";
  userProfile = userProfile + user;

  userProfile.toCharArray(pathBuff1, DIRENTRYSIZE);

  Serial.print(F("User "));
  Serial.print(user);

  if (!sd.exists(pathBuff1)) {
    fsError(0xBC, "User not known", txPort);
    return;
  }

  if (!profileHdl.open(pathBuff1, O_READ)) {
    fsError(0xBC, "Unable to open user profile", txPort);
    return;
  }

  int profileSize = profileHdl.read(&workBuff, 255);

  if (profileSize < (PROFILE_URD + 2)) {
    profileHdl.close();
    fsError(0xBC, "User profile corrupted", txPort);
    return;
  }

  byte priv = workBuff[PROFILE_PRIV];
  byte bootOpts = workBuff[PROFILE_OPTS];
  byte enabled = workBuff[PROFILE_ENABLED];
  String profilePass = String((char *)&workBuff[PROFILE_PASSWORD]);
  String profileURD = String((char *)&workBuff[PROFILE_URD]);
  profileHdl.close();
  
  if (!enabled) {
    fsError(0xBD, "User account disabled", txPort);
    return;
  }

  pass.toUpperCase(); // Yes I know, but they are case insensitive!
  profilePass.toUpperCase(); // Yes I know, but they are case insensitive!

  Serial.print(F("  '"));
  Serial.print(profilePass);
  Serial.print(F("'  '"));
  Serial.print(pass);
  Serial.print(F("'  "));

  if (!pass.equals(profilePass)) {
    fsError(0xBB, "Incorrect password", txPort);
    return;
  }

  txBuff[0] = rxBuff[2];
  txBuff[1] = rxBuff[3];
  txBuff[2] = rxBuff[0];
  txBuff[3] = rxBuff[1];
  txBuff[4] = 5; // Command code 5
  txBuff[5] = 0; // Is successful
  txBuff[9] = bootOpts; // Boot options

  // Check if station has an existing user handle
  if (fsGetUserHdl(rxBuff[3], rxBuff[2]) != -1) {
    Serial.print(F(" - Station already logged in, clearing existing connection"));
    fsLogoff(rxBuff[3], rxBuff[2]);
  }

  // Now find a spare user hundle

  ptr = 0;
  do {
    if (stataddress[ptr] == 0) {
      usrHdl = ptr;
      break;
    }
    ptr++;
  } while (ptr < MAXUSERS);

  if (usrHdl == -1) {
    fsError(0xB8, "Too many users", txPort);
    return;
  };


  // Record station address, privilege and configure directories
  userOpenFiles[usrHdl] = 0;
  stataddress[usrHdl] = rxBuff[2];
  netaddress[usrHdl] = rxBuff[3];
  userPriv[usrHdl]= priv;
  setURD(usrHdl,profileURD);

  byte uCSDhdl = openFolder(usrHdl, profileURD);
  byte uLibhdl = openFolder(usrHdl, "/export/Library");

  if (uCSDhdl == 0 || uLibhdl == 0) {
    // Run out of file handles
    fsError(0xc0, "Too many open files ", txPort);
    //return;
  }


  txBuff[6] = 255; // new URD
  txBuff[7] = uCSDhdl; // new CSD
  txBuff[8] = uLibhdl; // new Lib



  // Write the user to the name buffer
  uPtr = userName;
  uPtr += (usrHdl * 22);
  user.toCharArray(userBuf, 21);
  strcpy(uPtr, userBuf);

  Serial.print(F(", User allocated handle "));
  Serial.print(usrHdl);
  Serial.print(F(", URD: "));
  Serial.print(getURD(usrHdl));
  Serial.print(F(", CSD: "));
  Serial.print(getFilePath(uCSDhdl));
  Serial.print(F(" ("));
  Serial.print(uCSDhdl);
  Serial.print(F("), Library: "));
  Serial.print(getFilePath(uLibhdl));
  Serial.print(F(" ("));
  Serial.print(uLibhdl);
  Serial.print(F(")"));

  if (txWithHandshake(10, txPort, 0x80)) Serial.println(F(" - Login successful")); else Serial.println(F(" - Failed to TX result."));

  return;
}

void fsChangeDir(byte txPort, String command, boolean library, int bytesRx) {
  SdFile file;
  sdSelect(); // Make sure SD card is selected on the SPI bus

  int usrHdl = fsGetUserHdl(rxBuff[3], rxBuff[2]);
  if (usrHdl == -1) {
    fsError(0xBF, F("Who are you?"), txPort);
    return;
  }

  byte urd = rxBuff[6];
  byte csd = rxBuff[7];
  byte lib = rxBuff[8];
  String workingDir = "";

  workingDir = getDirectoryPath(usrHdl, csd);

  String newDir = getStringFromRX(13, bytesRx - 13);
  Serial.print(" User: ");
  Serial.print(usrHdl);
  Serial.print(" CSD: ");
  Serial.print(csd);
  Serial.print(" Lib: ");
  Serial.print(lib);
  if (library) Serial.print(" Requested library path: "); else Serial.print(" Requested csd path: ");
  String fatPath = convertToFATPath(newDir, workingDir, txPort);
  Serial.print(fatPath);

  fatPath.toCharArray(pathBuff1, DIRENTRYSIZE);

  if (!sd.exists(pathBuff1)) {
    //Object not found - send error and leave
    Serial.print(" - ");
    fsError(0xD6, F("Not found"), txPort);
    return;
  }

  if (file.open(pathBuff1, O_READ)) {
    if (!file.isDir()) {
      file.close();
      fsError(0xBE, F("Object not a directory"), txPort);
      return;
    }
    file.close();
  }


  txBuff[0] = rxBuff[2];
  txBuff[1] = rxBuff[3];
  txBuff[2] = rxBuff[0];
  txBuff[3] = rxBuff[1];

  int dirHdl;

  if (library) {
    closeFolder(lib);
    dirHdl = openFolder(usrHdl, fatPath);

    if (dirHdl == 0) {
      // Run out of file handles
      fsError(0xc0, "Too many open files ", txPort);
      return;
    }

    txBuff[4] = 9; // library command code
    txBuff[5] = 0; // Is successful
    txBuff[6] = dirHdl; // new library handle
    Serial.print(F(" new lib handle "));
    Serial.print(dirHdl);
  } else {
    closeFolder(csd);
    dirHdl = openFolder(usrHdl, fatPath);

    if (dirHdl == 0) {
      // Run out of file handles
      fsError(0xc0, "Too many open files ", txPort);
      return;
    }

    txBuff[4] = 7; // directory command code
    txBuff[5] = 0; // Is successful
    txBuff[6] = dirHdl; // new CSD handle
    Serial.print(F(" new csd handle "));
    Serial.print(dirHdl);
  }

  if (txWithHandshake(7, txPort, 0x80)) Serial.println(F(", success .")); else Serial.println(F(", success !"));
}

void fsRename(byte txPort, int bytesRx) {
  SdFile file;
  sdSelect(); // Make sure SD card is selected on the SPI bus

  int usrHdl = fsGetUserHdl(rxBuff[3], rxBuff[2]);
  if (usrHdl == -1) {
    fsError(0xBF, F("Who are you?"), txPort);
    return;
  }

  String workingDir, objectName;


  workingDir = getDirectoryPath(usrHdl, rxBuff[7]);

  String oldName, newName;
  oldName = getStringFromRX(16, bytesRx - 16);
  newName = getStringFromRX(17 + oldName.length(), bytesRx - 17 - oldName.length());
  Serial.print(" User: ");
  Serial.print(usrHdl);
  Serial.print(" Old Filename: ");
  String fatPathO = convertToFATPath(oldName, workingDir, txPort);
  Serial.print(fatPathO);
  Serial.print(" New Filename: ");
  String fatPathN = convertToFATPath(newName, workingDir, txPort);
  Serial.print(fatPathN);

  if (!hasUserAccess(usrHdl,fatPathO,false)){
    // User does not have access to file
    fsError(0xBD,"Insufficient Access",txPort);
    return; 
  }  

  if (!hasUserAccess(usrHdl,fatPathN,false)){
    // User does not have access to file
    fsError(0xBD,"Insufficient Access at Destination",txPort);
    return; 
  }  

  fatPathO.toCharArray(pathBuff1, DIRENTRYSIZE);
  fatPathN.toCharArray(pathBuff2, DIRENTRYSIZE);

  if (!sd.exists(pathBuff1)) {
    //Object not found - send error and leave
    fsError(0xD6, "Not Found", txPort);
    return;
  }

  if (sd.exists(pathBuff2)) {
    //Destination object exists - send error and leave
    fsError(0xB0, "Bad rename - destination exists", txPort);
    return;
  }

  if (!sd.rename(pathBuff1, pathBuff2)) {
    //Rename has failed
    fsError(0xB0, "Bad rename", txPort);
    return;
  }

  //Need to move the metadata too

  fatPathO.replace(config_FSRoot, config_MetaRoot);
  fatPathO.toCharArray(pathBuff3, DIRENTRYSIZE);
  fatPathN.replace(config_FSRoot, config_MetaRoot);
  fatPathN.toCharArray(pathBuff4, DIRENTRYSIZE);

  if (sd.exists(pathBuff3)) {
    if (!sd.rename(pathBuff3, pathBuff4)) {
      fsError(0xFF, "Rename failed for file metadata", txPort);
      return;
    }
  }

  txBuff[0] = rxBuff[2];
  txBuff[1] = rxBuff[3];
  txBuff[2] = rxBuff[0];
  txBuff[3] = rxBuff[1];
  txBuff[4] = 0; // Command code
  txBuff[5] = 0; // Is successful

  if (txWithHandshake(6, txPort, 0x80)) Serial.println(F(".")); else Serial.println(F("!"));

}

void fsInfo(byte txPort, boolean abbreviated) {
  SdFile file;
  dir_t dirEntry;
  sdSelect(); // Make sure SD card is selected on the SPI bus

  int usrHdl = fsGetUserHdl(rxBuff[3], rxBuff[2]);
  if (usrHdl == -1) {
    fsError(0xBF, F("Who are you?"), txPort);
    return;
  }

  String workingDir;

  workingDir = getDirectoryPath(usrHdl, rxBuff[7]);

  String object;
 
  if (abbreviated){  object = getStringFromRX(11, DIRENTRYSIZE); } else { object = getStringFromRX(14, DIRENTRYSIZE); };

  Serial.print(" User: ");
  Serial.print(usrHdl);
  Serial.print(" Filename: ");
  String fatPath = convertToFATPath(object, workingDir, txPort);
  Serial.print(fatPath);

  if (!hasUserAccess(usrHdl,fatPath,true)){
    // User does not have write access to object
    fsError(0xBD,"Insufficient Access",txPort);
    return; 
  }  

  fatPath.toCharArray(pathBuff1, DIRENTRYSIZE);

  if (!sd.exists(pathBuff1)) {
    //Object not found - send error and leave
    fsError(0xD6, "Not Found", txPort);
    return;
  }
  //work out if it's a file or directory, as info is different

  boolean isDir = false;

  if (!file.open(pathBuff1, O_READ)) {
    // File failed to open for examination, can't continue
    fsError(0xFF, "Server internal error", txPort);
    return;
  }
  file.getName(pathBuff4, DIRENTRYSIZE);
  file.dirEntry(&dirEntry);

  unsigned int fileAddress = dirEntry.firstClusterLow;
  bool directory = file.isDir();
  unsigned int fileSize = file.fileSize();
  unsigned long execAddress = getExecAddressForObject(pathBuff1);
  unsigned long loadAddress = getLoadAddressForObject(pathBuff1);
  byte fileAttr = getAttributes(pathBuff1);
  int fsDate = getEconetDate(&dirEntry);
  String fcName=pathBuff4;
  fcName.replace(".","/");
  String text="";
  int padding=0;

  file.close();


  txBuff[0] = rxBuff[2];
  txBuff[1] = rxBuff[3];
  txBuff[2] = rxBuff[0];
  txBuff[3] = rxBuff[1];
  txBuff[4] = 4; // Command code
  txBuff[5] = 0; // Is successful

  int bufpos=6;

  //Send name first
  writeStringtoTX(fcName, bufpos, 10);    //TODO: If root send $
  bufpos += 10;
  txBuff[bufpos] = 32;
  bufpos++;

  // Load address
  text=String(loadAddress,HEX);

  while (text.length() < 8){
    // Pad address if short
    text="0"+text;
  }

  text.toUpperCase();

  writeStringtoTX(text, bufpos, 8);    //TODO: If root send $
  bufpos += 8;
  txBuff[bufpos] = 32;
  bufpos++;
  
  // Exec address
  text=String(execAddress,HEX);

  while (text.length() < 8){
    // Pad address if short
    text="0"+text;
  }

  text.toUpperCase();

  writeStringtoTX(text, bufpos, 8);  
  bufpos += 8;
  txBuff[bufpos] = 32;
  bufpos++;

  
  // Length
  txBuff[bufpos] = 32;
  bufpos++;
  txBuff[bufpos] = 32;
  bufpos++;

  text=String(fileSize,HEX);

  while (text.length() < 6){
    // Pad address if short
    text="0"+text;
  }

  text.toUpperCase();

  writeStringtoTX(text, bufpos, 6);   
  bufpos += 6;
  txBuff[bufpos] = 32;
  bufpos++;
  txBuff[bufpos] = 32;
  bufpos++;
  txBuff[bufpos] = 32;
  bufpos++;
  
  // 7 characters access attributes
  padding = 0;
  if ((fileAttr & 64) || (fileAttr & 16)) {
    txBuff[bufpos] = 76;
    bufpos ++;
  } else padding++; // Locked
  if (fileAttr & 32) {
    txBuff[bufpos] = 68;
    bufpos ++;
  } else padding++; // Directory
  if (fileAttr & 8) {
    txBuff[bufpos] = 87;
    bufpos ++;
  } else padding++; // Owner W
  if (fileAttr & 4) {
    txBuff[bufpos] = 82;
    bufpos ++;
  } else padding++; // Owner R
  txBuff[bufpos] = 47; // / seperator
  bufpos++;
  if (fileAttr & 2) {
    txBuff[bufpos] = 119;
    bufpos ++;
  } else padding++; // Public w
  if (fileAttr & 1) {
    txBuff[bufpos] = 114;
    bufpos ++;
  } else padding++; // Public r

  while (padding > 0) {
    txBuff[bufpos] = 32;
    bufpos++;
    padding--;
  }

  txBuff[bufpos] = 32;
  bufpos++;
  txBuff[bufpos] = 32;
  bufpos++;
  txBuff[bufpos] = 32;
  bufpos++;
  txBuff[bufpos] = 32;
  bufpos++;

  // Econet date
  text=econetDateToString(fsDate);
  writeStringtoTX(text, bufpos, 8); 
  bufpos += 8;
  txBuff[bufpos] = 32;
  bufpos++;


  // System Internal Numbner
  text=String(fileAddress,HEX);

  while (text.length() < 6){
    // Pad address if short
    text="0"+text;
  }

  text.toUpperCase();

  writeStringtoTX(text, bufpos, 6);   
  bufpos += 6;
 
  // Terminator
  txBuff[bufpos] = 0x0d;
  bufpos ++;
  txBuff[bufpos] = 0x80;
  bufpos ++;
    
  if (txWithHandshake(bufpos, txPort, 0x80)) Serial.println(F(".")); else Serial.println(F("!"));

}

void fsNewUser(byte txPort, int bytesRx) {
  FatFile profileHdl;
  sdSelect(); // Make sure SD card is selected on the SPI bus

  int usrHdl = fsGetUserHdl(rxBuff[3], rxBuff[2]);
  if (usrHdl == -1) {
    fsError(0xBF, F("Who are you?"), txPort);
    return;
  }

  String newUser = getStringFromRX(17, bytesRx - 17);
  newUser.toUpperCase();

  Serial.print(" User: ");
  Serial.print(usrHdl);
  Serial.print(" creating user: ");
  Serial.print(newUser);

  if (!isSystemUser(usrHdl)) {
    fsError(0xBA, F("Insufficient privilege"), txPort);
    return;
  }

  if (newUser.length() == 0) {
    fsError(0xCC, F("Bad Name"), txPort);
    return;
  }

  String userProfile = config_ProfileRoot;
  userProfile = userProfile + "/";
  userProfile = userProfile + newUser;
  userProfile.toCharArray(pathBuff1, DIRENTRYSIZE);

  if (sd.exists(pathBuff1)) {
    fsError(0xB1, "Already a user", txPort);
    return;
  }

  if (profileHdl.open(pathBuff1, (O_RDWR | O_CREAT))) {
    workBuff[PROFILE_PRIV] = 0x80; // Privilege is Normal
    workBuff[PROFILE_OPTS] = 0; // boot options
    workBuff[PROFILE_ENABLED] = 1; // Can login
    workBuff[PROFILE_PASSWORD] = 0; // No password
    // Allow 22 characters for the password to expand
    workBuff[PROFILE_QUOTA] = 0; // Quota not used
    workBuff[PROFILE_QUOTA + 1] = 0; // Quota not used
    workBuff[PROFILE_QUOTA + 2] = 0; // Quota not used
    workBuff[PROFILE_QUOTA + 3] = 0; // Quota not used
    workBuff[PROFILE_BALANCE] = 0; // Quota not used
    workBuff[PROFILE_BALANCE + 1] = 0; // Quota not used
    workBuff[PROFILE_BALANCE + 2] = 0; // Quota not used
    workBuff[PROFILE_BALANCE + 3] = 0; // Quota not used
    workBuff[PROFILE_URD] = 47; // User directory is / (ASCII 47)
    workBuff[PROFILE_URD + 1] = 0; // User directory termination

    // TODO: Error checking here
    profileHdl.write(&workBuff, PROFILE_URD + 2);
    profileHdl.close();

    txBuff[0] = rxBuff[2];
    txBuff[1] = rxBuff[3];
    txBuff[2] = rxBuff[0];
    txBuff[3] = rxBuff[1];
    txBuff[4] = 0; // Command code
    txBuff[5] = 0; // Is successful

    if (txWithHandshake(6, txPort, 0x80)) Serial.println(F(".")); else Serial.println(F("!"));

  } else {
    Serial.println("Failed to create profile " + userProfile);
  }
}

void fsRemUser(byte txPort, int bytesRx) {
  FatFile profileHdl;
  sdSelect(); // Make sure SD card is selected on the SPI bus

  int usrHdl = fsGetUserHdl(rxBuff[3], rxBuff[2]);
  if (usrHdl == -1) {
    fsError(0xBF, F("Who are you?"), txPort);
    return;
  }

  String remUser = getStringFromRX(17, bytesRx - 17);

  Serial.print(" User: ");
  Serial.print(usrHdl);
  Serial.print(" deleting user: ");
  Serial.print(remUser);

  if (!isSystemUser(usrHdl)) {
    fsError(0xBA, F("Insufficient privilege"), txPort);
    return;
  }
  remUser.toUpperCase();

  if (remUser.length() == 0) {
    fsError(0xCC, F("Bad Name"), txPort);
    return;
  }

  String userProfile = config_ProfileRoot;
  userProfile = userProfile + "/";
  userProfile = userProfile + remUser;

  userProfile.toCharArray(pathBuff1, DIRENTRYSIZE);

  if (!sd.exists(pathBuff1)) {
    fsError(0xBC, "User not known", txPort);
    return;
  }

  // TODO: Log off user if still logged on

  if (sd.remove(pathBuff1)) {
    // Profile deleted successfully
    txBuff[0] = rxBuff[2];
    txBuff[1] = rxBuff[3];
    txBuff[2] = rxBuff[0];
    txBuff[3] = rxBuff[1];
    txBuff[4] = 0; // Command code
    txBuff[5] = 0; // Is successful

    if (txWithHandshake(6, txPort, 0x80)) Serial.println(F(".")); else Serial.println(F("!"));

  } else {
    Serial.println("Failed to delete profile " + userProfile);
  }

}

void fsSetUserPriv(byte txPort, int bytesRx) {
  FatFile profileHdl;
  sdSelect(); // Make sure SD card is selected on the SPI bus

  int usrHdl = fsGetUserHdl(rxBuff[3], rxBuff[2]);
  if (usrHdl == -1) {
    fsError(0xBF, F("Who are you?"), txPort);
    return;
  }

  String privUser = getStringFromRX(14, bytesRx - 14);
  String newPriv;
  if (bytesRx > (14 + privUser.length())) newPriv = getStringFromRX(15 + privUser.length(), bytesRx - 14); else newPriv = "N";

  privUser.toUpperCase();

  Serial.print(" User: ");
  Serial.print(usrHdl);
  Serial.print(" updating user: ");
  Serial.print(privUser);
  Serial.print(" to priv: ");
  Serial.print(newPriv);

  if (!isSystemUser(usrHdl)) {
    fsError(0xBA, F("Insufficient privilege"), txPort);
    return;
  }

  if (privUser.length() == 0) {
    fsError(0xCC, F("Bad Name"), txPort);
    return;
  }

  String userProfile = config_ProfileRoot;
  userProfile = userProfile + "/";
  userProfile = userProfile + privUser;
  userProfile.toCharArray(pathBuff1, DIRENTRYSIZE);

  if (!sd.exists(pathBuff1)) {
    fsError(0xBC, "User not known", txPort);
    return;
  }

  if (!profileHdl.open(pathBuff1, O_WRITE)) {
    Serial.println("Failed to open profile " + userProfile);
    return;
  }

  if (newPriv == "L") profileHdl.write((uint8_t)0x00); else if (newPriv == "F") profileHdl.write((uint8_t)0x40); else if (newPriv == "N") profileHdl.write((uint8_t)0x80); else if (newPriv == "S") profileHdl.write((uint8_t)0xFF); else {
    profileHdl.close();
    fsError(0x8C, F("Bad privilege letter"), txPort);
  }

  profileHdl.close();

  txBuff[0] = rxBuff[2];
  txBuff[1] = rxBuff[3];
  txBuff[2] = rxBuff[0];
  txBuff[3] = rxBuff[1];
  txBuff[4] = 0; // Command code
  txBuff[5] = 0; // Is successful

  if (txWithHandshake(6, txPort, 0x80)) Serial.println(F(".")); else Serial.println(F("!"));


}

void fsChangePassword(byte txPort, int bytesRx) {
  SdFile file;
  FatFile profileHdl;
  sdSelect(); // Make sure SD card is selected on the SPI bus

  int usrHdl = fsGetUserHdl(rxBuff[3], rxBuff[2]);
  if (usrHdl == -1) {
    fsError(0xBF, F("Who are you?"), txPort);
    return;
  }

  // Stop Fixed and Locked users from changing their password
  if (userPriv[usrHdl]<0x41) {
    fsError(0xBA, F("Insufficient privilege"), txPort);
    return;
  }

  String oldPass, newPass, userName;
  oldPass = getStringFromRX(14, bytesRx - 14);
  if (oldPass == "") oldPass = getStringFromRX(15, bytesRx - 15); // The command has an additional space from RiscOS
  newPass = getStringFromRX(15 + oldPass.length(), bytesRx - 15 - oldPass.length());
  if (newPass == "") newPass = getStringFromRX(16 + oldPass.length(), bytesRx - 16 - oldPass.length()); // The command has an additional space from RiscOS
  userName = getUsername(usrHdl);

  if (oldPass == "\"\"") oldPass = "";
  if (newPass == "\"\"") newPass = "";

  userName.toUpperCase(); // Everything is case insensitive for compatibility
  oldPass.toUpperCase();
  newPass.toUpperCase();

  Serial.print(" User: ");
  Serial.print(userName);
  Serial.print(" Changing Old Pass: '");
  Serial.print(oldPass);
  Serial.print("' To New Pass: '");
  Serial.print(newPass);
  Serial.print("'");

  String userProfile = config_ProfileRoot;
  userProfile = userProfile + "/";
  userProfile = userProfile + userName;

  userProfile.toCharArray(pathBuff1, DIRENTRYSIZE);


  if (!profileHdl.open(pathBuff1, O_RDWR)) {
    fsError(0xFF, "Unable to open user profile", txPort);
    return;
  }

  int profileSize = profileHdl.read(&workBuff, 255);

  byte userPriv = workBuff[0];

  // Only allow Normal (or above) users to change password
  if (userPriv < 0x80) {
    profileHdl.close();
    fsError(0xBA, F("Insufficient privilege"), txPort);
    return;
  }

  String profilePass = String((char *)&workBuff[PROFILE_PASSWORD]);

  if (!oldPass.equals(profilePass)) {
    profileHdl.close();
    fsError(0xBB, "Incorrect password", txPort);
    return;
  }

  if (!(newPass.length() > 5 && newPass.length() < 23)) {
    profileHdl.close();
    fsError(0xB9, "Password must be between 6 and 22 characters", txPort);
    return;
  }

  newPass.toCharArray((char *)&workBuff[PROFILE_PASSWORD], 23);
  profileHdl.seekSet(0);
  if (!profileHdl.write(&workBuff, profileSize)) {
    profileHdl.close();
    fsError(0xFF, "Unable to update user profile", txPort);
    return;
  }

  profileHdl.close();

  txBuff[0] = rxBuff[2];
  txBuff[1] = rxBuff[3];
  txBuff[2] = rxBuff[0];
  txBuff[3] = rxBuff[1];
  txBuff[4] = 0; // Command code
  txBuff[5] = 0; // Is successful

  if (txWithHandshake(6, txPort, 0x80)) Serial.println(F(".")); else Serial.println(F("!"));
}

void fsChangeDisc(byte txPort, int bytesRx) {
  
  int usrHdl = fsGetUserHdl(rxBuff[3], rxBuff[2]);
  if (usrHdl == -1) {
    fsError(0xBF, F("Who are you?"), txPort);
    return;
  }

  String availDisc=config_FSName;
  String newDisc = getStringFromRX(15, bytesRx - 15);

  Serial.print(" User: "+(String)usrHdl+" requesting "+newDisc);

  availDisc.toUpperCase();
  newDisc.toUpperCase();

  //TODO: This will need to change once additional volume support added

  if(newDisc.equals(availDisc)){
    txBuff[0] = rxBuff[2];
    txBuff[1] = rxBuff[3];
    txBuff[2] = rxBuff[0];
    txBuff[3] = rxBuff[1];
    txBuff[4] = 0; // Command code
    txBuff[5] = 0; // Is successful

    if (txWithHandshake(6, txPort, 0x80)) Serial.println(F(".")); else Serial.println(F("!"));

  } else {
    fsError(0xCD, F("Bad Drive"), txPort);
  }
}

void fsSetUserRoot(byte txPort, int bytesRx) {
  FatFile profileHdl;
  sdSelect(); // Make sure SD card is selected on the SPI bus

  int usrHdl = fsGetUserHdl(rxBuff[3], rxBuff[2]);
  if (usrHdl == -1) {
    fsError(0xBF, F("Who are you?"), txPort);
    return;
  }

  String dirUser = getStringFromRX(16, bytesRx - 14);
  String newURD = getStringFromRX(17 + dirUser.length(), bytesRx - 17);
  dirUser.toUpperCase();

  Serial.print(" User: ");
  Serial.print(usrHdl);
  Serial.print(" updating user: ");
  Serial.print(dirUser);
  Serial.print(" to dir: ");
  Serial.print(newURD);

  if (!isSystemUser(usrHdl)) {
    fsError(0xBA, F("Insufficient privilege"), txPort);
    return;
  }

  if (dirUser.length() == 0) {
    fsError(0xCC, F("Bad username"), txPort);
    return;
  }

  String fatPath = convertToFATPath(newURD, config_FSRoot, txPort);

  Serial.print(" (path: ");
  Serial.print(fatPath);
  Serial.print(") ");

  dirUser.toUpperCase();
  String userProfile = config_ProfileRoot;
  userProfile = userProfile + "/";
  userProfile = userProfile + dirUser;
  userProfile.toCharArray(pathBuff1, DIRENTRYSIZE);

  if (!sd.exists(pathBuff1)) {
    fsError(0xBC, "User not known", txPort);
    return;
  }

  if (!profileHdl.open(pathBuff1, O_RDWR)) {
    Serial.println("Failed to open profile " + userProfile);
    return;
  }


  int profileSize = profileHdl.read(&workBuff, 255);

  if (profileSize < (PROFILE_URD + 2)) {
    profileHdl.close();
    fsError(0xBC, "User profile corrupted", txPort);
    return;
  }

  fatPath.toCharArray((char *)&workBuff[PROFILE_URD], DIRENTRYSIZE);

  workBuff[PROFILE_URD + fatPath.length()] = 0;
  profileHdl.seekSet(0);

  if (!profileHdl.write(&workBuff, PROFILE_URD + 1 + fatPath.length())) {
    profileHdl.close();
    fsError(0xFF, "Unable to update user profile", txPort);
    return;
  }

  profileHdl.close();

  txBuff[0] = rxBuff[2];
  txBuff[1] = rxBuff[3];
  txBuff[2] = rxBuff[0];
  txBuff[3] = rxBuff[1];
  txBuff[4] = 0; // Command code
  txBuff[5] = 0; // Is successful

  if (txWithHandshake(6, txPort, 0x80)) Serial.println(F(".")); else Serial.println(F("!"));

}

void fsShutdown(byte txPort, bool reboot) {

  sdSelect(); // Make sure SD card is selected on the SPI bus

  // Get user handle
  int usrHdl = fsGetUserHdl(rxBuff[3], rxBuff[2]);
  if (usrHdl == -1) {
    fsError(0xBF, F("Who are you?"), txPort);
    return;
  }

  Serial.print(" User: " + (String)usrHdl + " requesting FS shutdown ");
  if (reboot) Serial.print(" and restart ");

  //Check they have system privs
  if (!isSystemUser(usrHdl)) {
    fsError(0xBA, F("Insufficient privilege"), txPort);
    return;
  }

  Serial.print("\nLogging off users ");

  int ptr = 0;
  do {
    if (stataddress[ptr] != 0) {
      Serial.print((String)ptr + " ");
      fsLogoff(netaddress[ptr], stataddress[ptr]);
    }
    ptr++;
  } while (ptr < MAXUSERS);

  String message;

  if (reboot) {
    message = "All users logged off, server rebooting\r";
  } else {
    message = "All users logged off, server halted\r";
  }

  Serial.print("\n------------------------------------------------\n\n");

  fsError(0xFF, message, txPort); // Send shutdown successful error

  Serial.print("\n\n");

  delay (5000); //Wait 5 seconds

  //Self reset definitions
#define SYSRESETREQ    (1<<2)
#define VECTKEY        (0x05fa0000UL)
#define VECTKEY_MASK   (0x0000ffffUL)
#define AIRCR          (*(uint32_t*)0xe000ed0cUL) // fixed arch-defined address
#define REQUEST_EXTERNAL_RESET (AIRCR=(AIRCR&VECTKEY_MASK)|VECTKEY|SYSRESETREQ);

  if (reboot) REQUEST_EXTERNAL_RESET; // Jump to hardware reset vector

  // if still here, put CPU to sleep until manually reset
  pmc_enable_sleepmode(0);

}

void fsStatus(byte txPort, int bytesRx) {
  FatFile profileHdl;
  sdSelect(); // Make sure SD card is selected on the SPI bus

  int usrHdl = fsGetUserHdl(rxBuff[3], rxBuff[2]);
  if (usrHdl == -1) {
    fsError(0xBF, F("Who are you?"), txPort);
    return;
  }

  String key = getStringFromRX(18, bytesRx - 18);

  Serial.print(" User: ");
  Serial.print(usrHdl);
  Serial.print(" Status: ");
  Serial.print(key);

  if (!isSystemUser(usrHdl)) {
    fsError(0xBA, F("Insufficient privilege"), txPort);
    return;
  }
  key.toUpperCase();

  if (key.length() == 0) {
    fsError(0xCC, F("No key supplied"), txPort);
    return;
  }

  String store = config_confRoot;
  store = store + "/" + key;
  //store = store + key;

  store.toCharArray(pathBuff1, DIRENTRYSIZE);

  if (!sd.exists(pathBuff1)) {
    fsError(0xBC, "Key not known", txPort);
    return;
  }
  String value=readConfigValue(key);
  Serial.print(" Result: ");
  Serial.print(value);

  value+="\r";

  txBuff[0] = rxBuff[2];
  txBuff[1] = rxBuff[3];
  txBuff[2] = rxBuff[0];
  txBuff[3] = rxBuff[1];
  txBuff[4] = 0; // Command code
  txBuff[5] = 0x80; // Return code (undefined - causes client to print string)
  writeStringtoTX(value, 6, value.length());

  if (txWithHandshake(6+value.length(), txPort, 0x80)) Serial.println(F(".")); else Serial.println(F("!"));

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function 1 - Save
/////////////////////////////////////////////////////////////////////

void fsSave(int txPort) {
  sdSelect(); // Make sure SD card is selected on the SPI bus
  FatFile file;

  int usrHdl = fsGetUserHdl(rxBuff[3], rxBuff[2]);
  if (usrHdl == -1) {
    fsError(0xBF, F("Who are you?"), txPort);
    return;
  }

  String workingDir;

  byte ackPort = rxBuff[6];

  workingDir = getDirectoryPath(usrHdl, rxBuff[7]);

  byte attribs=12; //Default owner read and write access to file

  unsigned long loadaddr, execaddr, fsize;

  loadaddr = rxBuff[12];
  loadaddr = loadaddr << 8;
  loadaddr += rxBuff[11];
  loadaddr = loadaddr << 8;
  loadaddr += rxBuff[10];
  loadaddr = loadaddr << 8;
  loadaddr += rxBuff[9];

  execaddr = rxBuff[16];
  execaddr = execaddr << 8;
  execaddr += rxBuff[15];
  execaddr = execaddr << 8;
  execaddr += rxBuff[14];
  execaddr = execaddr << 8;
  execaddr += rxBuff[13];

  fsize = rxBuff[19];
  fsize = fsize << 8;
  fsize += rxBuff[18];
  fsize = fsize << 8;
  fsize += rxBuff[17];


  String objectName = getStringFromRX(20, 128);
  String fatPath = convertToFATPath(objectName, workingDir, txPort);

  Serial.print(F(" - user "));
  Serial.print(usrHdl);
  Serial.print(F(" object "));
  Serial.print(objectName);
  Serial.print(F(" ack port "));
  Serial.print(ackPort, HEX);
  Serial.print(F(" reply port "));
  Serial.print(txPort, HEX);
  Serial.print(F(" load address "));
  Serial.print(loadaddr, HEX);
  Serial.print(F(" exec address "));
  Serial.print(execaddr, HEX);
  Serial.print(F(" size "));
  Serial.print(fsize);
  Serial.print(F(" working dir "));
  Serial.print(workingDir);
  Serial.print(F(" disk path "));
  Serial.print(fatPath);
  Serial.print(F(" "));

  if (isObjectLocked(fatPath)){
    // Object locked 
    fsError(0xBD,"Access violation",txPort);
    return; 
  }  

  fatPath.toCharArray(pathBuff1, 128);

  if (file.open(pathBuff1, O_READ)) {
    if (file.isDir()) {
      file.close();
      fsError(0xBE, F("Object is a directory "), txPort);
      return;
    }
    file.close();
  }

  if(sd.exists(pathBuff1)){
    // Store attributes for later
    attribs=getAttributes(pathBuff1);
    
    if (!hasUserAccess(usrHdl,fatPath,false)){  
      // User does not have write access to existing file
      fsError(0xBD,"Insufficient Access",txPort);
      return;
    } 
  } else { 
    if (!fatPath.startsWith(getURD(usrHdl),0) && !isSystemUser(usrHdl)){
      // File does not exist, and user does not own parent directory
      fsError(0xBD,"Insufficient Access",txPort);
      return; 
    } 
  }

  int fHdl = 1;
  while (fHandleActive[fHdl] && fHdl < MAXFILES) {
    fHdl++;
  };
  if (fHdl == MAXFILES) {
    // Run out of file handles
    fsError(0xc0, "Too many open files ", txPort);
    return;
  }

  if (userOpenFiles[usrHdl] >= MAXUSERFILES) {
    // Run out of file handles
    fsError(0xc0, "Too many open files ", txPort);
    return;
  }

  //TODO: Check if file is already open. Refuse write request if any other handles open, refuse read if open for write.

  // Open for writing, create if necessary
  if (fHandle[fHdl].open(pathBuff1, (O_RDWR | O_CREAT))) {
    fHandleActive[fHdl] = true;
    fHandleUser[fHdl] = usrHdl;
  } else {
    // Open failed for some reason
    fsError(0xFF, "Server error - Unable to open file for writing ", txPort);
    return;
  }

  byte dataPort = fHdl + 129; // Makes life easier to find the filehandle
  int maxTXSize = BUFFSIZE - 6;

  // Set up the tables
  fSequence[fHdl] = 2;
  setfHandlePath(fHdl, fatPath);
  dataReplyPort[fHdl] = txPort;
  dataAckPort[fHdl] = ackPort;

  expectingBytes[fHdl] = fsize;
  gotBytes[fHdl] = 0;
  isSave[fHdl] = true;
  fHandleIsFolder[fHdl] = false;
  userOpenFiles[usrHdl]++;

  //Now create the meta entry
  fatPath.replace(config_FSRoot, config_MetaRoot);
  fatPath.toCharArray(pathBuff2, DIRENTRYSIZE);

  if (!file.open(pathBuff2, (O_RDWR | O_CREAT))) {
    // File create failed
    // TODO better error response
    fsError(0xBD, "Metadata file creation failed, because reasons", txPort);
    return;
  }

  file.write(&rxBuff[9], 8); //Load and Exec address
  file.write(attribs); 
  file.close();


  if (fsize > 0) {
    // Send an OK so station can start the bulk TX process
    txBuff[0] = rxBuff[2];
    txBuff[1] = rxBuff[3];
    txBuff[2] = config_Station;
    txBuff[3] = config_Net;
    txBuff[4] = 0x00; // Command code 0
    txBuff[5] = 0x00; // Result OK
    txBuff[6] = dataPort;
    txBuff[7] = maxTXSize;
    txBuff[8] = maxTXSize << 8;
    if (txWithHandshake(9, txPort, rxControlByte)) Serial.println(F(" "));
    else {
      Serial.println(F("- setup TX Failed"));
      return;
    };
  } else {
    // Zero size file saved - send ack
    // Send final ack and close off the filehandles.
    dir_t dirEntry;
    fHandle[fHdl].dirEntry(&dirEntry);
    int fsDate = getEconetDate(&dirEntry);

    txBuff[0] = rxBuff[2];
    txBuff[1] = rxBuff[3];
    txBuff[2] = config_Station;
    txBuff[3] = config_Net;
    txBuff[4] = 0x00; // Command code
    txBuff[5] = 0x00; // result = OK
    txBuff[6] = 15; // TODO: correct attributes (hardcoded to allow all access, not locked)
    txBuff[7] = fsDate;
    txBuff[8] = fsDate << 8 ;
    if (txWithHandshake(9, txPort, rxControlByte)) Serial.println(F("")); else Serial.println(F("- Final Ack Failed"));

    // Close the filehandle and mark inactive in the table
    fHandle[fHdl].truncate(expectingBytes[fHdl]); // Ensure file is correct size - might be overwriting existing file
    fHandle[fHdl].close();
    userOpenFiles[usrHdl]--;
    fHandleActive[fHdl] = false;
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Functions 2 & 5 - Load and Load As
/////////////////////////////////////////////////////////////////////

void fsLoad(int txPort, boolean loadAs) {
  sdSelect(); // Make sure SD card is selected on the SPI bus

  SdFile file;
  dir_t dirEntry;


  int usrHdl = fsGetUserHdl(rxBuff[3], rxBuff[2]);
  bool foundFile=false;
  
  if (usrHdl == -1) {
    fsError(0xBF, F("Who are you?"), txPort);
    return;
  }

  byte dataTXPort = rxBuff[6];
  String workingDir, libraryPath;

  workingDir = getDirectoryPath(usrHdl, rxBuff[7]);
  libraryPath = getDirectoryPath(usrHdl, rxBuff[8]);

  String objectName = getStringFromRX(9, 128);
  String fatPath = convertToFATPath(objectName, workingDir, txPort);
  String libfatPath = convertToFATPath(objectName, libraryPath, txPort);

  Serial.print(F(" - user "));
  Serial.print(usrHdl);
  Serial.print(F(" object "));
  Serial.print(objectName);
  Serial.print(F(" txport "));
  Serial.print(dataTXPort);
  Serial.print(F(" working dir "));
  Serial.print(workingDir);
  Serial.print(F(" object path "));
  Serial.print(fatPath);
  Serial.print(F(" loadas "));
  Serial.print(loadAs);
  Serial.print(F(" library object path "));
  Serial.print(libfatPath);

  
  // Locate and open the file
  fatPath.toCharArray(pathBuff1, 128);

  if (!sd.exists(pathBuff1)){
    if (!loadAs){
      //Object not found, and not loadas - send error and leave
      Serial.print(" - ");
      fsError(0xFE, F("Bad command"), txPort);
      return;
    }    
  } else {
    // CSD object exists
     if (hasUserAccess(usrHdl,fatPath,true)){
      // User has access, try opening it
      if (file.open(pathBuff1, O_READ)) {
        foundFile=true;
        if (file.isDir()) {
          file.close();
          foundFile=false;
          if (!loadAs){
            fsError(0xFE, F("Bad command"), txPort);
            return;
          }
        }
      } else {
        // file open failed for some unknown reason
        fsError(0xFF, F("Unknown error opening object"), txPort);
        return;
      }
    } else {
      // User does not have read access to file
      fsError(0xBD,"Insufficient Access",txPort);
      return; 
    }   
  }

  // If we are here than either the file is open in the CSD (foundFile true), or it's missing or a directory
  // but as it's a loadas call the library needs to be searched too.

  if (!foundFile){
    // Need to search the library
    libfatPath.toCharArray(pathBuff4, 128);

    if (!sd.exists(pathBuff4)) {
        //Object not found in library either - send error
        Serial.print(" - ");
        fsError(0xFE, "Bad command", txPort);
        return;

    } else {
      //Object found in library

      if (!hasUserAccess(usrHdl,libfatPath,true)){
        // User does not have read access to Object, send error
        fsError(0xBD,"Insufficient Access",txPort);
        return; 
      } 
      // User has access, try opening it
      if (file.open(pathBuff4, O_READ)) {
        if (file.isDir()) {
          file.close();
          fsError(0xFE, F("Bad command"), txPort); // It's a directory, send error
          return;
        }
      } else {
        // file open failed for some unknown reason, send error
        fsError(0xFF, F("Unknown error opening object"), txPort);
        return;
      }
    } // end of is library object found   
  } // end of is file open

  // If we've got this far, file should be open

  long reqPtr = 0;
  long bytesRead = 0;
  boolean eofReached = false;
  boolean fileError = false;

  file.dirEntry(&dirEntry);

  bool directory = file.isDir();
  unsigned int fileSize = file.fileSize();
  unsigned long execAddress = getExecAddressForObject(pathBuff1);
  unsigned long loadAddress = getLoadAddressForObject(pathBuff1);
  byte fileAttr = getAttributes(pathBuff1);

  if (!foundFile){
    // Library object was opened instead
    execAddress = getExecAddressForObject(pathBuff4);
    loadAddress = getLoadAddressForObject(pathBuff4);
    fileAttr = getAttributes(pathBuff4);
  }
  
  int fsDate = getEconetDate(&dirEntry);

  Serial.print(F(" load addr "));
  Serial.print(loadAddress, HEX);
  Serial.print(F(" exec addr "));
  Serial.print(execAddress, HEX);
  Serial.print(F(" size "));
  Serial.print(fileSize);
  Serial.print(F(" "));


  // Send an OK with file information so we can begin TXing the requested bytes
  txBuff[0] = rxBuff[2];
  txBuff[1] = rxBuff[3];
  txBuff[2] = config_Station;
  txBuff[3] = config_Net;
  txBuff[4] = 0x00;
  txBuff[5] = 0x00;

  // Load and Execution addresses
  txBuff[6] = loadAddress;
  txBuff[7] = loadAddress >> 8;
  txBuff[8] = loadAddress >> 16;
  txBuff[9] = loadAddress >> 24;

  txBuff[10] = execAddress;
  txBuff[11] = execAddress >> 8;
  txBuff[12] = execAddress >> 16;
  txBuff[13] = execAddress >> 24;

  //File size
  txBuff[14] = fileSize;
  txBuff[15] = fileSize >> 8;
  txBuff[16] = fileSize >> 16;

  // File attributes
  txBuff[17] = fileAttr;

  //config_NetFS File date
  txBuff[18] = fsDate;
  txBuff[19] = fsDate >> 8;

  // TODO: Ignoring leaf name here, look into wildcard resolving etc.

  if (txWithHandshake(20, txPort, rxControlByte)) Serial.print(F("- Initial TX OK"));
  else {
    Serial.println(F("- Initial response TX Failed"));
    return;
  };


  byte rxControlByte = 0x80;

  while (reqPtr < fileSize) {
    int toFetch = (fileSize - reqPtr);
    if (toFetch > (BUFFSIZE - 8)) toFetch = BUFFSIZE - 8; // Allow a few bytes for addressing

    int result = file.read(&txBuff[4], toFetch);

    reqPtr += toFetch;
    if (result == -1) fileError = true; else if (result < toFetch) eofReached = true;

    bytesRead += result;

    if (!txWithHandshake(toFetch + 4, dataTXPort, rxControlByte)) {
      Serial.println(F("- TX failed  "));
      return;
    }

    // rxControlByte=!rxControlByte;
  }

  // Have TXed right number of bytes, do closing message
  if (!fileError) {
    txBuff[0] = rxBuff[2];
    txBuff[1] = rxBuff[3];
    txBuff[2] = config_Station;
    txBuff[3] = config_Net;
    txBuff[4] = 0x00;
    txBuff[5] = 0x00;

    //Serial.print(F("About to tx complete message "));

    if (txWithHandshake(6, txPort, rxControlByte)) Serial.println(F("- transfer OK")); else Serial.println(F("- transfer Failed"));

    file.close();

  } else {

    // File error returned by read
    fsError(0xFF, "Read Failed", txPort);

  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function 3 - Examine
/////////////////////////////////////////////////////////////////////

void fsExamine(int txPort) {
  SdFile file;
  dir_t dirEntry;
  sdSelect(); // Make sure SD card is selected on the SPI bus

  int usrHdl = fsGetUserHdl(rxBuff[3], rxBuff[2]);
  if (usrHdl == -1) {
    fsError(0xBF, F("Who are you?"), txPort);
    return;
  }

  String workingDir;

  workingDir = getDirectoryPath(usrHdl, rxBuff[7]);

  byte format = rxBuff[9];
  byte firstItem = rxBuff[10];
  byte totalItems = rxBuff[11];
  byte bufpos, dirCount = 0;


  String pathName = getStringFromRX(12, 128);
  Serial.print(F(" - user "));
  Serial.print(usrHdl);
  Serial.print(F(" dir handle "));
  Serial.print(rxBuff[7]);
  Serial.print(F(" working dir "));
  Serial.print(workingDir);
  Serial.print(F(", format "));
  Serial.print(format, HEX);
  Serial.print(F(", requesting "));
  Serial.print(totalItems, HEX);
  Serial.print(F(" items from "));
  Serial.print(firstItem, HEX);
  Serial.print(F(" onwards from folder "));
  Serial.print(pathName);
  Serial.print(F(" disk path "));
  String fatPath = convertToFATPath(pathName, workingDir, txPort);
  Serial.print(fatPath);

  // No access control required, directories are always readable

  fatPath.toCharArray(pathBuff1, DIRENTRYSIZE);

  if (!sd.exists(pathBuff1)) {
    //Object not found - send error and leave
    Serial.print(" - ");
    fsError(0xD6, F("Not found"), txPort);
    return;
  }

  if (file.open(pathBuff1, O_READ)) {
    if (!file.isDir()) {
      file.close();
      fsError(0xBE, F("Object not a directory"), txPort);
      return;
    }
    file.close();
  }

  //char nameBuffer[11];

  txBuff[0] = rxBuff[2];
  txBuff[1] = rxBuff[3];
  txBuff[2] = config_Station;
  txBuff[3] = config_Net;
  txBuff[4] = 0x00;
  txBuff[5] = 0x00;
  txBuff[6] = 0x00; // Populate with number of items returned later
  txBuff[7] = 0x00; // Populate with directory size later
  bufpos = 8;

  sd.chdir(pathBuff1);
  //sd.vwd()->seekSet(firstItem);

  int thisItem = 0;
  while (firstItem != thisItem) {
    file.openNext(sd.vwd(), O_READ);
    file.close();
    thisItem++;
  }

  String pathname;

  while ((file.openNext(sd.vwd(), O_READ)) && dirCount < 9) {

    // We have a directory item open successfully
    // Get the contents we need

    file.getName(pathBuff4, DIRENTRYSIZE);
    file.dirEntry(&dirEntry);

    pathName = fatPath + pathBuff4;
    pathName.toCharArray(pathBuff2, DIRENTRYSIZE);

    unsigned int fileAddress = dirEntry.firstClusterLow;
    bool directory = file.isDir();
    unsigned int fileSize = file.fileSize();
    unsigned long execAddress = getExecAddressForObject(pathBuff2);
    unsigned long loadAddress = getLoadAddressForObject(pathBuff2);
    byte fileAttr = getAttributes(pathBuff2);
    int fsDate = getEconetDate(&dirEntry);
    String fcName=pathBuff4;
    fcName.replace(".","/");
    String text="";
    int padding=0;
    
    switch (format) {
      case 0:
        // Machine readable format requested

        //Send name first
        writeStringtoTX(fcName, bufpos, 10);    //TODO: If root send $
        bufpos += 10;

        // Load and Execution addresses
        txBuff[bufpos] = loadAddress;
        bufpos++;
        txBuff[bufpos] = loadAddress >> 8;
        bufpos++;
        txBuff[bufpos] = loadAddress >> 16;
        bufpos++;
        txBuff[bufpos] = loadAddress >> 24;
        bufpos++;

        txBuff[bufpos] = execAddress;
        bufpos++;
        txBuff[bufpos] = execAddress >> 8;
        bufpos++;
        txBuff[bufpos] = execAddress >> 16;
        bufpos++;
        txBuff[bufpos] = execAddress >> 24;
        bufpos++;


        // File attributes
        txBuff[bufpos] = fileAttr;
        bufpos++;

        //File date
        txBuff[bufpos] = fsDate;
        bufpos++;
        txBuff[bufpos] = fsDate >> 8;
        bufpos++;

        //System internal name (SIN) - Fudged based on the cluster address of the file
        txBuff[bufpos] = fileAddress;
        bufpos++;
        txBuff[bufpos] = fileAddress >> 8;
        bufpos++;
        txBuff[bufpos] = fileAddress >> 16;
        bufpos++;

        //File size

        txBuff[bufpos] = fileSize;
        bufpos++;
        txBuff[bufpos] = fileSize >> 8;
        bufpos++;
        txBuff[bufpos] = fileSize >> 16;
        bufpos++;
        break;
        
      case 1:
        // All information in human readable format

        //Send name first
        writeStringtoTX(fcName, bufpos, 10);    //TODO: If root send $
        bufpos += 10;
        txBuff[bufpos] = 32;
        bufpos++;

        // Load address
        text=String(loadAddress,HEX);
      
        while (text.length() < 8){
          // Pad address if short
          text="0"+text;
        }
      
        text.toUpperCase();
      
        writeStringtoTX(text, bufpos, 8);    //TODO: If root send $
        bufpos += 8;
        txBuff[bufpos] = 32;
        bufpos++;
        
        // Exec address
        text=String(execAddress,HEX);
      
        while (text.length() < 8){
          // Pad address if short
          text="0"+text;
        }
      
        text.toUpperCase();
      
        writeStringtoTX(text, bufpos, 8);  
        bufpos += 8;
        txBuff[bufpos] = 32;
        bufpos++;
      
        
        // Length
        txBuff[bufpos] = 32;
        bufpos++;
        txBuff[bufpos] = 32;
        bufpos++;
      
        text=String(fileSize,HEX);
      
        while (text.length() < 6){
          // Pad address if short
          text="0"+text;
        }
      
        text.toUpperCase();
      
        writeStringtoTX(text, bufpos, 6);   
        bufpos += 6;
        txBuff[bufpos] = 32;
        bufpos++;
        txBuff[bufpos] = 32;
        bufpos++;
        txBuff[bufpos] = 32;
        bufpos++;
      
        // 7 characters access attributes
        padding = 0;
        if ((fileAttr & 64) || (fileAttr & 16)) {
          txBuff[bufpos] = 76;
          bufpos ++;
        } else padding++; // Locked
        if (fileAttr & 32) {
          txBuff[bufpos] = 68;
          bufpos ++;
        } else padding++; // Directory
        if (fileAttr & 8) {
          txBuff[bufpos] = 87;
          bufpos ++;
        } else padding++; // Owner W
        if (fileAttr & 4) {
          txBuff[bufpos] = 82;
          bufpos ++;
        } else padding++; // Owner R
        txBuff[bufpos] = 47; // / seperator
        bufpos++;
        if (fileAttr & 2) {
          txBuff[bufpos] = 119;
          bufpos ++;
        } else padding++; // Public w
        if (fileAttr & 1) {
          txBuff[bufpos] = 114;
          bufpos ++;
        } else padding++; // Public r
      
        while (padding > 0) {
          txBuff[bufpos] = 32;
          bufpos++;
          padding--;
        }
      
        txBuff[bufpos] = 32;
        bufpos++;
        txBuff[bufpos] = 32;
        bufpos++;
        txBuff[bufpos] = 32;
        bufpos++;
        txBuff[bufpos] = 32;
        bufpos++;
      
        // Econet date
        text=econetDateToString(fsDate);
        writeStringtoTX(text, bufpos, 8); 
        bufpos += 8;
        txBuff[bufpos] = 32;
        bufpos++;
      
      
        // System Internal Numbner
        text=String(fileAddress,HEX);
      
        while (text.length() < 6){
          // Pad address if short
          text="0"+text;
        }
      
        text.toUpperCase();
      
        writeStringtoTX(text, bufpos, 6);   
        bufpos += 6;
     
        // Terminator
        txBuff[bufpos] = 0;
        bufpos ++;
        break;
        
      case 2:
        // 10 character filename short format

        pathBuff4[bufpos] = 10; // Name fixed length
        bufpos++;

        //Now the name
        writeStringtoTX(fcName, bufpos, 10);
        bufpos += 10;
        break;

      case 3:
        // 10 character filename format

        //Send name first
        writeStringtoTX(fcName, bufpos, 10);
        bufpos += 10;

        // 7 characters access attributes - padded out to 8 characters
        padding = 1;
        if ((fileAttr & 64) || (fileAttr & 16)) {
          txBuff[bufpos] = 76;
          bufpos ++;
        } else padding++; // Locked
        if (fileAttr & 32) {
          txBuff[bufpos] = 68;
          bufpos ++;
        } else padding++; // Directory
        if (fileAttr & 8) {
          txBuff[bufpos] = 87;
          bufpos ++;
        } else padding++; // Owner W
        if (fileAttr & 4) {
          txBuff[bufpos] = 82;
          bufpos ++;
        } else padding++; // Owner R
        txBuff[bufpos] = 47; // / seperator
        bufpos++;
        if (fileAttr & 2) {
          txBuff[bufpos] = 119;
          bufpos ++;
        } else padding++; // Public w
        if (fileAttr & 1) {
          txBuff[bufpos] = 114;
          bufpos ++;
        } else padding++; // Public r

        while (padding > 0) {
          txBuff[bufpos] = 32;
          bufpos++;
          padding--;
        }

        txBuff[bufpos] = 0;
        bufpos++;
        break;
    };
    file.close();
    dirCount++; // Increment counter as successfully parsed a file
  }

  txBuff[bufpos] = 0x80;
  bufpos++;

  txBuff[6] = dirCount; // Populate with number of items returned later
  txBuff[7] = dirCount; // Populate with directory size later

  Serial.print(F(", found items: "));
  Serial.print(dirCount, HEX);
  if (txWithHandshake(bufpos + 1, txPort, 0x80)) Serial.println(F(" .")); else Serial.println(F(" !"));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function 6 - Open File
/////////////////////////////////////////////////////////////////////

void fsOpen(int txPort) {
  SdFile file;
  dir_t dirEntry;
  sdSelect(); // Make sure SD card is selected on the SPI bus

  int usrHdl = fsGetUserHdl(rxBuff[3], rxBuff[2]);
  if (usrHdl == -1) {
    fsError(0xBF, F("Who are you?"), txPort);
    return;
  }

  String workingDir;

  workingDir = getDirectoryPath(usrHdl, rxBuff[7]);

  byte existingFile = rxBuff[9];
  byte readOnly = rxBuff[10];
  String objectName = getStringFromRX(11, 128);
  String fatPath = convertToFATPath(objectName, workingDir, txPort);

  String pathName = getStringFromRX(12, 128);
  Serial.print(F(" - user "));
  Serial.print(usrHdl);
  Serial.print(F(" dir handle "));
  Serial.print(rxBuff[7]);
  Serial.print(F(" working dir "));
  Serial.print(workingDir);
  Serial.print(F(", existing "));
  Serial.print(existingFile);
  Serial.print(F(", readonly "));
  Serial.print(readOnly);
  Serial.print(F(" object "));
  Serial.print(objectName);
  Serial.print(F(" disk path "));
  Serial.print(fatPath);

  if (isObjectLocked(fatPath) && !readOnly){
    // Object locked 
    fsError(0xBD,"Access violation",txPort);
    return; 
  }  

  fatPath.toCharArray(pathBuff1, DIRENTRYSIZE);

  if (!sd.exists(pathBuff1) && existingFile) {
    // Request to open an existing file that does not exist
    fsError(0xD6, pathName + " Not found", txPort);
    return;
  }

  if (file.open(pathBuff1, O_READ)) {
    if (file.isDir() && !existingFile && !readOnly) {
      file.close();
      fsError(0xBE, F("Object is a directory "), txPort);
      return;
    }
    file.close();
  }
  
  if(sd.exists(pathBuff1)){
    if (!hasUserAccess(usrHdl,fatPath,readOnly)){  
      // User does not have appropriate access to existing file
      fsError(0xBD,"Access violation",txPort);
      return;
    } 
  } else { 
    if (!fatPath.startsWith(getURD(usrHdl),0) && !isSystemUser(usrHdl)){
      // Request to create a new file, and user does not own parent directory
      fsError(0xBD,"Insufficient Access",txPort);
      return; 
    } 
  }



  int fHdl = 1;
  while (fHandleActive[fHdl] && fHdl < MAXFILES) {
    fHdl++;
  };
  if (fHdl == MAXFILES) {
    // Run out of file handles
    fsError(0xc0, "Too many open files ", txPort);
    return;
  }

  if (userOpenFiles[usrHdl] >= MAXUSERFILES) {
    // Run out of file handles
    fsError(0xc0, "Too many open files ", txPort);
    return;
  }

  //TODO: Check if file is already open. Refuse write request if any other handles open, refuse read if open for write.
  if (readOnly) {
    if (fHandle[fHdl].open(pathBuff1, O_READ)) {
      fHandleActive[fHdl] = true;
      fHandleUser[fHdl] = usrHdl;
    } else {
      // Open failed for some reason
      fsError(0xFF, "Server error - Unable to open file for reading ", txPort);
      return;
    }
  } else {
    // Open for writing, create if necessary
    if (fHandle[fHdl].open(pathBuff1, (O_RDWR | O_CREAT))) {
      fHandleActive[fHdl] = true;
      fHandleUser[fHdl] = usrHdl;
    } else {
      // Open failed for some reason
      fsError(0xFF, "Server error - Unable to open file for writing ", txPort);
      return;
    }
  }

  fHandleIsFolder[fHdl] = false;
  setfHandlePath(fHdl, fatPath);
  userOpenFiles[usrHdl]++;
  fSequence[fHdl] = 2; // Set sequence number to undefined (can only be 0 or 1 normally)

  Serial.print(F(" - success, new handle = "));
  Serial.print(fHdl);

  txBuff[0] = rxBuff[2];
  txBuff[1] = rxBuff[3];
  txBuff[2] = config_Station;
  txBuff[3] = config_Net;
  txBuff[4] = 0x00;
  txBuff[5] = 0x00;
  txBuff[6] = fHdl;
  if (txWithHandshake(7, txPort, 0x80)) Serial.println(F(".")); else Serial.println(F("!"));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function 7 - Close File
/////////////////////////////////////////////////////////////////////

void fsClose(int txPort) {
  sdSelect(); // Make sure SD card is selected on the SPI bus

  int usrHdl = fsGetUserHdl(rxBuff[3], rxBuff[2]);
  if (usrHdl == -1) {
    fsError(0xBF, F("Who are you?"), txPort);
    return;
  }

  byte fileHandle = rxBuff[9];

  Serial.print(F(" - user "));
  Serial.print(usrHdl);
  Serial.print(F(" object handle "));
  Serial.print(fileHandle);
  Serial.print(F(" "));

  if (fileHandle > 0) {
    // Specific file to close
    if (fHandleUser[fileHandle] != usrHdl) {
      // File handle does not belong to user!
      fsError(0xBD, "Insufficient Access", txPort);
      return;
    }
    if (!fHandleActive[fileHandle]) {
      // File handle already closed (this really shouldn't happen)
      fsError(0xDE, "Channel?", txPort);
      return;
    }
    // Still here, so file  or folder really does need closing
    fHandleActive[fileHandle] = false;
    fHandleUser[fileHandle] = -1;
    userOpenFiles[usrHdl]--;
    if (fHandleIsFolder[fileHandle] == false) {
      fHandle[fileHandle].sync();
      if (!fHandle[fileHandle].close()) {
        // File close failed for some reason
        fsError(0xFF, "Error closing file", txPort);
        return;
      }
    }
  } else {
    //User is requesting all files closed (but not folders)
    for (int ptr = 1; ptr < MAXFILES; ptr++) {
      if (fHandleUser[ptr] == usrHdl && fHandleActive[ptr] && fHandleIsFolder[ptr] == false) {
        // Need to close file
        fHandleActive[ptr] = false;
        fHandleUser[ptr] = -1;
        userOpenFiles[usrHdl]--;
        fHandle[ptr].close();
      }
    }
  }
  // Still here, so.response
  txBuff[0] = rxBuff[2];
  txBuff[1] = rxBuff[3];
  txBuff[2] = config_Station;
  txBuff[3] = config_Net;
  txBuff[4] = 0x00;
  txBuff[5] = 0x00;
  if (txWithHandshake(6, txPort, 0x80)) Serial.println(F(".")); else Serial.println(F("!"));

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function 8 - Get Byte
/////////////////////////////////////////////////////////////////////

void fsGetByte(int txPort) {
  sdSelect(); // Make sure SD card is selected on the SPI bus

  int usrHdl = fsGetUserHdl(rxBuff[3], rxBuff[2]);
  if (usrHdl == -1) {
    fsError(0xBF, F("Who are you?"), txPort);
    return;
  }

  byte fileHandle = rxBuff[6];
  byte fileByte;

  Serial.print(F(" - user "));
  Serial.print(usrHdl);
  Serial.print(F(" object handle "));
  Serial.print(fileHandle);
  Serial.print(F(", byte read = "));

  if (fileHandle == 0) {
    //Invalid file handle
    fsError(0xDE, "Channel?", txPort);
  }

  if (fHandleUser[fileHandle] != usrHdl) {
    // File handle does not belong to user!
    fsError(0xBD, "Insufficient Access", txPort);
    return;
  }
  if (!fHandleActive[fileHandle]) {
    // File handle already closed (this really shouldn't happen)
    fsError(0xDE, "Channel?", txPort);
    return;
  }

  fileByte = fHandle[fileHandle].read();


  Serial.print(fileByte);
  Serial.print(F(" "));

  // Still here, so.response
  txBuff[0] = rxBuff[2];
  txBuff[1] = rxBuff[3];
  txBuff[2] = config_Station;
  txBuff[3] = config_Net;
  txBuff[4] = 0x00;
  txBuff[5] = 0x00;
  txBuff[6] = fileByte;
  txBuff[7] = 0;
  if (fHandle[fileHandle].curPosition() == fHandle[fileHandle].fileSize()) txBuff[7] = 0x80; //At end of file
  if (fileByte == -1) txBuff[7] = 0xC0; //REad past file end (or other error)

  Serial.print(fileByte);
  Serial.print(F(" status byte "));
  Serial.print(txBuff[7], HEX);
  Serial.print(F(" "));

  if (txWithHandshake(8, txPort, rxControlByte)) Serial.println(F(".")); else Serial.println(F("!"));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function 9 - Put Byte
/////////////////////////////////////////////////////////////////////

void fsPutByte(int txPort) {
  sdSelect(); // Make sure SD card is selected on the SPI bus

  int usrHdl = fsGetUserHdl(rxBuff[3], rxBuff[2]);
  if (usrHdl == -1) {
    fsError(0xBF, F("Who are you?"), txPort);
    return;
  }

  byte fileHandle = rxBuff[6];
  byte fileByte = rxBuff[7];

  Serial.print(F(" - user "));
  Serial.print(usrHdl);
  Serial.print(F(" object handle "));
  Serial.print(fileHandle);
  Serial.print(F(" byte = "));
  Serial.print(fileByte);
  Serial.print(F(" this sequence = "));
  Serial.print(rxControlByte & 1);
  Serial.print(F(" last sequence = "));
  Serial.print(fSequence[fileHandle]);
  Serial.print(F(" "));

  if (fileHandle > 0) {
    // Specific file to close
    if (fHandleUser[fileHandle] != usrHdl) {
      // File handle does not belong to user!
      fsError(0xBD, "Insufficient Access", txPort);
      return;
    }
    if (!fHandleActive[fileHandle]) {
      // File handle already closed (this really shouldn't happen)
      fsError(0xDE, "Channel?", txPort);
      return;
    }

    if (rxControlByte & 1 == fSequence[fileHandle]) {
      // Have received a duplicate, diskard the write
      Serial.print(" Duplicate sequence - skipping write ");
    } else {
      // All good, write that byte!
      if (fHandle[fileHandle].write(fileByte) != 1) {
        // write failed for some reason
        fsError(0xFF, "Error writing", txPort);
        return;
      }
    }

    // Update stored sequence number
    fSequence[fileHandle] = rxControlByte & 1;

  } else {
    //Invalid file handle
    fsError(0xDE, "Channel?", txPort);
  }
  // Still here, so.response
  txBuff[0] = rxBuff[2];
  txBuff[1] = rxBuff[3];
  txBuff[2] = config_Station;
  txBuff[3] = config_Net;
  txBuff[4] = 0x00;
  txBuff[5] = 0x00;
  if (txWithHandshake(6, txPort, rxControlByte)) Serial.println(F(".")); else Serial.println(F("!"));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function 10 - Get Bytes
/////////////////////////////////////////////////////////////////////

void fsGetBytes(int txPort) {
  sdSelect(); // Make sure SD card is selected on the SPI bus

  int usrHdl = fsGetUserHdl(rxBuff[3], rxBuff[2]);
  if (usrHdl == -1) {
    fsError(0xBF, F("Who are you?"), txPort);
    return;
  }

  byte dataTXPort = rxBuff[6];
  byte fileHandle = rxBuff[9];
  byte useOffset = rxBuff[10];
  long bytesToFetch = (rxBuff[13] << 16) + (rxBuff[12] << 8) + rxBuff[11];
  long fileOffset = (rxBuff[16] << 16) + (rxBuff[15] << 8) + rxBuff[14];

  Serial.print(F(" - user "));
  Serial.print(usrHdl);
  Serial.print(F(" object handle "));
  Serial.print(fileHandle);
  Serial.print(F(" txport "));
  Serial.print(dataTXPort);
  Serial.print(F(" bytes requested "));
  Serial.print(bytesToFetch);
  Serial.print(F(" use offset "));
  Serial.print(useOffset);
  Serial.print(F(" offset "));
  Serial.print(fileOffset);
  Serial.print(F(" "));

  if (fileHandle == 0) {
    //Invalid file handle
    fsError(0xDE, "Channel?", txPort);
    return;
  }
  if (fHandleUser[fileHandle] != usrHdl) {
    // File handle does not belong to user!
    fsError(0xBD, "Insufficient Access", txPort);
    return;
  }
  if (!fHandleActive[fileHandle]) {
    // File handle already closed (this really shouldn't happen)
    fsError(0xDE, "Channel?", txPort);
    return;
  }

  // First move to location if required
  if (useOffset == 0) {
    // Need to adjust file pointer
    if (!fHandle[fileHandle].seekSet(fileOffset)) {
      // seek failed for some reason
      fsError(0xFF, "Error seeking", txPort);
      return;
    }
  }

  long reqPtr = 0;
  long bytesRead = 0;
  boolean eofReached = false;
  boolean fileError = false;


  // Send an OK so we can begin TXing the requested bytes
  txBuff[0] = rxBuff[2];
  txBuff[1] = rxBuff[3];
  txBuff[2] = config_Station;
  txBuff[3] = config_Net;
  txBuff[4] = 0x00;
  txBuff[5] = 0x00;
  if (txWithHandshake(6, txPort, rxControlByte)) Serial.print(F(""));
  else {
    Serial.println(F("- Begin Failed"));
    return;
  };


  byte rxControlByte = 0x80;

  while (reqPtr < bytesToFetch) {
    int toFetch = (bytesToFetch - reqPtr);
    if (toFetch > (BUFFSIZE - 8)) toFetch = BUFFSIZE - 8; // Allow a few bytes for addressing

    int result = fHandle[fileHandle].read(&txBuff[4], toFetch);

    reqPtr += toFetch;
    if (result == -1) fileError = true; else if (result < toFetch) eofReached = true;

    bytesRead += result;

    if (!txWithHandshake(toFetch + 4, dataTXPort, rxControlByte)) {
      Serial.print(F(" - block TX failed at "));
      Serial.println(reqPtr);
      return;
    }
  }

  // Have TXed right number of bytes, do closing message
  if (!fileError) {
    txBuff[0] = rxBuff[2];
    txBuff[1] = rxBuff[3];
    txBuff[2] = config_Station;
    txBuff[3] = config_Net;
    txBuff[4] = 0x00;
    txBuff[5] = 0x00;
    if (eofReached) txBuff[6] = 0x80; else txBuff[6] = 0x00;

    txBuff[7] = bytesRead & 0xFF;
    txBuff[8] = bytesRead >> 8 & 0xFF;
    txBuff[9] = bytesRead >> 16 & 0xFF;
    //Serial.print(F("About to tx close message "));

    if (txWithHandshake(10, txPort, rxControlByte)) Serial.println(F(" ")); else Serial.println(F(" - completion Failed"));

  } else {

    // File error returned by read
    fsError(0xFF, "Read Failed", txPort);

  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function 11 - Put Bytes
/////////////////////////////////////////////////////////////////////

void fsPutBytes(int txPort) {
  sdSelect(); // Make sure SD card is selected on the SPI bus

  int usrHdl = fsGetUserHdl(rxBuff[3], rxBuff[2]);
  if (usrHdl == -1) {
    fsError(0xBF, F("Who are you?"), txPort);
    return;
  }

  byte AckPort = rxBuff[6];
  // CSD and Lib handles not used
  byte fileHandle = rxBuff[9];
  byte useOffset = rxBuff[10];
  unsigned long bytesToWrite = (rxBuff[13] << 16) + (rxBuff[12] << 8) + rxBuff[11];
  unsigned long fileOffset = (rxBuff[16] << 16) + (rxBuff[15] << 8) + rxBuff[14];
  byte dataPort = fileHandle + 129; // Makes life easier to find the filehandle
  int maxTXSize = BUFFSIZE - 6;

  Serial.print(F(" - user "));
  Serial.print(usrHdl);
  Serial.print(F(" object handle "));
  Serial.print(fileHandle);
  Serial.print(F(" ack port "));
  Serial.print(AckPort, HEX);
  Serial.print(F(" data port "));
  Serial.print(dataPort, HEX);
  Serial.print(F(" bytes to write "));
  Serial.print(bytesToWrite);
  Serial.print(F(" use offset "));
  Serial.print(useOffset);
  Serial.print(F(" offset "));
  Serial.print(fileOffset);
  Serial.print(F(" max TX size "));
  Serial.print(maxTXSize);
  Serial.print(F(" "));

  if (fileHandle == 0) {
    //Invalid file handle
    fsError(0xDE, "Channel?", txPort);
    return;
  }
  if (fHandleUser[fileHandle] != usrHdl) {
    // File handle does not belong to user!
    fsError(0xBD, "Insufficient Access", txPort);
    return;
  }
  if (!fHandleActive[fileHandle]) {
    // File handle already closed (this really shouldn't happen)
    fsError(0xDE, "Channel?", txPort);
    return;
  }

  // Set up the tables
  fSequence[fileHandle] = 2;
  dataReplyPort[fileHandle] = txPort;
  dataAckPort[fileHandle] = AckPort;
  expectingBytes[fileHandle] = bytesToWrite;
  gotBytes[fileHandle] = 0;
  isSave[fileHandle] = false;


  // First move to location if required
  if (useOffset == 0) {
    // Need to adjust file pointer
    if (!fHandle[fileHandle].seekSet(fileOffset)) {
      // seek failed for some reason
      fsError(0xFF, "Error seeking", txPort);
      return;
    }
  }

  // Send an OK so station can start the bulk TX process
  txBuff[0] = rxBuff[2];
  txBuff[1] = rxBuff[3];
  txBuff[2] = config_Station;
  txBuff[3] = config_Net;
  txBuff[4] = 0x00; // Command code 0
  txBuff[5] = 0x00; // Result OK
  txBuff[6] = dataPort;
  txBuff[7] = maxTXSize;
  txBuff[8] = maxTXSize << 8;
  if (txWithHandshake(9, txPort, rxControlByte)) Serial.println(F(""));
  else {
    Serial.println(F("- setup TX Failed"));
    return;
  };
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Bulk incoming transfer received - set up by Function 1 Save File, or Function 11 put bytes
/////////////////////////////////////////////////////////////////////

void fsBulkRXArrived(int rxPort, int bytesRX) {
  sdSelect(); // Make sure SD card is selected on the SPI bus

  int fileHandle = (rxPort - 129);
  byte ackPort = dataAckPort[fileHandle];
  byte replyPort = dataReplyPort[fileHandle];

  int usrHdl = fsGetUserHdl(rxBuff[3], rxBuff[2]);
  if (usrHdl == -1) {
    fsError(0xBF, F("Who are you?"), ackPort);
    return;
  }

  if (fileHandle == 0) {
    //Invalid file handle
    fsError(0xDE, "Channel?", replyPort);
    return;
  }
  if (fHandleUser[fileHandle] != usrHdl) {
    // File handle does not belong to user!
    fsError(0xBD, "Insufficient Access", replyPort);
    return;
  }
  if (!fHandleActive[fileHandle]) {
    // File handle already closed (this really shouldn't happen)
    fsError(0xDE, "Channel?", replyPort);
    return;
  }

  int result = 0;

  // File handle seems OK

  if (rxControlByte & 1 == fSequence[fileHandle]) {
    // Have received a duplicate, discard the write
    Serial.print(" Duplicate sequence number - skipping write ");
  } else {
    // All good, write that byte!
    int result = fHandle[fileHandle].write(&rxBuff[4], bytesRX - 4);
    // fHandle[fileHandle].sync();

    // Update byte count
    gotBytes[fileHandle] += (bytesRX - 4);

    // Update stored sequence number
    //fSequence[fileHandle]=rxControlByte & 1;
  }

  // delay(100); // Some clients don't like you answering too quickly!

  if (gotBytes[fileHandle] != expectingBytes[fileHandle]) {
    // Not all bytes received yet, send ack.
    txBuff[0] = rxBuff[2];
    txBuff[1] = rxBuff[3];
    txBuff[2] = config_Station;
    txBuff[3] = config_Net;
    txBuff[4] = 0x00; // Any byte can be sent as the ack

    if (!txWithHandshake(5, ackPort, rxControlByte)) {
      Serial.println(F("Client failed to acknowledge during bulk transfer"));
      // Clear out the ADLC before returning
      readFIFO();
      resetIRQ();
      return;
    }
  } else {
    // All expected bytes received

    if (result == -1) {
      // Transfer completed with a write failure
      fsError(0xFF, "Internal server error writing file", replyPort);
      return;
    }

    // Transfer complete, send final reply
    if (isSave[fileHandle]) {
      // File is open for saving, close file and send ack

      dir_t dirEntry;
      fHandle[fileHandle].dirEntry(&dirEntry);
      int fsDate = getEconetDate(&dirEntry);

      txBuff[0] = rxBuff[2];
      txBuff[1] = rxBuff[3];
      txBuff[2] = config_Station;
      txBuff[3] = config_Net;
      txBuff[4] = 0x00; // Command code
      txBuff[5] = 0x00; // result = OK
      txBuff[6] = 15; // TODO: correct attributes (hardcoded to allow all access, not locked)
      txBuff[7] = fsDate;
      txBuff[8] = fsDate << 8 ;

      if (!txWithHandshake(9, replyPort, rxControlByte)) {
        Serial.println(F("Ack failed at end of save bulk transfer"));
        return; // return without closing, as final frame may get sent again
      }

      // Close the filehandle and mark inactive in the table
      fHandle[fileHandle].truncate(expectingBytes[fileHandle]); // Ensure file is correct size - might be overwriting existing file
      fHandle[fileHandle].close();
      userOpenFiles[usrHdl]--;
      fHandleActive[fileHandle] = false;

    } else {
      // File is open for put bytes, send put bytes acknowledgement
      txBuff[0] = rxBuff[2];
      txBuff[1] = rxBuff[3];
      txBuff[2] = config_Station;
      txBuff[3] = config_Net;
      txBuff[4] = 0x00; // Command code
      txBuff[5] = 0x00; // result = OK
      txBuff[6] = 0x00; // Undefined content
      txBuff[7] = expectingBytes[fileHandle];
      txBuff[8] = expectingBytes[fileHandle] << 8 ;
      txBuff[9] = expectingBytes[fileHandle] << 16;

      if (!txWithHandshake(10, replyPort, rxControlByte)) Serial.print(F("Ack failed at end of put bulk transfer"));
    }   
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function 12 - Get Random Access Arguments
/////////////////////////////////////////////////////////////////////

void fsGetRandomAccessArgs(int txPort) {
  sdSelect(); // Make sure SD card is selected on the SPI bus

  int usrHdl = fsGetUserHdl(rxBuff[3], rxBuff[2]);
  if (usrHdl == -1) {
    fsError(0xBF, F("Who are you?"), txPort);
    return;
  }

  byte fileHandle = rxBuff[9];
  byte fileAction = rxBuff[10];

  long reqValue = 0;

  Serial.print(F(" - user "));
  Serial.print(usrHdl);
  Serial.print(F(" object handle "));
  Serial.print(fileHandle);
  Serial.print(F(" action "));
  Serial.print(fileAction);
  Serial.print(F(", result "));
  Serial.print(F(" "));

  if (fileHandle == 0) {
    //Invalid file handle
    fsError(0xDE, "Channel?", txPort);
    return;
  }

  if (fHandleUser[fileHandle] != usrHdl) {
    // File handle does not belong to user!
    fsError(0xBD, "Insufficient Access", txPort);
    return;
  }
  if (!fHandleActive[fileHandle]) {
    // File handle already closed (this really shouldn't happen)
    fsError(0xDE, "Channel?", txPort);
    return;
  }

  if (fileAction == 0) {
    // Get sequential position
    reqValue = fHandle[fileHandle].curPosition();

    if (reqValue == -1) {
      // Failed for some reason
      fsError(0xFF, "Error getting position", txPort);
      return;
    }
  } else if (fileAction == 1 || fileAction == 2) {
    // Get file extent or allocation (TODO: get proper allocation)

    reqValue = fHandle[fileHandle].fileSize();

    if (reqValue == -1) {
      // Failed for some reason
      fsError(0xFF, "Error getting size", txPort);
      return;
    }

  } else {
    fsError(0xFF, "Unknown action", txPort);
  }

  Serial.print(reqValue);
  Serial.print(F(" "));

  // Still here, so.response and value
  txBuff[0] = rxBuff[2];
  txBuff[1] = rxBuff[3];
  txBuff[2] = config_Station;
  txBuff[3] = config_Net;
  txBuff[4] = 0x00;
  txBuff[5] = 0x00;
  txBuff[6] = reqValue & 0xFF;
  txBuff[7] = reqValue >> 8 & 0xFF;
  txBuff[8] = reqValue >> 16 & 0xFF;
  if (txWithHandshake(9, txPort, 0x80)) Serial.println(F(".")); else Serial.println(F("!"));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function 13 - Set Random Access Arguments
/////////////////////////////////////////////////////////////////////

void fsSetRandomAccessArgs(int txPort) {
  sdSelect(); // Make sure SD card is selected on the SPI bus

  int usrHdl = fsGetUserHdl(rxBuff[3], rxBuff[2]);
  if (usrHdl == -1) {
    fsError(0xBF, F("Who are you?"), txPort);
    return;
  }

  byte fileHandle = rxBuff[9];
  byte fileAction = rxBuff[10];
  long fileValue = (rxBuff[13] << 16) + (rxBuff[12] << 8) + rxBuff[11];
  byte extByte = 0;

  Serial.print(F(" - user "));
  Serial.print(usrHdl);
  Serial.print(F(" object handle "));
  Serial.print(fileHandle);
  Serial.print(F(" action "));
  Serial.print(fileAction);
  Serial.print(F(" by amount "));
  Serial.print(fileValue);
  Serial.print(F(" "));

  if (fileHandle > 0) {
    // 0 is invalid
    if (fHandleUser[fileHandle] != usrHdl) {
      // File handle does not belong to user!
      fsError(0xBD, "Insufficient Access", txPort);
      return;
    }
    if (!fHandleActive[fileHandle]) {
      // File handle already closed (this really shouldn't happen)
      fsError(0xDE, "Channel?", txPort);
      return;
    }

    if (fileAction == 0) {
      // Set sequential position
      if (!fHandle[fileHandle].seekSet(fileValue)) {
        // Seek fail

        if (fHandle[fileHandle].fileSize() < fileValue) {
          // Seek past end of file - so extend it
          for (int thisByte = fHandle[fileHandle].fileSize(); thisByte < fileValue; thisByte++) {
            fHandle[fileHandle].write(extByte);
          }
        } else {
          // Seek within file failed!
          fsError(0xFF, "Seek fail", txPort);
          return;
        }
      }
    } else if (fileAction == 1) {
      // Set file extent
      //TODO: Check quota
      // for (int thisByte=0;thisByte<fileValue;thisByte++){
      //   fHandle[fileHandle].write(extByte);
      // }
    } else {
      fsError(0xFF, "Unknown action", txPort);
    }

  } else {
    //Invalid file handle
    fsError(0xDE, "Channel?", txPort);
  }
  // Still here, so.response
  txBuff[0] = rxBuff[2];
  txBuff[1] = rxBuff[3];
  txBuff[2] = config_Station;
  txBuff[3] = config_Net;
  txBuff[4] = 0x00;
  txBuff[5] = 0x00;
  if (txWithHandshake(6, txPort, 0x80)) Serial.println(F(".")); else Serial.println(F("!"));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function 14 - Read disk information
/////////////////////////////////////////////////////////////////////

void fsReadDiskInfo(byte txPort) {

  if (rxBuff[9] != 0) {
    // Non-First drive requested - send null response
    txBuff[0] = rxBuff[2];
    txBuff[1] = rxBuff[3];
    txBuff[2] = config_Station;
    txBuff[3] = config_Net;
    txBuff[4] = 0x00;
    txBuff[5] = 0x00;
    txBuff[6] = 0x00; // No disks
    txBuff[7] = 0x00;

    if (txWithHandshake(7, txPort, 0x80)) Serial.println(F(".")); else Serial.println(F("!"));


  } else {
    // First drive requested
    txBuff[0] = rxBuff[2];
    txBuff[1] = rxBuff[3];
    txBuff[2] = config_Station;
    txBuff[3] = config_Net;
    txBuff[4] = 0x00;
    txBuff[5] = 0x00;
    txBuff[6] = 0x01; // Number of disks in response
    txBuff[7] = 0x00; // Begin disk 0

    writeStringtoTX(config_FSName, 8, 16);

    if (txWithHandshake(23, txPort, 0x80)) Serial.println(F(".")); else Serial.println(F("!"));
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function 15 - Read Current Users information
/////////////////////////////////////////////////////////////////////

void fsReadCurrentUsers(int txPort) {
  sdSelect(); // Make sure SD card is selected on the SPI bus

  int usrHdl = fsGetUserHdl(rxBuff[3], rxBuff[2]);
  if (usrHdl == -1) {
    fsError(0xBF, F("Who are you?"), txPort);
    return;
  }

  byte firstRecord = rxBuff[9];
  byte totalRecords = rxBuff[10];

  if (totalRecords == 0) totalRecords = MAXUSERS;

  Serial.print(F(" - user "));
  Serial.print(usrHdl);
  Serial.print(F(" from "));
  Serial.print(firstRecord);
  Serial.print(F(" to "));
  Serial.print(totalRecords);

  byte recordsReturned = 0;

  txBuff[0] = rxBuff[2];
  txBuff[1] = rxBuff[3];
  txBuff[2] = config_Station;
  txBuff[3] = config_Net;
  txBuff[4] = 0x00;
  txBuff[5] = 0x00;
  // txBuff[6]=0x00; // Entries returned - gets written later

  int bufptr = 7;

  for (int thisRecord = firstRecord; thisRecord < MAXUSERS; thisRecord++) {
    if (stataddress[thisRecord] != 0) {
      // User handle is in use
      // Add station details
      txBuff[bufptr] = stataddress[thisRecord];
      bufptr++;
      txBuff[bufptr] = netaddress[thisRecord];
      bufptr++;

      // Add user details
      String userName = getUsername(thisRecord);
      userName.toCharArray((char *)&txBuff[bufptr], 22);
      bufptr += userName.length();
      txBuff[bufptr] = 0x0D; //Return character
      bufptr++;

      // Add priv details
      txBuff[bufptr] = userPriv[thisRecord];
      bufptr++;

      recordsReturned++;
    }
    if (recordsReturned == totalRecords) break;
  }

  txBuff[6] = recordsReturned;

  Serial.print(F(", sending "));
  Serial.print(recordsReturned);

  if (txWithHandshake(bufptr, txPort, 0x80)) Serial.println(F(".")); else Serial.println(F("!"));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function 16 - Get File Server Date and Time
/////////////////////////////////////////////////////////////////////

void fsGetDateTime(int txPort) {
  int ecolodate = ((year() - 1981) << 1 & 224) + day();
  int ecohidate = ((year() - 1981) << 4) + month();
  txBuff[0] = rxBuff[2];
  txBuff[1] = rxBuff[3];
  txBuff[2] = config_Station;
  txBuff[3] = config_Net;
  txBuff[4] = 0x00;
  txBuff[5] = 0x00;
  txBuff[6] = ecolodate;
  txBuff[7] = ecohidate;
  txBuff[8] = hour();
  txBuff[9] = minute();
  txBuff[10] = second();
  if (txWithHandshake(11, txPort, 0x80)) Serial.println(F(".")); else Serial.println(F("!"));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function 17 - Get End of File status
/////////////////////////////////////////////////////////////////////

void fsGetEndOfFile(int txPort) {
  sdSelect(); // Make sure SD card is selected on the SPI bus

  int usrHdl = fsGetUserHdl(rxBuff[3], rxBuff[2]);
  if (usrHdl == -1) {
    fsError(0xBF, F("Who are you?"), txPort);
    return;
  }

  byte fileHandle = rxBuff[9];

  long reqValue = 0;

  Serial.print(F(" - user "));
  Serial.print(usrHdl);
  Serial.print(F(" object handle "));
  Serial.print(fileHandle);
  Serial.print(F(", result "));
  ;

  if (fileHandle == 0) {
    //Invalid file handle
    fsError(0xDE, "Channel?", txPort);
    return;
  }

  if (fHandleUser[fileHandle] != usrHdl) {
    // File handle does not belong to user!
    fsError(0xBD, "Insufficient Access", txPort);
    return;
  }
  if (!fHandleActive[fileHandle]) {
    // File handle already closed (this really shouldn't happen)
    fsError(0xDE, "Channel?", txPort);
    return;
  }


  reqValue = fHandle[fileHandle].curPosition();

  if (reqValue == -1) {
    // Failed for some reason
    fsError(0xFF, "Error getting position", txPort);
    return;
  }

  byte result = 0; // pointer in file
  if (reqValue > fHandle[fileHandle].fileSize()) result = 1;

  Serial.print(result);

  // Still here, so.response and value
  txBuff[0] = rxBuff[2];
  txBuff[1] = rxBuff[3];
  txBuff[2] = config_Station;
  txBuff[3] = config_Net;
  txBuff[4] = 0x00;
  txBuff[5] = 0x00;
  txBuff[6] = result;
  if (txWithHandshake(7, txPort, 0x80)) Serial.println(F(".")); else Serial.println(F("!"));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function 18 - Read Object Information
/////////////////////////////////////////////////////////////////////

void fsReadObjectInfo(byte txPort) {
  SdFile file;
  dir_t dirEntry;
  sdSelect(); // Make sure SD card is selected on the SPI bus
  char nameBuffer[128];

  byte command = rxBuff[9];
  byte bufpos;
  // String pathName=(char*)userCSD;
  String pathName = getStringFromRX(10, 128);

  int usrHdl = fsGetUserHdl(rxBuff[3], rxBuff[2]);
  if (usrHdl == -1) {
    fsError(0xBF, F("Who are you?"), txPort);
    return;
  }

  String workingDir;
  workingDir = getDirectoryPath(usrHdl, rxBuff[7]);


  String fatPath = convertToFATPath(pathName, workingDir, txPort);
  Serial.print(F(" - user "));
  Serial.print(usrHdl);
  Serial.print(F(" dir handle "));
  Serial.print(rxBuff[7]);
  Serial.print(F(" working dir "));
  Serial.print(workingDir);
  Serial.print(F(" operation "));
  Serial.print(command);
  Serial.print(F(" on object "));
  Serial.print(pathName);
  Serial.print(F(" ("));
  Serial.print(fatPath);
  Serial.print(F(")"));


  // No access control required, it's always possible to read attributes

  fatPath.toCharArray(pathBuff1, DIRENTRYSIZE);

  // Start building reply
  txBuff[0] = rxBuff[2];
  txBuff[1] = rxBuff[3];
  txBuff[2] = rxBuff[0];
  txBuff[3] = rxBuff[1];
  txBuff[4] = 0; // Command code 0
  txBuff[5] = 0; // Is successful



  if (!sd.exists(pathBuff1)) {
    //Object not found - apparently this needs to be all zeros returned
    for (int bptr = 6; bptr < 22; bptr++) {
      txBuff[bptr] = 0;
    }

    int replySize = 5;

    switch (command) {
      case 1:
        replySize = 7;
        break;
      case 2:
        replySize = 13;
        break;
      case 3:
        replySize = 8;
        break;
      case 4:
        replySize = 7;
        break;
      case 5:
        replySize = 22;
        break;
    }

    if (txWithHandshake(replySize, txPort, 0x80)) Serial.println(F("Not found .")); else Serial.println(F("Not found !"));
    return;
  }

  if (file.isDir()) {
    txBuff[6] = 2;
  } else {
    txBuff[6] = 1;
  };

  bufpos = 7;


  if (file.open(pathBuff1, O_READ)) {

    file.getName(nameBuffer, 128);
    file.dirEntry(&dirEntry);
    unsigned long execAddress = getExecAddressForObject(pathBuff1);
    unsigned long loadAddress = getLoadAddressForObject(pathBuff1);
    byte fileAttr = getAttributes(pathBuff1);
    unsigned int fileAddress = dirEntry.firstClusterLow;
    bool directory = file.isDir();
    unsigned int fileSize = file.fileSize();
    int fsDate = getEconetDate(&dirEntry);



    txBuff[0] = rxBuff[2];
    txBuff[1] = rxBuff[3];
    txBuff[2] = rxBuff[0];
    txBuff[3] = rxBuff[1];
    txBuff[4] = 0; // Command code 0
    txBuff[5] = 0; // Is successful
    if (file.isDir()) {
      txBuff[6] = 2;
    } else {
      txBuff[6] = 1;
    };

    bufpos = 7;

    if (command == 2 || command == 5) {
      // Load and Execution addresses
      txBuff[bufpos] = loadAddress;
      bufpos++;
      txBuff[bufpos] = loadAddress >> 8;
      bufpos++;
      txBuff[bufpos] = loadAddress >> 16;
      bufpos++;
      txBuff[bufpos] = loadAddress >> 24;
      bufpos++;

      txBuff[bufpos] = execAddress;
      bufpos++;
      txBuff[bufpos] = execAddress >> 8;
      bufpos++;
      txBuff[bufpos] = execAddress >> 16;
      bufpos++;
      txBuff[bufpos] = execAddress >> 24;
      bufpos++;
    }

    if (command == 3 || command == 5) {
      // Length required
      txBuff[bufpos] = fileSize;
      bufpos++;
      txBuff[bufpos] = fileSize >> 8;
      bufpos++;
      txBuff[bufpos] = fileSize >> 16;
      bufpos++;
    }

    if (command == 4 || command == 5) {
      txBuff[bufpos] = fileAttr;
      bufpos++;
    }

    if (command == 1 || command == 5) {
      // Date required
      txBuff[bufpos] = fsDate;
      bufpos++;
      txBuff[bufpos] = fsDate >> 8;
      bufpos++;
    }

    if (command == 6) {
      // Get directory size and attributes
      if (!file.isDir()) {
        fsError(0xBD, "Object is a file", txPort); // May not be correct, but feels right
        return;
      }
      txBuff[bufpos] = 0; // Byte undefined on this command
      bufpos++;
      txBuff[bufpos] = 10; // Length of directory name
      bufpos++;

      // Now insert the directory name
      String fcPath = convertToFilecorePath(fatPath);

      if (fcPath.length() > 2) {
        String dirName;
        int ptr = fcPath.length() - 2;
        dirName = fcPath.substring(ptr);
        while (!(dirName.startsWith(".")) && (ptr > -1)) {
          dirName = fcPath.substring(ptr);
          ptr--;
        }
        dirName = fcPath.substring(ptr + 2);
        if (dirName.endsWith(".")) dirName = dirName.substring(0, dirName.length() - 1);
        writeStringtoTX(dirName, bufpos, 10);
      } else {
        writeStringtoTX(fcPath, bufpos, 10);
      }

      bufpos += 10;

      String urd = getURD(usrHdl);
      if (fatPath.startsWith(urd, 0) || isSystemUser(usrHdl)) txBuff[bufpos] = 0x00; else txBuff[bufpos] = 0xFF; // Directory access rights (0x00= Owner, 0xFF = Public)

      bufpos++;

      txBuff[bufpos] = 00; // TODO: Actual cycle number? Probably not.
      bufpos++;


    }

    file.close();

    if (txWithHandshake(bufpos + 1, txPort, 0x80)) Serial.println(F(".")); else Serial.println(F("!"));

  } else {
    // Don't really know what happened here, as file exists.
    fsError(0x65, "Object not open", txPort); // Seems generic enough?
    return;
  };

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function 19 - Set Object Information
/////////////////////////////////////////////////////////////////////

void fsSetObjectInfo(byte txPort) {
  SdFile file;
  dir_t dirEntry;
  sdSelect(); // Make sure SD card is selected on the SPI bus
  char nameBuffer[128];
  byte command = rxBuff[9];
  byte bufpos = 10;

  int usrHdl = fsGetUserHdl(rxBuff[3], rxBuff[2]);
  if (usrHdl == -1) {
    fsError(0xBF, F("Who are you?"), txPort);
    return;
  }

  String workingDir;

  workingDir = getDirectoryPath(usrHdl, rxBuff[7]);

  switch (command) {
    case 1:
      Serial.print(F("set Load, Exec and Attributes "));
      bufpos += 9;
      break;
    case 2:
      Serial.print(F("set Load "));
      bufpos += 4;
      break;
    case 3:
      Serial.print(F("set Exec "));
      bufpos += 4;
      break;
    case 4:
      Serial.print(F("set Attributes "));
      bufpos += 1;
      break;
    case 5:
      Serial.print(F("set Date "));
      bufpos += 4;
      break;
    default: //No idea!
      fsError(0xFF, F("Unimplemented command"), txPort);
      break;
  }

  // String pathName=(char*)userCSD;
  String pathName = getStringFromRX(bufpos, 128);
  String fatPath = convertToFATPath(pathName, workingDir, txPort);
  Serial.print(F(" - user "));
  Serial.print(usrHdl);
  Serial.print(F(" dir handle "));
  Serial.print(rxBuff[7]);
  Serial.print(F(" working dir "));
  Serial.print(workingDir);
  Serial.print(F(" operation "));
  Serial.print(command);


  Serial.print(F(" on object "));
  Serial.print(pathName);
  Serial.print(F(" ("));
  Serial.print(fatPath);
  Serial.print(F(")"));




  fatPath.toCharArray(pathBuff1, DIRENTRYSIZE);


  // Start building reply
  txBuff[0] = rxBuff[2];
  txBuff[1] = rxBuff[3];
  txBuff[2] = rxBuff[0];
  txBuff[3] = rxBuff[1];
  txBuff[4] = 0; // Command code 0
  txBuff[5] = 0; // Is successful

  bufpos = 6;

  if (!sd.exists(pathBuff1)) {
    //Object not found
    fsError(0xD6, pathName + " Not found", txPort);
    return;
  }

  if (!fatPath.startsWith(getURD(usrHdl),0) && !isSystemUser(usrHdl)){
    // User does not own parent directory
    fsError(0xBD,"Insufficient Access",txPort);
    return; 
  }  

  if (!file.open(pathBuff1, O_READ)) {
    // File open failed
    // TODO better error response
    fsError(0xBD, "File open failed", txPort);
    return;
  }

  boolean isDir = file.isDir();
  file.close();

  String metaPath=fatPath;
  metaPath.replace(config_FSRoot, config_MetaRoot);

  if (!isDir) {
    //Object is a file,  update the meta entry
    metaPath.toCharArray(pathBuff2, DIRENTRYSIZE);

    if (!file.exists(pathBuff2)) {
      // Create the missing metafile first to populate defaults
      if (!createMetaFile(fatPath)) {
        fsError(0xFF, "Metafile creation failed", txPort);
        return;
      }
    }

    if (!file.open(pathBuff2, O_RDWR)) {
      // File create failed
      // TODO better error response
      fsError(0xBD, "Metadata file open failed", txPort);
      return;
    }
  } else {

    //Object is a folder, update the folder meta entry    
    metaPath = metaPath + "/$";
    metaPath.toCharArray(pathBuff2, DIRENTRYSIZE);

    if (!file.exists(pathBuff2)) {
      // Create the missing metafile first to populate defaults
      if (!createMetaFile(fatPath)) {
        fsError(0xFF, "Metafile creation failed", txPort);
        return;
      }
    }

    if (!file.open(pathBuff2, O_RDWR)) {
      // File create failed
      // TODO better error response
      fsError(0xBD, "Metadata file open failed, because reasons", txPort);
      return;
    }
  }

  switch (command) {
    case 1:
      //Set Load, Exec and Attributes
      Serial.print("(" + (String)rxBuff[18] + ")");
      if (isDir) {
        rxBuff[18] = rxBuff[18] | 32; // Make sure directory flag is preserved
        rxBuff[18] = rxBuff[18] & 115; // Filter out owner attributes which are unused by NetFS
      }
      Serial.print("(" + (String)rxBuff[18] + ")");
      file.write(&rxBuff[10], 9);
      break;
    case 2:
      //Set Load
      file.write(&rxBuff[10], 4);
      break;
    case 3:
      //Set Exec
      file.seekSet(4);
      file.write(&rxBuff[10], 4);
      break;
    case 4:
      //Set Attributes
      Serial.print("(" + (String)rxBuff[10] + ")");
      file.seekSet(8);
      if (isDir) {
        rxBuff[10] = rxBuff[10] | 32; // Make sure directory flag is preserved
        rxBuff[10] = rxBuff[10] & 115; // Filter out owner attributes which are unused by config_NetFS
      }
      file.write(rxBuff[10]);
      Serial.print("(" + (String)rxBuff[10] + ")");
      break;
  }
  file.close(); //Close the metafile
  if (txWithHandshake(6, txPort, 0x80)) Serial.println(F(" .")); else Serial.println(F(" - TX Failed"));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function 20 - Delete Object
/////////////////////////////////////////////////////////////////////

void fsDelete(byte txPort, boolean oscli) {
  SdFile file;
  sdSelect(); // Make sure SD card is selected on the SPI bus

  int usrHdl = fsGetUserHdl(rxBuff[3], rxBuff[2]);
  if (usrHdl == -1) {
    fsError(0xBF, F("Who are you?"), txPort);
    return;
  }

  String workingDir;

  workingDir = getDirectoryPath(usrHdl, rxBuff[7]);

  String newDir;
  if (!oscli) newDir = getStringFromRX(9, DIRENTRYSIZE); else newDir = getStringFromRX(16, DIRENTRYSIZE);
  Serial.print(" User: ");
  Serial.print(usrHdl);
  Serial.print(" Filename: ");
  String fatPath = convertToFATPath(newDir, workingDir, txPort);
  Serial.print(fatPath);

  if (!hasUserAccess(usrHdl,fatPath,false)){
    // User does not have write access to object
    fsError(0xBD,"Insufficient Access",txPort);
    return; 
  }  

  if (isObjectLocked(fatPath)){
    // Object locked against deletion
    fsError(0xBD,"Locked",txPort);
    return; 
  }  

  fatPath.toCharArray(pathBuff1, DIRENTRYSIZE);

  if (!sd.exists(pathBuff1)) {
    //Object not found - send error and leave
    fsError(0xD6, "Not Found", txPort);
    return;
  }
  //work out if it's a file or directory, as delete is different

  boolean isDir = false;

  if (file.open(pathBuff1, O_READ)) {
    if (file.isDir()) isDir = true;
    file.close();
  } else {
    // File failed to open for examination, can't continue
    fsError(0xFF, "Server internal error", txPort);
    return;
  }

  if (isDir) {
    if (!sd.rmdir(pathBuff1)) {
      fsError(0xFF, "rmdir failed - is directory empty?", txPort);
      return;
    }
  } else {
    if (!sd.remove(pathBuff1)) {
      fsError(0xFF, "rm failed", txPort);
      return;
    }
  }

  if (!isDir) {
    //Clean up metadata file if necessary

    fatPath.replace(config_FSRoot, config_MetaRoot);
    fatPath.toCharArray(pathBuff2, DIRENTRYSIZE);

    if (sd.exists(pathBuff2)) {
      if (!sd.remove(pathBuff2)) {
        fsError(0xFF, "Delete failed for file metadata", txPort);
        return;
      }
    }
  } else {
    // Remove the folder metadata

    fatPath.replace(config_FSRoot, config_MetaRoot);
    fatPath = fatPath + "/$";
    fatPath.toCharArray(pathBuff2, DIRENTRYSIZE);

    if (sd.exists(pathBuff2)) {
      if (!sd.remove(pathBuff2)) {
        fsError(0xFF, "Delete failed for folder metadata", txPort);
        return;
      }
    }
  }

  txBuff[0] = rxBuff[2];
  txBuff[1] = rxBuff[3];
  txBuff[2] = rxBuff[0];
  txBuff[3] = rxBuff[1];
  txBuff[4] = 0; // Command code
  txBuff[5] = 0; // Is successful

  if (txWithHandshake(6, txPort, 0x80)) Serial.println(F(".")); else Serial.println(F("!"));

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function 21 - Get User Environment
/////////////////////////////////////////////////////////////////////

void fsReadUserEnv(byte txPort) {
  int usrHdl = fsGetUserHdl(rxBuff[3], rxBuff[2]);
  if (usrHdl == -1) {
    fsError(0xBF, F("Who are you?"), txPort);
    return;
  }

  String csdPath = getDirectoryPath(usrHdl, rxBuff[7]);
  String libPath = getDirectoryPath(usrHdl, rxBuff[8]);

  txBuff[0] = rxBuff[2];
  txBuff[1] = rxBuff[3];
  txBuff[2] = rxBuff[0];
  txBuff[3] = rxBuff[1];
  txBuff[4] = 0; // Command code 0
  txBuff[5] = 0; // Is successful
  txBuff[6] = 16; //length of disk name

  writeStringtoTX(config_FSName, 7, 16); // diskname

  String fcPath = convertToFilecorePath(csdPath);
  if (fcPath.length() > 2) {
    String dirName;
    int ptr = fcPath.length() - 2;
    dirName = fcPath.substring(ptr);
    while (!(dirName.startsWith(".")) && (ptr > -1)) {
      dirName = fcPath.substring(ptr);
      ptr--;
    }
    dirName = fcPath.substring(ptr + 2);
    if (dirName.endsWith(".")) dirName = dirName.substring(0, dirName.length() - 1);
    writeStringtoTX(dirName, 23, 10);
  } else {
    writeStringtoTX(fcPath, 23, 10);
  }

  fcPath = convertToFilecorePath(libPath);
  if (fcPath.length() > 2) {
    String dirName;
    int ptr = fcPath.length() - 2;
    dirName = fcPath.substring(ptr);
    while (!(dirName.startsWith(".")) && (ptr > -1)) {
      dirName = fcPath.substring(ptr);
      ptr--;
    }
    dirName = fcPath.substring(ptr + 2);
    if (dirName.endsWith(".")) dirName = dirName.substring(0, dirName.length() - 1);
    writeStringtoTX(dirName, 33, 10);
  } else {
    writeStringtoTX(fcPath, 33, 10);
  }

  if (txWithHandshake(43, txPort, 0x80)) Serial.println(F(".")); else Serial.println(F("!"));

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function 22 - Set boot options
/////////////////////////////////////////////////////////////////////

void fsSetBootOpts(byte txPort) {
  sdSelect();
  FatFile profileHdl;

  int usrHdl = fsGetUserHdl(rxBuff[3], rxBuff[2]);
  if (usrHdl == -1) {
    fsError(0xBF, F("Who are you?"), txPort);
    return;
  }

  // Stop Fixed and Locked users from changing their boot options
  if (userPriv[usrHdl]<0x41) {
    fsError(0xBA, F("Insufficient privilege"), txPort);
    return;
  }

  String user = getUsername(usrHdl);
  String userProfile = config_ProfileRoot;
  userProfile = userProfile + "/";
  userProfile = userProfile + user;
  userProfile.toCharArray(pathBuff1, DIRENTRYSIZE);
  byte newBootOpt = rxBuff[9];

  Serial.print(F("User "));
  Serial.print(user);
  Serial.print(F(" Profile "));
  Serial.print(userProfile);
  Serial.print(F(" New option "));
  Serial.print(newBootOpt);

  if (newBootOpt > 7) {
    fsError(0xBD, "Bad boot option", txPort);
    return;
  }


  if (!profileHdl.open(pathBuff1, O_WRITE)) {
    fsError(0xBC, "Unable to open user profile", txPort);
    return;
  }

  //TODO: Error checking here
  profileHdl.seekSet(1);
  profileHdl.write(newBootOpt);

  txBuff[0] = rxBuff[2];
  txBuff[1] = rxBuff[3];
  txBuff[2] = rxBuff[0];
  txBuff[3] = rxBuff[1];
  txBuff[4] = 0; // Command code 0
  txBuff[5] = 0; // Is successful

  if (txWithHandshake(6, txPort, 0x80)) Serial.println(F(".")); else Serial.println(F("!"));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function 23 - Log off
/////////////////////////////////////////////////////////////////////

void fsBye(byte txPort) {
  txBuff[0] = rxBuff[2];
  txBuff[1] = rxBuff[3];
  txBuff[2] = rxBuff[0];
  txBuff[3] = rxBuff[1];
  txBuff[4] = 0; // Command code 0
  txBuff[5] = 0; // Is successful

  if (fsGetUserHdl(rxBuff[3], rxBuff[2]) == -1) {
    fsError(0xBF, F("Who are you?"), txPort);
    return;
  }

  fsLogoff(rxBuff[3], rxBuff[2]);

  if (txWithHandshake(6, txPort, 0x80)) Serial.println(F(".")); else Serial.println(F("!"));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function 25 - Get File Server Version Information
/////////////////////////////////////////////////////////////////////

void fsInfo(int txPort) {
  txBuff[0] = rxBuff[2];
  txBuff[1] = rxBuff[3];
  txBuff[2] = config_Station;
  txBuff[3] = config_Net;

  txBuff[4] = 0x00;
  txBuff[5] = 0x00;

  String fsInfo = "ArduinoFS 0.01";

  fsInfo.toCharArray((char *)&txBuff[6], 15);

  if (txWithHandshake(18, txPort, 0x80)) Serial.println(F(".")); else Serial.println(F("!"));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function 26 - Read disk free
/////////////////////////////////////////////////////////////////////

void fsReaddiskFree(byte txPort) {

  if (fsGetUserHdl(rxBuff[3], rxBuff[2]) == -1) {
    fsError(0xBF, F("Who are you?"), txPort);
    return;
  }

  //TODO: Properly identify the disk - as there is only one it doesn't matter now.

  uint32_t sizeBytes = (sd.card()->cardSize() * 512);
  uint32_t freeBytes = ((512L * sd.vol()->blocksPerCluster()) * sd.vol()->freeClusterCount());

  txBuff[0] = rxBuff[2];
  txBuff[1] = rxBuff[3];
  txBuff[2] = rxBuff[0];
  txBuff[3] = rxBuff[1];
  txBuff[4] = 0; // Command code 0
  txBuff[5] = 0; // Is successful

  // Note Ecoconfig_Net sectors are 256 bytes big, so need to shift right...
  txBuff[6] = freeBytes >> 8;
  txBuff[7] = freeBytes >> 16;
  txBuff[8] = freeBytes >> 24;
  txBuff[9] = sizeBytes >> 8;
  txBuff[10] = sizeBytes >> 16;
  txBuff[11] = sizeBytes >> 24;


  if (txWithHandshake(12, txPort, 0x80)) Serial.println(F(".")); else Serial.println(F("!"));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function 27 - Create Directory
/////////////////////////////////////////////////////////////////////

void fsCdir(byte txPort, boolean oscli) {
  FatFile file;
  sdSelect(); // Make sure SD card is selected on the SPI bus

  int usrHdl = fsGetUserHdl(rxBuff[3], rxBuff[2]);
  if (usrHdl == -1) {
    fsError(0xBF, F("Who are you?"), txPort);
    return;
  }

  String workingDir;
  workingDir = getDirectoryPath(usrHdl, rxBuff[7]);
  String newDir;
  if (!oscli) newDir = getStringFromRX(10, DIRENTRYSIZE); else newDir = getStringFromRX(14, DIRENTRYSIZE);
  Serial.print(" User: ");
  Serial.print(usrHdl);
  Serial.print(" new directory: ");
  String fatPath = convertToFATPath(newDir, workingDir, txPort);
  Serial.print(fatPath);

  if (!fatPath.startsWith(getURD(usrHdl),0) && !isSystemUser(usrHdl)){
    // User does not own parent directory
    fsError(0xBD,"Insufficient Access",txPort);
    return; 
  } 

  fatPath.toCharArray(pathBuff1, DIRENTRYSIZE);

  if (sd.exists(pathBuff1)) {
    //Object not found - send error and leave
    Serial.print(" - ");

    //But first work out if it's a file or directory..

    if (file.open(pathBuff1, O_READ)) {
      if (file.isDir()) {
        fsError(0xB5, "Directory already exists", txPort); // May not be correct, but feels right
      } else {
        fsError(0xBD, "Cannot create - File already exists", txPort); // May not be correct, but feels right
      }
      file.close();
    } else {
      // File failed to open for examination!
      fsError(0xCC, "Bad directory name", txPort); // That should cover it!
    }
    return;
  }


  if (!sd.mkdir(pathBuff1)) {
    fsError(0xCC, "Bad directory name", txPort); // Meh, also not right.
    return;
  }

  // Now try and make a matching meta folder

  if (!createMetaFile(fatPath)) {
    fsError(0xFF, "Metafile creation failed", txPort);
    return;
  }

  Serial.print(F(", creation successful"));

  txBuff[0] = rxBuff[2];
  txBuff[1] = rxBuff[3];
  txBuff[2] = rxBuff[0];
  txBuff[3] = rxBuff[1];
  txBuff[4] = 0; // Command code
  txBuff[5] = 0; // Is successful

  if (txWithHandshake(6, txPort, 0x80)) Serial.println(F(".")); else Serial.println(F("!"));

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function 29 - Create file
/////////////////////////////////////////////////////////////////////

void fsCreateFile(int txPort) {
  FatFile file;
  dir_t dirEntry;
  sdSelect(); // Make sure SD card is selected on the SPI bus

  int usrHdl = fsGetUserHdl(rxBuff[3], rxBuff[2]);
  if (usrHdl == -1) {
    fsError(0xBF, F("Who are you?"), txPort);
    return;
  }

  String workingDir;
  workingDir = getDirectoryPath(usrHdl, rxBuff[7]);

  unsigned long loadaddr, execaddr, fsize;

  loadaddr = rxBuff[12];
  loadaddr = loadaddr << 8;
  loadaddr += rxBuff[11];
  loadaddr = loadaddr << 8;
  loadaddr += rxBuff[10];
  loadaddr = loadaddr << 8;
  loadaddr += rxBuff[9];

  execaddr = rxBuff[16];
  execaddr = execaddr << 8;
  execaddr += rxBuff[15];
  execaddr = execaddr << 8;
  execaddr += rxBuff[14];
  execaddr = execaddr << 8;
  execaddr += rxBuff[13];

  fsize = rxBuff[19];
  fsize = fsize << 8;
  fsize += rxBuff[18];
  fsize = fsize << 8;
  fsize += rxBuff[17];

  String objectName = getStringFromRX(20, 128);
  String fatPath = convertToFATPath(objectName, workingDir, txPort);

  Serial.print(F(" - user "));
  Serial.print(usrHdl);
  Serial.print(F(" dir handle "));
  Serial.print(rxBuff[7]);
  Serial.print(F(" working dir "));
  Serial.print(workingDir);
  Serial.print(F(" load address "));
  Serial.print(loadaddr, HEX);
  Serial.print(F(" exec address "));
  Serial.print(execaddr, HEX);
  Serial.print(F(" length "));
  Serial.print(fsize);
  Serial.print(F(" on object "));
  Serial.print(objectName);
  Serial.print(F(" ("));
  Serial.print(fatPath);
  Serial.print(F(")"));

  if (!hasUserAccess(usrHdl,fatPath,false)){
    // User does not have write access to file
    fsError(0xBD,"Insufficient Access",txPort);
    return; 
  } 

  fatPath.toCharArray(pathBuff1, DIRENTRYSIZE);

  if (sd.exists(pathBuff1)) {
    fsError(0xBD, "Already a file", txPort); // TODO: May not be correct, might have to delete existing file instead
    return;
  }

  if (fsize > 0) {
    if (!file.createContiguous(pathBuff1, fsize)) {
      // File create failed
      // TODO better error response
      fsError(0xBD, "File creation failed, because reasons", txPort);
      return;
    }
  } else {
    if (!file.open(pathBuff1, (O_RDWR | O_CREAT))) {
      // File create failed
      // TODO better error response
      fsError(0xBD, "Empty file creation failed, because reasons", txPort);
      return;
    }
  }

  file.close();

  //Now create the meta entry
  fatPath.replace(config_FSRoot, config_MetaRoot);
  fatPath.toCharArray(pathBuff2, DIRENTRYSIZE);

  if (!file.open(pathBuff2, (O_RDWR | O_CREAT))) {
    // File create failed
    // TODO better error response
    fsError(0xBD, "Metadata file creation failed, because reasons", txPort);
    return;
  }

  file.write(&rxBuff[9], 8); //Load and Exec address
  file.write(12); //Owner access only attributes - TODO: Default create attributes?
  file.close();


  // Return success and other info
  txBuff[0] = rxBuff[2];
  txBuff[1] = rxBuff[3];
  txBuff[2] = rxBuff[0];
  txBuff[3] = rxBuff[1];
  txBuff[4] = 0; // Command code
  txBuff[5] = 0; // Is successful
  txBuff[6] = 15; // File attributes TODO: might need to review this

  byte date0, date1;
  date0 = (year() - 1981) << 1 & 224;
  date0 += day();
  date1 = (year() - 1981) << 4;
  date1 += month();

  txBuff[7] = date0;
  txBuff[8] = date1;

  if (txWithHandshake(9, txPort, 0x80)) Serial.println(F(".")); else Serial.println(F("!"));

  return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function 30 - Read user free space
/////////////////////////////////////////////////////////////////////

void fsReadUserFree(byte txPort) {

  if (fsGetUserHdl(rxBuff[3], rxBuff[2]) == -1) {
    fsError(0xBF, F("Who are you?"), txPort);
    return;
  }

  //TODO: Properly identify the user, as just returning disk free until quotas sorted.

  uint32_t freeBytes = ((512L * sd.vol()->blocksPerCluster()) * sd.vol()->freeClusterCount());

  txBuff[0] = rxBuff[2];
  txBuff[1] = rxBuff[3];
  txBuff[2] = rxBuff[0];
  txBuff[3] = rxBuff[1];
  txBuff[4] = 0; // Command code 0
  txBuff[5] = 0; // Is successful

  txBuff[6] = freeBytes;
  txBuff[7] = freeBytes >> 8;
  txBuff[8] = freeBytes >> 16;
  txBuff[9] = freeBytes >> 24;

  if (txWithHandshake(10, txPort, 0x80)) Serial.println(F(".")); else Serial.println(F("!"));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function 32 - Read Client UserID
/////////////////////////////////////////////////////////////////////

void fsReadClientID(int txPort) {

  int usrHdl = fsGetUserHdl(rxBuff[3], rxBuff[2]);
  if (usrHdl == -1) {
    fsError(0xBF, F("Who are you?"), txPort);
    return;
  }

  txBuff[0] = rxBuff[2];
  txBuff[1] = rxBuff[3];
  txBuff[2] = config_Station;
  txBuff[3] = config_Net;
  txBuff[4] = 0x00;
  txBuff[5] = 0x00;

  int bufPtr = 6;

  String userName = getUsername(usrHdl);
  userName.toCharArray((char *)&txBuff[bufPtr], 22);
  bufPtr += userName.length();
  txBuff[bufPtr] = 0x0D; //Return character
  bufPtr++;

  Serial.print(F(", sending name"));

  if (txWithHandshake(bufPtr, txPort, 0x80)) Serial.println(F(".")); else Serial.println(F("!"));
}

/////////////////////////////////////////////////////////////////////
// End of FS functions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void fsError(byte errNo, String error, byte txPort) {
  Serial.print(F(" - Error 0x"));
  Serial.print(errNo, HEX);
  Serial.print(F(" - "));
  Serial.print(error);

  txBuff[0] = rxBuff[2];
  txBuff[1] = rxBuff[3];
  txBuff[2] = rxBuff[0];
  txBuff[3] = rxBuff[1];
  txBuff[4] = 0; // Command code 0
  txBuff[5] = errNo; // Is not successful
  error += "\r";
  writeStringtoTX(error, 6, (error.length() + 1));

  if (txWithHandshake(6 + error.length(), txPort, 0x80)) Serial.println(F(" - Error sent"));  else Serial.println(F(" - Failed to send error"));
}

void fsLogoff(byte config_Net, byte station) {
  boolean done = false;
  int userHdl = fsGetUserHdl(config_Net, station);
  if (userHdl == -1) return;

  fsCloseUserFiles(userHdl);
  stataddress[userHdl] = 0;
  netaddress[userHdl] = 0;
}

void fsCloseUserFiles(int usrHdl) {
  Serial.print(" closing any open files ");
  for (int ptr = 1; ptr < MAXFILES; ptr++) {
    if (fHandleUser[ptr] == usrHdl && fHandleActive[ptr]) {
      // Need to close file
      fHandleActive[ptr] = false;
      fHandleUser[ptr] = -1;
      if (fHandleIsFolder[ptr] == false) fHandle[ptr].close();
    }
  }
}

String convertToFATPath(String pathName, String basedir, byte errorPort) {
  String serverRoot = config_FSRoot;
  String result = pathName;
  if (pathName.startsWith(":")) {
    // Disk name specified, check it's correct then remove it
    String diskName = pathName.substring(1, pathName.indexOf("."));
    String fsdiskName = config_FSName;
    diskName.toUpperCase();
    fsdiskName.toUpperCase();

    if (diskName != fsdiskName) {
      fsError(0x54, F("disk not a fileserver disk"), errorPort);
      return ("");
    } else {
      result = pathName.substring((pathName.indexOf(".") + 1));
    };
  }

  //if (result=="$") return "/"; // If just the root left, no more work to do

  result.replace(".", "\xFF"); //Swap the dot and slash
  result.replace("/", ".");
  result.replace("\xFF", "/");

  result.replace("^", ".."); // Parent directory
  result.replace("$/", "/"); // Root directory  ($. has already been turned into $/)
  result.replace("$", "/");

  if (!(result.startsWith("/"))) {
    if (basedir.endsWith("/")) result = basedir + result; else result = basedir + "/" + result;
  }

  //clean up any parent requests, as sdfat does not translate them

  while (result.indexOf("/..") != -1) {
    String parentDir;
    String remainPath = "";
    int parentSlash = 0;

    int parentReq = result.indexOf("/..");

    if (parentReq < 2) {
      // Trying to change to parent of root, stop here
      pathName = "/";
    } else {
      parentSlash = parentReq;
      while (parentSlash > 0) {
        parentDir = result.substring(0, parentSlash);
        parentSlash--;
        if (parentDir.endsWith("/")) parentSlash = 0; //Break loop
      }

      if (result.length() > parentReq + 4) remainPath = result.substring(parentReq + 4);
      result = parentDir + remainPath;
    }
  }  // End of while result contains /..

  // Check ServerRoot is present
  if (!result.startsWith(serverRoot)) result = serverRoot + result;

  return result;
}

int getEconetDate(dir_t* dirEntry) {
  int ecoDate;
  ecoDate = (FAT_YEAR(dirEntry->lastWriteDate) - 1981) << 1 & 224;
  ecoDate += FAT_DAY(dirEntry->lastWriteDate);
  ecoDate += (FAT_YEAR(dirEntry->lastWriteDate) - 1981) << 12;
  ecoDate += FAT_MONTH(dirEntry->lastWriteDate) << 8;

}

String econetDateToString(int ecoDate){

  String result="01/01/81"; // Set to Econet epoch, returned if passed invalid date
  
  if (ecoDate>0){
    int mnthday=(ecoDate & 31);
    int mnth = (ecoDate & 3840)>>8;
    int yr=ecoDate>>12;
    yr+=(ecoDate &224)>1;
    yr+=1981;
    String ds=(String)mnthday;
    if (ds.length()==1) ds="0"+ds; // Make day 2 digits if necessary
    String ms=(String)mnth;
    if (ms.length()==1) ms="0"+ms; // Make month 2 digits if necessary
    String yrs=(String)yr;
    yrs=yrs.substring(yrs.length()-2); // reduce year to 2 digits
    result=ds+"/"+ms+"/"+yrs;
  }
  
  return(result);
}

boolean createMetaFile(String path) {
  SdFile file;

  path.toCharArray(pathBuff1, DIRENTRYSIZE);

  if (!file.open(pathBuff1, O_READ)) {
    // File open failed - return false
    return  (false);
  }

  boolean isDir = file.isDir();

  file.close();

  // Get defaults for the file or folder
  unsigned long loadAddress = getLoadAddressForObject(pathBuff1);
  unsigned long execAddress = getExecAddressForObject(pathBuff1);
  byte attr = getAttributes(pathBuff1);

  path.replace(config_FSRoot, config_MetaRoot);
  path.toCharArray(pathBuff2, DIRENTRYSIZE);

  if (isDir) {
    // dealing with a directory

    // Create meta folder if does not exist
    if (!sd.exists(pathBuff2)) {
      if (!sd.mkdir(pathBuff2)) {
        return (false);
      }
    }
    // Now build the path to the directory meta folder
    String metaName = path + "/$";
    metaName.toCharArray(pathBuff2, DIRENTRYSIZE);

    Serial.print("(" + metaName + ")");
    if (file.open(pathBuff2, O_CREAT | O_WRITE)) {
      file.write(&loadAddress, 4);
      file.write(&execAddress, 4);
      file.write(attr);
      file.close();
    } else {
      // File create failed
      return (false);
    }
  } else {
    // dealing with a file
    if (file.open(pathBuff2, O_CREAT | O_WRITE)) {
      file.write(&loadAddress, 4);
      file.write(&execAddress, 4);
      file.write(attr);
      file.close();
    } else {
      // File create failed
      return (false);
    }
  }

  return (true);
}

byte getAttributes(const char* objectName) {
  FatFile file;
  FatFile meta;
  byte attribute = 0;


  if (file.open(objectName, O_READ)) {
    if (file.isDir()) {
      // Object is a folder
      String metaName = objectName + (String)"/$";
      metaName.replace(config_FSRoot, config_MetaRoot);
      metaName.toCharArray(pathBuff3, DIRENTRYSIZE);

      if (meta.open(pathBuff3, O_READ)) {
        meta.seekSet(8);
        attribute = meta.read();
        meta.close();
      } else {
        // No meta file (or error opening it)
        attribute = 32; //Return directory
      }

    } else {
      //Object is a file - look for metadata file
      String metaName = objectName;
      metaName.replace(config_FSRoot, config_MetaRoot);
      metaName.toCharArray(pathBuff3, DIRENTRYSIZE);

      if (meta.open(pathBuff3, O_READ)) {
        meta.seekSet(8);
        attribute = meta.read();
        meta.close();
      } else {
        // No meta file (or error opening it)
        attribute = 12; //Default to owner read and write access
      }
    }
    file.close();
  } else {
      // File or directory failed to open, probably doesn't exist
      attribute=12; //Default to owner read and write access
  }
  return attribute;
}

unsigned long getExecAddressForObject(const char* objectName) {
  FatFile file;
  FatFile meta;
  dir_t dirEntry;

  unsigned long execAddress = 0;

  if (file.open(objectName, O_READ)) {
    if (file.isDir()) {
      String metaName = objectName + (String)"/$";
      metaName.replace(config_FSRoot, config_MetaRoot);
      metaName.toCharArray(pathBuff3, DIRENTRYSIZE);

      if (meta.open(pathBuff3, O_READ)) {
        meta.seekSet(4);
        meta.read(&execAddress, 4);
        meta.close();
      } else {
        // No metadata file, or failed to open - create data from timestamp
        file.dirEntry(&dirEntry);
        tmElements_t tm;
        tm.Year = (FAT_YEAR(dirEntry.lastWriteDate)) - 1970;
        tm.Month = FAT_MONTH(dirEntry.lastWriteDate);
        tm.Day = FAT_DAY(dirEntry.lastWriteDate);
        tm.Hour = FAT_HOUR(dirEntry.lastWriteTime);
        tm.Minute = FAT_MINUTE(dirEntry.lastWriteTime);
        tm.Second = FAT_SECOND(dirEntry.lastWriteTime);
        unsigned long fileTime = makeTime(tm);

        uint64_t roDate, multiplier; // Risc OS date is centiseconds since 1900, filetime is seconds since 1970
        multiplier = 100;
        roDate = 220898880000 + (fileTime * multiplier);

        execAddress = roDate;
      }

    } else {
      //Object is a file - look for metadata file
      String metaName = objectName;
      metaName.replace(config_FSRoot, config_MetaRoot);
      metaName.toCharArray(pathBuff3, DIRENTRYSIZE);

      if (meta.open(pathBuff3, O_READ)) {
        meta.seekSet(4);
        meta.read(&execAddress, 4);
        meta.close();
      }
    }
    file.close();
  } // return from open file. If file didn't open returning 0 is sensible

  return execAddress;
}

unsigned long getLoadAddressForObject(const char* objectName) {
  FatFile file;
  FatFile meta;
  dir_t dirEntry;

  unsigned long loadAddress = 0;

  if (file.open(objectName, O_READ)) {
    if (file.isDir()) {
      // Open metadata if available
      String metaName = objectName + (String)"/$";
      metaName.replace(config_FSRoot, config_MetaRoot);
      metaName.toCharArray(pathBuff3, DIRENTRYSIZE);

      if (meta.open(pathBuff3, O_READ)) {
        meta.read(&loadAddress, 4);
        meta.close();
      } else {
        // Unable to open metadata -  create data from timestamp

        file.dirEntry(&dirEntry);
        tmElements_t tm;
        tm.Year = (FAT_YEAR(dirEntry.lastWriteDate)) - 1970;
        tm.Month = FAT_MONTH(dirEntry.lastWriteDate);
        tm.Day = FAT_DAY(dirEntry.lastWriteDate);
        tm.Hour = FAT_HOUR(dirEntry.lastWriteTime);
        tm.Minute = FAT_MINUTE(dirEntry.lastWriteTime);
        tm.Second = FAT_SECOND(dirEntry.lastWriteTime);
        unsigned long fileTime = makeTime(tm);

        uint64_t roDate, multiplier; // Risc OS date is centiseconds since 1900, filetime is seconds since 1970
        multiplier = 100;
        roDate = 220898880000 + (fileTime * multiplier);

        loadAddress = roDate >> 32;

        //TODO: Put the right filetype in here, currently setting to FFD (Data)

        loadAddress += (0xFD << 8);
        loadAddress += (0xFF << 16);
        loadAddress += (0xFF << 24);
      }
    } else {
      //Object is a file - look for metadata file
      String metaName = objectName;
      metaName.replace(config_FSRoot, config_MetaRoot);
      metaName.toCharArray(pathBuff3, DIRENTRYSIZE);

      if (meta.open(pathBuff3, O_READ)) {
        meta.read(&loadAddress, 4);
        meta.close();
      }
    }
    file.close();
  } // return from open file. If file didn't open returning 0 is sensible

  return loadAddress;
}

String convertToFilecorePath(String pathName) {
  String result = pathName;

  result.replace(config_FSRoot, ""); //Remove the prefix TODO: Probably should be smarter about this

  if (pathName == "/") return "$"; // If just the root left, no more work to do

  result.replace("/", "\xFF"); //Swap the dot and slash
  result.replace(".", "/");
  result.replace("\xFF", ".");

  result = "$" + result;

  return result;
}

int fsGetUserHdl(byte config_Net, byte station) {
  for (int ptr = 0; ptr < MAXUSERS; ptr++) {
    if (stataddress[ptr] == station) {
      if (netaddress[ptr] == config_Net) {
        return ptr;
      }
    }
  }
  return (-1);
}

String readBuff(char *buff, int start, int limit) {
  String output = "";
  int ptr = 0;
  char thisbyte;
  buff += start;

  do {
    thisbyte = *buff;
    output += thisbyte;
    ptr++;
    buff++;
  } while (thisbyte != 0 && ptr < limit);

  return output;
}

boolean setURD(int uhdl, String newpath) {
  char* ptr = NULL;
  char path[DIRENTRYSIZE + 1];

  if (uhdl > MAXUSERS) return false;
  if (newpath.length() > DIRENTRYSIZE) return false;

  ptr = &userURD[0];
  ptr += (uhdl * DIRENTRYSIZE);

  newpath.toCharArray(path, DIRENTRYSIZE);
  strcpy(ptr, path);

  return (true);
}

String getURD(int uhdl) {
  char * ptr = NULL;
  if (uhdl > MAXUSERS) return "";

  ptr = &userURD[0];
  ptr += (uhdl * DIRENTRYSIZE);

  return String(ptr);
}

String getUsername(int uhdl) {
  char * ptr = NULL;
  if (uhdl > MAXUSERS) return "";

  ptr = &userName[0];
  ptr += (uhdl * 22);

  return String(ptr);
}

byte getUserLibhandle(int user) {
  return (userLibhdl[user]);
}

byte getUserCSDhandle(int user) {
  return (userCSDhdl[user]);
}

byte getUserURDhandle(int user) {
  return (255);
}

void writeStringtoTX(String data, int txLocation, int maxBytes) {
  while (data.length() < maxBytes) {
    data += " ";
  }; // Pad any undersized strings

  data.getBytes(workBuff, maxBytes + 1);
  for (int i = 0; i < maxBytes; i++) {
    txBuff[i + txLocation] = workBuff[i];
  }
}

String getStringFromRX(int startLocation, int maxLength) {
  String result = "";
  maxLength += startLocation;
  int thisByte = startLocation;
  while (thisByte < maxLength) {
    if (rxBuff[thisByte] == 0 || rxBuff[thisByte] == 13 || rxBuff[thisByte] == 32) break;
    result += (char)rxBuff[thisByte];
    thisByte++;
  }
  return result;
}

String getDirectoryPath(byte usrHdl, byte dirHdl) {

  String workingDir;

  if (dirHdl == 255) {
    // URD requested
    return (getURD(usrHdl));
  }

  if (fHandleActive[dirHdl]) {
    workingDir = getFilePath(dirHdl);
  } else {
    // Inactive filehandle, return to root
    workingDir = config_FSRoot;
  }

  return workingDir;
}

boolean setfHandlePath(int fhdl, String newpath) {
  char* ptr = NULL;
  char path[DIRENTRYSIZE + 1];

  if (fhdl > MAXFILES) return false;
  if (newpath.length() > DIRENTRYSIZE) return false;

  ptr = &fHandlePath[0];
  ptr += (fhdl * DIRENTRYSIZE);

  newpath.toCharArray(path, DIRENTRYSIZE);
  strcpy(ptr, path);

  return (true);
}

int openFolder(int usrHdl, String path) {
  int fHdl = 1;
  while (fHandleActive[fHdl] && fHdl < MAXFILES) {
    fHdl++;
  };
  if (fHdl >= MAXFILES) {
    // Run out of file handles
    //fsError(0xc0,"Too many open files ",txPort);
    return (0);
  }

  if (userOpenFiles[usrHdl] >= MAXUSERFILES) {
    // Run out of file handles
    //fsError(0xc0,"Too many open files ",txPort);
    return (0);
  }

  fHandleActive[fHdl] = true;
  fHandleUser[fHdl] = usrHdl;
  fHandleIsFolder[fHdl] = true;
  setfHandlePath(fHdl, path);
  userOpenFiles[usrHdl]++;

  return (fHdl);
}

boolean closeFolder(int fHdl) {

  if (fHandleActive[fHdl] && fHandleIsFolder[fHdl]) {
    // Close the folder
    byte usrHdl = fHandleUser[fHdl];
    userOpenFiles[usrHdl]--;
    fHandleActive[fHdl] = false;
  }

}

String getFilePath(int fHdl) {
  char* ptr = NULL;

  if (!fHandleActive[fHdl]) return (""); // Inactive filehandle passed

  ptr = &fHandlePath[0];
  ptr += (fHdl * DIRENTRYSIZE);

  String result = String(ptr);
  return result;
}

boolean isSystemUser(int usrHdl) {
    if (userPriv[usrHdl] == 255) return (true); else return (false);
}

String readConfigValue(String key) {
  FatFile handle;
  String configFile = config_confRoot + "/" + key;
  //Serial.print("Reading from " + configFile + ",");
  configFile.toCharArray(pathBuff1, DIRENTRYSIZE);
  if (handle.open(pathBuff1, (O_READ))) {
    int bytesRead = handle.read(&workBuff, 255);
    handle.close();

    if (bytesRead == 0) return ("");
    workBuff[bytesRead]=0; // Mark the end of string

    String value = String((char *)&workBuff);

    //Serial.println(" got value '" + value + "' (" + (String)bytesRead + " bytes)");
    return (value);

  } else {
    //Serial.println(" file failed to open");
    return ("");
  }
}

int readConfigValueAsInt(String key) {
  FatFile handle;
  int result;
  String configFile = config_confRoot + "/" + key;
  //Serial.print("Reading from " + configFile + ",");
  configFile.toCharArray(pathBuff1, DIRENTRYSIZE);
  if (handle.open(pathBuff1, (O_READ))) {
    int bytesRead = handle.read(&workBuff, 255);
    handle.close();

    if (bytesRead == 0) return (0);
    workBuff[bytesRead]=0; // Mark the end of string

    String value = String((char *)&workBuff);
    result=value.toInt();
    return (result);

  } else {
    //Serial.println(" file failed to open");
    return (0);
  }
}

void writeConfigValue(String key, String value) {
  FatFile handle;
  String configFile = config_confRoot + "/" + key;
  configFile.toCharArray(pathBuff1, DIRENTRYSIZE);
  value.toCharArray(confBuff,255);

  if (handle.open(pathBuff1, (O_RDWR | O_CREAT))) {
    Serial.print("Writing " + value + " to " + configFile);
    handle.write(&confBuff,value.length());
    handle.close();
    Serial.println(" done");
  } else {
    Serial.println("Failed to open / create config value " + configFile);
  }
}

boolean hasUserAccess(int usrHdl, String file, boolean readOnly){
  if (isSystemUser(usrHdl)==true) return true; // System user, no point checking further.
  file.toCharArray(pathBuff1, DIRENTRYSIZE);
  int fileAttr=getAttributes(pathBuff1);


  if (file.startsWith(getURD(usrHdl),0)){
    // User is file owner
    if (readOnly && (fileAttr & 4) ) return true; // File has owner read
    if ((!readOnly) && (fileAttr & 8) ) return true; // File has owner write   
    if (fileAttr & 32 ) return true; // It's a directory
  } else {
    // Not file owner
    if (readOnly && (fileAttr & 1) ) return true; // File has public read
    if ((!readOnly) && (fileAttr & 2) ) return true; // File has public write    
    if (readOnly && (fileAttr & 32) ) return true; // It's a directory
  }

  return (false);  
}

boolean isObjectLocked(String file){
  file.toCharArray(pathBuff1, DIRENTRYSIZE);
  int fileAttr=getAttributes(pathBuff1);
  if (fileAttr && (fileAttr & 16)) return true;
  return false;
}
