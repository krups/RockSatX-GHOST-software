/* Thermocouple thread measurement test
 *  
 * Like TcTest but with measurement in a timesliced task using TeensyThreads
 * 
 * see the following link regarding sending structs
 * https://arduino.stackexchange.com/questions/45066/radiohead-library-nrf24-sending-and-receiving-struct-data-problem
 */

// radio logger node class needs this defined
#define NODE_ADDRESS 2

#include <IridiumSBD.h>
#include <TeensyThreads.h>
#include "src/TcInterface/TcInterface.h"
#include "src/RadioLogger/RadioLoggerNode.h"

#define LED_IR_ON    3
#define LED_IR_SIG   4
#define LED_IR_TX    5
#define LED_ISM_TX   6
#define LED_ACT      7

#define PIN_IR_ENABLE 23

RadioLoggerNode logNode;
char types[8] = "KKKKKKKK";
TcInterface tc(types);

Threads::Mutex spi_lock;
Threads::Mutex ser_lock;
Threads::Mutex ircmd_lock;

struct tc_reading {
  float data;
  bool valid;
};

volatile tc_reading tcVals[8];


////////////////////////////////////
//           IRIDIUM VARS
////////////////////////////////////
#define IridiumSerial Serial4
#define DIAGNOSTICS false// Change this to see diagnostics
// Declare the IridiumSBD object
IridiumSBD modem(IridiumSerial);
int signalQuality = -1;
int irerr;
int ircmd = 0;
// command variables
#define IR_SEND_TEST 1
// #define IR_SEND_PACKET etc. 
uint8_t signalBrightness[6] = {0, 2, 10, 50, 100, 250};


void safePrint(String s) {
  ser_lock.lock();
  Serial.print(s);
  ser_lock.unlock();
}

void safePrintln(String s) {
  ser_lock.lock();
  Serial.println(s);
  ser_lock.unlock();
}

void tc_thread(int inc) {

  safePrintln("TC thread starting");

  spi_lock.lock();
  if( tc.enable() ){
    safePrintln("TC INIT OK");
  } else {
    safePrintln("TC INIT FAIL!!");
  }
  spi_lock.unlock();

  float vals[8] = {0,0,0,0,0,0,0,0};
  unsigned long lastData = millis();

  safePrintln("HERE");
  
  while(1) {

    spi_lock.lock();
    bool forceStart = millis() - lastData > 5000 ? true : false;
    if( forceStart ) lastData = millis();
    bool gotData = tc.read_all(vals, forceStart);
    spi_lock.unlock();

    if( gotData ){
      digitalWrite(LED_ACT, HIGH);
      
      lastData = millis();
      for( int i=0; i<8; i++ ){
        tcVals[i].data = vals[i];
        safePrint(vals[i]); safePrint(", ");
      }
      safePrint("\r\n");
      digitalWrite(LED_ACT, LOW);
    } else {
      
    }
    threads.delay(100);
  }
}

void radio_thread(int inc) {
  
  spi_lock.lock();
  if( logNode.begin() ){
    safePrintln("log node started");
    analogWrite(LED_ISM_TX, 5);
    logNode.setRetries(0);
  } else {
    safePrintln("log node failed to start");
  }
  spi_lock.unlock();

  while(1) {
    threads.delay(5000);
    String data;
    for( int i=0; i<8; i++ ){
      data += " ";
      data += tcVals[i].data;
    } data += "\n";
    
    spi_lock.lock();
    analogWrite(LED_ISM_TX, 100);
    safePrintln("about to log");
    logNode.log(data);
    analogWrite(LED_ISM_TX, 5);
    spi_lock.unlock();
  }
}


