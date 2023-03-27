#include <SoftwareSerial.h>

SoftwareSerial BluetoothDevice(2,3);

const byte numChars = 32;
char receivedChars[numChars];
boolean newData = false;

void setup() {
  Serial.begin(115200); 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  BluetoothDevice.begin(115200);
    while (!BluetoothDevice) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("setup");
  delay(1000);
}

void loop() {
  receiveWithStartEndMarkers();
  useNewData();
  delay(100);
}

void receiveWithStartEndMarkers() {
  static boolean receiveInProgress = false;
  static byte index = 0;
  char startMarker = '<';
  char endMarker = '>';
  char incomingCharacter;

  while (BluetoothDevice.available() > 0 && newData == false) {
    incomingCharacter = BluetoothDevice.read();

    if (receiveInProgress == true)
        if (incomingCharacter != endMarker) {
            receivedChars[index] = incomingCharacter;
            index++;
            if (index >= numChars) {
                index = numChars - 1;
            }
        }
        else {
            receivedChars[index] = '\0'; // terminate the string
            receiveInProgress = false;
            index = 0;
            newData = true;
        }
    else if (incomingCharacter == startMarker) {
        receiveInProgress = true;
    }
  }
}

void useNewData() {
    if (newData == true) {
        Serial.print("This just in ... ");
        Serial.println(receivedChars);
          char * part1;
          char * part2;
          char * part3;
          // This is how we split a string(actually character array) with a delimiter without c++ std lib.
          // See: https://hackingmajenkoblog.wordpress.com/2016/02/04/the-evils-of-arduino-strings/
          const char * delimiter = ",";
          part1 = strtok(receivedChars,delimiter);
          part2 = strtok (NULL,delimiter);
          part3 = strtok (NULL,delimiter);
          Serial.print("first part: ");
          Serial.println(part1);
          Serial.print("second part: ");
          Serial.println(part2);
          Serial.print("third part: ");
          Serial.println(part3);
          Serial.println(receivedChars);
        if(strcmp(part1, "Hi from web") == 0){
          Serial.println("Yay web says hi!");
          BluetoothDevice.println("Hi from arduino");
        }
        // I'll be using this for a temperature controller, 
        // but just leaving it here as an example of 
        // the kind of commands you can send.
        // If you send "setT,72" then you could handle that here.
        if(strcmp(part1, "setTemp") == 0){ // In this strange world strcmp is 0 if the comparison is true.
          Serial.print("I see targetTemp: ");
          Serial.println(atof(part2));
          BluetoothDevice.print("Aiming for ");
          BluetoothDevice.print(atof(part2));
          BluetoothDevice.println("C");
          delay(3000);
          BluetoothDevice.println("And that's great news to me.");
          // Heater.setTarget(atof(part2));
        }
        if(strcmp(part1, "echo") == 0){
          Serial.println("I must echo...");
          BluetoothDevice.print("I echo back    ");
          BluetoothDevice.println(part2);
        }
        if(strcmp(part1, "getTemp") == 0){
          Serial.println("I must fetch tempC...");
          BluetoothDevice.print("TempC: ");
          // Heater.getTemp();
        }
        newData = false;
    }
}
