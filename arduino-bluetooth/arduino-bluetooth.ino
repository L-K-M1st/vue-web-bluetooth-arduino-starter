#include <SoftwareSerial.h>
#include "BluetoothSerial.h"

SoftwareSerial BluetoothDevice(3,4);
void onMessageReceived(void){}
// &BluetoothDevice is dependency injection and composition. myBluetooth has a BluetoothDevice.
// onMessageReceived is a callback function.
Bluetooth myBluetooth(&BluetoothDevice, onMessageReceived);

void setup() {
  Serial.begin(115200); 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  myBluetooth.initialize();
  delay(1000);
}

void loop() {
  myBluetooth.receive();
  myBluetooth.useNewData();
  delay(1000);
}


