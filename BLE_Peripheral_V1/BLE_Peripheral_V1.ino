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

//All serial commands need to be turned off when you want to use it disconnected of the laptop

#include <ArduinoBLE.h>

#define EXE_INTERVAL 100

// set the UUID of the periperals service
BLEService serviceUuid = "19B10000-E8F2-537E-4F6C-D104768A1214";

// defining the characteristics for the sensor data to be written to central. Also defining that the permissions for the central device.
BLEUnsignedIntCharacteristic feetpressurecharacteristic1("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);
BLEUnsignedIntCharacteristic feetpressurecharacteristic2("19B10002-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);
BLEUnsignedIntCharacteristic feetpressurecharacteristic3("19B10003-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);


int period = 10000;
unsigned long time_now = 0;

unsigned long lastExecutedMillis = 0;  // variable to save the last executed time

void setup() {

  //only definining the pins already turns on the blue LED, defining is not necessary 
  //pinMode(LEDB, OUTPUT);
  //pinMode(LED_BUILTIN, OUTPUT);


  Serial.begin(9600);
  // the code only executes while the serial monitor is available because of this line
  //while (!Serial);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");

    while (1)
      ;
  }

  Serial.println("Bluetooth® Low Energy Central - Peripheral Explorer");

  //set connection interval to be the fastest possible (every 7.5ms)
  BLE.setConnectionInterval(6, 6);
  Serial.println("peripheral connection interval set (6, 6)");

  // set advertised local name and service UUID:
  BLE.setLocalName("pressure_sensor_peripheral");
  Serial.println("peripheral Local Name set");

  // add the service UUID, only the uuid
  BLE.setAdvertisedService(serviceUuid);
  Serial.println("service UUID added");

  // add the characteristics to the service
  serviceUuid.addCharacteristic(feetpressurecharacteristic1);
  serviceUuid.addCharacteristic(feetpressurecharacteristic2);
  serviceUuid.addCharacteristic(feetpressurecharacteristic3);
  Serial.println("characteristics added");

  // add THE >SERVICE<
  BLE.addService(serviceUuid);
  Serial.println("service added");

  BLE.advertise();
  Serial.println("advertising started");
}

void loop() {

  // this turns on the LED somehow, it needs to be low
  digitalWrite(LED_BUILTIN, LOW);

  // listen for BLE peripherals to connect. This naming and reasoning does not make sense to me
  BLEDevice central = BLE.central();

  unsigned long currentMillis = millis();

  // When connected it does not execute the beginning of the loop anymore so everything would have to take place in the while statement.
  // Because this is what code will be running when the device is connected
  if (central) {

    Serial.print("Connected to central: ");

    // print the central's MAC address:
    Serial.println(central.address());

    // byte sensorValue1 = analogRead(A0);
    Serial.println(sensorValue1);

    // while the central is still connected to peripheral:
    while (central.connected()) {

      digitalWrite(LEDB, LOW);

      unsigned long currentMillis = millis();

      // If the time now - 0 >= 1000 so the code is executed after 1000 once because the lastexecutedmilis gets equalized with the currentmillis.
      // So 1000 - 1000, but then after that lastexecutedmillis is still 1000 because this isn't reset. So How does this work? But it works
      if (currentMillis - lastExecutedMillis >= EXE_INTERVAL) {
        // Save the last executed time
        lastExecutedMillis = currentMillis;

        byte sensorValue1 = analogRead(A0);
        byte sensorValue2 = analogRead(A1);
        byte sensorValue3 = analogRead(A2);

        // Turning the analogread into bytes works perfectly
        sensorValue1 = map(sensorValue1, 0, 205, 0, 100);
        sensorValue2 = map(sensorValue2, 0, 205, 0, 100);
        sensorValue3 = map(sensorValue3, 0, 205, 0, 100);

        Serial.println(sensorValue1);
        Serial.println(sensorValue2);
        Serial.println(sensorValue3);
        Serial.println("---------");

        //write the sensor values to the characteristic
        feetpressurecharacteristic1.writeValue(sensorValue1);
        feetpressurecharacteristic2.writeValue(sensorValue2);
        feetpressurecharacteristic3.writeValue(sensorValue3);
      }

      //delay(500);
    }
  }
}
