/*
Code to send “Are you there?” from one Arduino to another Arduino using RLYR999 Module
and receive reply and display complete communication on I2C LCD by platwithcircuit.com
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <LiquidCrystal_I2C.h>

#define REPLY_TIMEOUT_IN_MS 300
#define REPLY_END_CHAR '\n'
#define MODULE_ADDRESS 0
#define RECEIVERS_ADDRESS 1
#define MIN_CHAR_TO_RCV 1
#define WAIT_FOR_RECIVERS_REPLY 3000
#define DELAY_BW_REPS 1000

// Init LCD at 0x27, 16x2
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  boolean boRetVal = false;
  // begin serial commmunicatio at baud 115200,n,8,1
  // to comunicate with the RF module
  Serial.begin(115200);
  // initialize the LCD
  lcd.init();
  // Turn ON the Backlight
  lcd.backlight();
  // Clear the display buffer
  lcd.clear();

  flushBuffer();  // clear rx data

  // Reset settings to factory defaults
  boRetVal = boRestoreFactoryDefaults();

  // setting the address if reset successfull
  if (boRetVal == true) {
    flushBuffer();  // clear rx data
    boRetVal = boSetAddress();
  }

  if (boRetVal == true) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Module Init");
    lcd.setCursor(0, 1);
    lcd.print("Successful");
    delay(1000);
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Module Init");
    lcd.setCursor(0, 1);
    lcd.print("Failed");
    while (1)
      ;
  }
}

void loop() {
  String request = "Are you there?";
  String expected_reply = "Yes";
  bool boRetVal = false;

  flushBuffer();  // clear rx data
  // tranmits String named request
  boRetVal = boSendData(request);
  if (boRetVal == true) {
    // Displaying Sent Msg
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Msg Sent:");
    lcd.setCursor(0, 1);
    lcd.print(request);
    delay(1000);
    boRetVal = chkReply(expected_reply, REPLY_END_CHAR, WAIT_FOR_RECIVERS_REPLY);

    if (boRetVal == true) {
      // Displaying received Msg
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Msg Received:");
      lcd.setCursor(0, 1);
      lcd.print(expected_reply);
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("No reply received.");
    }
  } else {
    // Displaying Failed Msg
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Msg Sending");
    lcd.setCursor(0, 1);
    lcd.print("Failed");
  }

  delay(DELAY_BW_REPS);  // wait before sending again
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