/*
Code to receieve “Are you there?” from one Arduino to another Arduino using RLYR999 Module
and send "Yes" in reply by platwithcircuit.com
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <LiquidCrystal_I2C.h>

#define REPLY_TIMEOUT_IN_MS 300
#define REPLY_END_CHAR '\n'
#define MODULE_ADDRESS 1
#define RECEIVERS_ADDRESS 0
#define MIN_CHAR_TO_RCV 1
#define WAIT_FOR_REQUEST 3000

void setup() {
  boolean boRetVal = false;
  // begin serial commmunicatio at baud 115200,n,8,1
  // to comunicate with the RF module
  Serial.begin(115200);

  delay(1000);

  flushBuffer();  // clear rx data

  // Reset settings to factory defaults
  boRetVal = boRestoreFactoryDefaults();

  // setting the address if reset successfull
  if (boRetVal == true) {
    flushBuffer();  // clear rx data
    boRetVal = boSetAddress();
  }

  if (boRetVal != true) {
    Serial.print("\n\rModule Init");
    Serial.print("Failed");
    while (1)
      ;
  }
}

void loop() {
  String request = "Are you there?";
  String expected_reply = "Yes";
  bool boRetVal = false;

  // check string sent by Initiator
  boRetVal = chkReply(request, REPLY_END_CHAR, WAIT_FOR_REQUEST);

  if (boRetVal == true) {
    boSendData(expected_reply);
  }
}

void sendCrLf(void) {
  Serial.write(0x0D);  // Carriage Return
  Serial.write(0x0A);  // Line Feed
}

void flushBuffer(void) {
  while (Serial.available() > 0) {
    Serial.read();
  }
}

bool chkReply(String chkString, char receiveUntil, unsigned int timeout) {
  String receivedString;       // save received data in this string object
  bool boReturnValue = false;  // function's return value

  // wait for reply
  do {
    timeout--;
    delay(1);  // delay of 1 ms
  } while ((Serial.available() < MIN_CHAR_TO_RCV) && (timeout > 0));

  if (timeout) {
    // if timeout is left then a reply is received check for the string in the reply
    receivedString = Serial.readStringUntil(receiveUntil);
    if (receivedString.indexOf(chkString) != -1) {
      boReturnValue = true;
    } else {
      boReturnValue = false;
    }
  } else {
    boReturnValue = false;
  }

  // return result
  return boReturnValue;
}

bool boRestoreFactoryDefaults(void) {
  const char factoryDefaultCmd[] = "AT+FACTORY";  // command to be sent
  bool boReturnValue = false;                     // function's return value
  char downCounter = 100;                         // Down counter to wait for reply
  String receivedString;                          // save received data in this string object

  String chkRcvString1 = "+FACTORY";
  String chkRcvString2 = "+READY";

  // send command
  Serial.print(factoryDefaultCmd);
  sendCrLf();

  // check first string in reply
  boReturnValue = chkReply(chkRcvString1, REPLY_END_CHAR, REPLY_TIMEOUT_IN_MS);
  if (boReturnValue == true) {
    // check second string in reply
    boReturnValue = chkReply(chkRcvString2, REPLY_END_CHAR, REPLY_TIMEOUT_IN_MS);
  }

  // return result
  return boReturnValue;
}

bool boSetAddress(void) {
  const char setAddressCmd[] = "AT+ADDRESS=";  // command to be sent
  bool boReturnValue = false;                  // function's return value
  String chkRcvString = "+OK";

  // send command
  Serial.print(setAddressCmd);
  Serial.print(MODULE_ADDRESS);
  sendCrLf();

  // check reply
  boReturnValue = chkReply(chkRcvString, REPLY_END_CHAR, REPLY_TIMEOUT_IN_MS);

  // return result
  return boReturnValue;
}

bool boSendData(String data) {
  const char sendDataCmd[] = "AT+SEND=";  // command to be sent
  bool boReturnValue = false;             // function's return value
  String chkRcvString = "+OK";

  // send command
  Serial.print(sendDataCmd);
  Serial.print(RECEIVERS_ADDRESS);
  Serial.print(',');
  Serial.print(data.length());
  Serial.print(',');
  Serial.print(data);
  sendCrLf();

  // check reply
  boReturnValue = chkReply(chkRcvString, REPLY_END_CHAR, REPLY_TIMEOUT_IN_MS);

  // return result
  return boReturnValue;
}