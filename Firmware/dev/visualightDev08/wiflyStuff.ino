void joinWifi(){
  /* Setup the WiFly to connect to a wifi network */
  if(DEBUG) Serial.println(F("From joinWifi"));
  wifly.reboot();
  //if (!wifly.isAssociated()) {

  //wifly.startCommand();
  //wifly.setSpaceReplace('+');

  wifly.setSSID(network);
  wifly.setPassphrase(password);
  wifly.setJoin(WIFLY_WLAN_JOIN_AUTO);
  //wifly.join();


  //wifly.setIpProtocol(WIFLY_PROTOCOL_TCP);
  wifly.save();
  //wifly.finishCommand();
  wifly.reboot();
  //delay(1000);

  //}

  if(!wifly.isAssociated()){
    if(DEBUG) Serial.println(F("Joining network"));
    if (wifly.join()) {
      if(DEBUG) Serial.println(F("Joined wifi network"));
      connectToServer();
    } 
    else {
      if(DEBUG) Serial.println(F("Failed to join wifi network"));
      delay(1000);
      joinWifi();
      //TODO: Reboot count and reset to AP after count expires
    }
  }
  else{
    //Serial.println(F("Already Joined!"));
    colorLED(255,255,255);
    if(isServer){
      if(DEBUG) Serial.println(F("Switch to Client Mode"));
      EEPROM.write(0, 0); 
      isServer = false;
      //colorLED(0,0,255);
    }
    connectToServer();
  }
}

void connectToServer(){
  wifly.flushRx();
  //MAC = wifly.getMAC(buf, sizeof(buf));
  if(!wifly.isAssociated()) { //|| 
      if(DEBUG) Serial.println(F("Joining"));
      //Serial.println(wifly.isAssociated());
      if(!wifly.join()){
        if(DEBUG) Serial.println("Join Failed");
        if(DEBUG) Serial.println(wifly.isAssociated());
      }
      //else{
      //wifiTimer = millis();
      //}
    }
    else{
      if(DEBUG) Serial.println("Already Assoc");
    }
  if (wifly.isConnected()) {
    //Serial.println("Old connection active. Closing");
    wifly.close();
  }
  if(DEBUG) Serial.println("Connecting");
  if (wifly.open("dev.visualight.org",5001)) {
  //if (wifly.open("10.0.1.39",5001)) {
    colorLED(255,255,255); // white is connected
    if(DEBUG) Serial.println("Connected");
    //Serial.println("Connected: ");
    //Serial.print(F("Free memory: "));
    //Serial.println(wifly.getFreeMemory(),DEC);
    //Serial.flush();
    //Serial1.flush();
    //Serial.println(MAC);
    //wifly.flush();
    wifly.write(MAC);
    connectTime = millis();
  } 
  else {
    if(DEBUG) Serial.println("Failed to open");
    //wifly.connected = false;
    //delay(1000);

  }


}
void wifiReset(){
  wifly.close();
  if(DEBUG) Serial.println("Wifi RESET");
  colorLED(255,0,0);
  isServer = true;
  EEPROM.write(0, 1);
  wifly.reboot();
  wifly.setSoftAP();
}
void configureWifi(){
  if(DEBUG) Serial.println("From Config");
  wifly.setBroadcastInterval(0);	// Turn off UPD broadcast
  wifly.setDeviceID("Visualight");
  wifly.setProtocol(WIFLY_PROTOCOL_TCP);
  wifly.enableDHCP();
  wifly.setChannel("0");
  wifly.save();
  wifly.reboot();
}

