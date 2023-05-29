/*

Author: Max Crutzen
Date:   28-05-2023

This code for the Central device. The system I am building uses 2 seeed studio xiao BLE nRF52840 microcontrollers to gather sensor data on the 
peripherals side and communicate that through BLE to the central device. Which interpets the data and creates feedback for the user through 
mapping data to vibrational motor actuators. 

The stage in which this code is in can be called the first prototype phase. The system works being conneted through breadboards to a laptop as
power source. The connection stratup can probably be streamlined with more checks and failsafes to make sure the system will always work when
activated. The various actuators can be put in an array instead of being defined independently. The if statements can be turned to a switch case
structure to also streamline this part of the code. Also code has to be added for switches for activating and deactivating the system. Plus the 
monitoring system of the battery level has to be added. Plus many more featured that will be added after the first hardware test.


Code based on an example on the official arduino page https://docs.arduino.cc/tutorials/nano-rp2040-connect/rp2040-ble-device-to-device
Although this code is written for 2 arduino nano rp2040 microcontrollers, it also works for my system since the seeed studio xiao BLE nRF52840 
is also compatible with the ArduinoBLE.h library.

*/

#include <ArduinoBLE.h>

// define which pins belong to which haptic feedback motor 
int vibOutPinFront = 0;
int vibOutPinMiddle = 1;
int vibOutPinBack = 2;

// no service uuid has to be defined since this is the central code, which means this is the side that searches for the adverting peripheral
// no characteristic has to be defined since it only reads data from the peripheral. There is no double sided communication

void setup() {

  pinMode(vibOutPinFront, OUTPUT);
  pinMode(vibOutPinMiddle, OUTPUT);
  pinMode(vibOutPinBack, OUTPUT);

  Serial.begin(9600);

  while (!Serial)
    ;

  // initialize the BLE hardware
  BLE.begin();
  Serial.println("BLE Central activated - hapticfeedback device");

  // start scanning for serviceuuid which is used by the peripheral
  BLE.scanForUuid("19B10000-E8F2-537E-4F6C-D104768A1214");
}

void loop() {

  // check if a peripheral has been discovered
  BLEDevice peripheral = BLE.available();
  if (peripheral) {

    // discovered a peripheral, print out address, local name, and advertised service
    // this part could be removed in the final code

    Serial.print("Found ");
    Serial.print(peripheral.address());
    Serial.print(" '");
    Serial.print(peripheral.localName());
    Serial.print("' ");
    Serial.print(peripheral.advertisedServiceUuid());
    Serial.println();

    if (peripheral.localName().indexOf("pressure_sensor_peripheral") < 0) {
      Serial.println("No 'pressure_sensor_peripheral' in name");
      return;  // If the name doesn't have "pressure_sensor_peripheral" in it then ignore it
    }

    // stop scanning if a peripheral with the localname has been found
    BLE.stopScan();

    // execute this void, stays in loop if the connection is active
    ReadDataAndActuate(peripheral);

    // peripheral disconnected, start scanning again
    BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1214");
  }
}

void ReadDataAndActuate(BLEDevice peripheral) {

  // connect to the peripheral, only to the one with the localname since it stops scanning if one of those has been found
  Serial.println("Connecting ...");

  if (peripheral.connect()) {

    Serial.println("Connected");

  } else {

    Serial.println("Failed to connect!");

    return;
    //return if connecting failed
  }

  // discover peripheral attributes
  Serial.println("Discovering attributes ...");

  if (peripheral.discoverAttributes()) {

    Serial.println("Attributes discovered");

  } else {

    Serial.println("Attribute discovery failed!");
    peripheral.disconnect();

    return;
  }

  // retrieve the sensor characteristics
  BLECharacteristic feetpressurecharacteristic1 = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1214");
  BLECharacteristic feetpressurecharacteristic2 = peripheral.characteristic("19b10002-e8f2-537e-4f6c-d104768a1214");
  BLECharacteristic feetpressurecharacteristic3 = peripheral.characteristic("19b10003-e8f2-537e-4f6c-d104768a1214");

  if (!feetpressurecharacteristic1) {

    Serial.println("Peripheral does not have LED characteristic!");

    peripheral.disconnect();

    return;
  }

  while (peripheral.connected()) {

    // while the peripheral is connected

    if (feetpressurecharacteristic1.canRead() && feetpressurecharacteristic2.canRead() && feetpressurecharacteristic3.canRead()) {

      byte value1 = feetpressurecharacteristic1.read(); //use for front, small sensor 
      byte value2 = feetpressurecharacteristic2.read(); //use for middle, small sensor 
      byte value3 = feetpressurecharacteristic3.read(); //use for the back, big sensor 

      feetpressurecharacteristic1.readValue(value1);
      feetpressurecharacteristic2.readValue(value2);
      feetpressurecharacteristic3.readValue(value3);

      Serial.println(value1);
      Serial.println(value2);
      Serial.println(value3);
      Serial.println("---------");

      if (value1 > 50) { 

        //maybe map the vibOut should be mapped to the value of the sensor. However, the sensors read until 1kg or the big one of 10kg so the 1kg ones will act more as a switch.
        analogWrite(vibOutPinFront, 255);

      }
      if(value1 < 50){
        analogWrite(vibOutPinFront, 0);
      }


      
      if(value2 > 50){
        analogWrite(vibOutPinMiddle, 255);
      }
      if(value2 < 50){
        analogWrite(vibOutPinMiddle, 0);
      }



      if(value3 > 50){
        analogWrite(vibOutPinBack, 255);
      }
      if(value3 < 50){
        analogWrite(vibOutPinBack, 0);
      }

      //use a swtich statement to clean this stuff up and put the outputvalue in a variable and map that variable to the value of the pressuresensor value

    }

    //delay(500);
  }

  Serial.println("Peripheral disconnected");
}