void iridium_thread(int inc) {

  // Start the serial port connected to the satellite modem
  IridiumSerial.begin(9600);

  safePrint("Powering on modem...");
  digitalWrite(PIN_IR_ENABLE, HIGH);
  analogWrite(LED_IR_ON, 5); // not blinding
  threads.delay(2000);
  safePrintln("done.");


  // Begin satellite modem operation
  safePrintln("Starting modem...");
  irerr = modem.begin();
  if (irerr != ISBD_SUCCESS)
  {
    safePrint("Begin failed: error ");
    safePrintln(irerr);
    if (irerr == ISBD_NO_MODEM_DETECTED)
      safePrintln("No modem detected: check wiring.");
    return;
  }

  // Test the signal quality.
  // This returns a number between 0 and 5.
  // 2 or better is preferred.
  irerr = modem.getSignalQuality(signalQuality);
  if (irerr != ISBD_SUCCESS)
  {
    safePrint("SignalQuality failed: error ");
    safePrintln(irerr);
    //TODO: error handling
    //return;
  }
  safePrint("On a scale of 0 to 5, signal quality is currently ");
  safePrint(signalQuality);
  if( signalQuality >= 0 && signalQuality <= 5 ){
    analogWrite(LED_IR_SIG, signalBrightness[signalQuality]);
  }
  safePrintln(".");

  int cmd = 0;

  unsigned long signalCheckInterval = 15000;
  unsigned long lastSignalCheck = millis();
  
  while(1) {
    threads.delay(50);

    // check for a command set in the command variable
    ircmd_lock.lock();
    if( ircmd != 0){
      cmd = ircmd;
      ircmd = 0;
    }
    ircmd_lock.unlock();

    /////////////////////
    // PROCESS TEST SEND COMMAND
    /////////////////////
    if( cmd == IR_SEND_TEST ){
      // Send the message
      safePrintln("* sending test iridium message. This might take several minutes.\r\n");
      analogWrite(LED_IR_TX, 20);
      irerr = modem.sendSBDText("Hello, world!");
      analogWrite(LED_IR_TX, 0);
      if (irerr != ISBD_SUCCESS)
      {
        safePrint("sendSBDText failed: error ");
        safePrintln(irerr);
        if (irerr == ISBD_SENDRECEIVE_TIMEOUT)
          safePrintln("Try again with a better view of the sky.");
      }
    
      else
      {
        safePrintln("*****************");
        safePrintln("*** MESSAGE SENT *");
        safePrintln("*****************");

        // clear command flag on success
        cmd = 0;
      }
    }

    ///////////////////
    // CHECK THE SINGL QUALITY PERIODICALLY
    ////////////////////
    if( lastSignalCheck - millis() > signalCheckInterval ){
     irerr = modem.getSignalQuality(signalQuality);
      if (irerr != ISBD_SUCCESS)
      {
        safePrint("SignalQuality failed: error ");
        safePrintln(irerr);
        //TODO: error handling
        //return;
      }
      safePrint("On a scale of 0 to 5, signal quality is currently ");
      safePrint(signalQuality);
      safePrintln(".");
      if( signalQuality >= 0 && signalQuality <= 5 ){
        analogWrite(LED_IR_SIG, signalBrightness[signalQuality]);
      }
      lastSignalCheck = millis();
    }
    
  }
}


void setup() {
  Serial.begin(115200);

  pinMode(LED_IR_ON, OUTPUT);
  pinMode(LED_IR_SIG, OUTPUT);
  pinMode(LED_IR_TX, OUTPUT);
  pinMode(LED_ISM_TX, OUTPUT);
  pinMode(LED_ACT, OUTPUT);

  digitalWrite(PIN_IR_ENABLE, LOW);
  pinMode(PIN_IR_ENABLE, OUTPUT);

  delay(2000);

  threads.addThread(tc_thread, 1);
  threads.addThread(radio_thread, 1);

  safePrintln("* starting iridium thread");
  threads.addThread(iridium_thread, 1);
  
  //threads.setSliceMicros(500);
}

void loop() {
  // put your main code here, to run repeatedly:

  threads.delay(1000);

  ser_lock.lock();
  char a = Serial.read();
  ser_lock.unlock();

  if( a == '1' ){
    ircmd_lock.lock();
    ircmd = 1;
    ircmd_lock.unlock();
  }
  
}