#include "Adafruit_VL53L0X.h"
#include "./json.h"
// address we will assign if dual sensor is present
#define LOX1_ADDRESS 0x2A
#define LOX2_ADDRESS 0x2B
#define LOX3_ADDRESS 0x2C
#define LOX4_ADDRESS 0x2D

// set the pins to shutdown
#define SHT_LOX1 6
#define SHT_LOX2 7 
#define SHT_LOX3 8
#define SHT_LOX4 9

// objects for the vl53l0x
Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox3 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox4 = Adafruit_VL53L0X();

// this holds the measurement
VL53L0X_RangingMeasurementData_t measure1;
VL53L0X_RangingMeasurementData_t measure2;
VL53L0X_RangingMeasurementData_t measure3;
VL53L0X_RangingMeasurementData_t measure4;

DynamicJsonDocument doc(1024);

void resetAll() {
  // all reset
  digitalWrite(SHT_LOX1, LOW);    
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);
  digitalWrite(SHT_LOX4, LOW);
  delay(10);
  // all unreset
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, HIGH);
  digitalWrite(SHT_LOX3, HIGH);
  digitalWrite(SHT_LOX4, HIGH);
  delay(10);
  digitalWrite(SHT_LOX1, LOW);    
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);
  digitalWrite(SHT_LOX4, LOW);
  delay(10);

}
 
void setID() {
  resetAll();
  // activating LOX1 and reseting LOX2
  digitalWrite(SHT_LOX1, HIGH);
  delay(50);
  // initing LOX1
  if(!lox1.begin(LOX1_ADDRESS)) {
    Serial.println(F("Failed to boot first VL53L0X"));
    while(1);
  }
  delay(100);

  // activating LOX2
  digitalWrite(SHT_LOX2, HIGH);
  delay(50);

  //initing LOX2
  if(!lox2.begin(LOX2_ADDRESS)) {
    Serial.println(F("Failed to boot second VL53L0X"));
    while(1);
  }
  delay(100);

  // activating LOX3
  digitalWrite(SHT_LOX3, HIGH);
  delay(50);

  //initing LOX3
  if(!lox3.begin(LOX3_ADDRESS)) {
    Serial.println(F("Failed to boot third VL53L0X"));
    while(1);
  }
  delay(100);

   digitalWrite(SHT_LOX4, HIGH);
  delay(50);

  //initing LOX3
  if(!lox4.begin(LOX4_ADDRESS)) {
    Serial.println(F("Failed to boot fourth VL53L0X"));
    while(1);
  }
  delay(100);
  
}

void read_sensors() {
  
  lox1.rangingTest(&measure1, false); // pass in 'true' to get debug data printout!
  lox2.rangingTest(&measure2, false); // pass in 'true' to get debug data printout!
  lox3.rangingTest(&measure3, false);
  lox4.rangingTest(&measure4, false);
  
  // print sensor one reading
  //Serial.print(F("1: "));
  if(measure1.RangeStatus != 4 && measure1.RangeMilliMeter <= 1000) {     // if not out of range
    doc["s1"] = measure1.RangeMilliMeter;
  } else {
    doc["s1"] = -1;
  }
  
  //Serial.print(F(" "));

  // print sensor two reading
  //Serial.print(F("2: "));
  if(measure2.RangeStatus != 4 && measure2.RangeMilliMeter <= 1000) {
    doc["s2"] = measure2.RangeMilliMeter;
  } else {
    doc["s2"] = -1;
  }


  //Serial.print(F(" "));

  // print sensor two reading
  //Serial.print(F("3: "));
  if(measure3.RangeStatus != 4 && measure3.RangeMilliMeter <= 1000) {
    doc["s3"] = measure3.RangeMilliMeter;
  } else {
    doc["s3"] = -1;
  }

  //Serial.print(F(" "));

  // print sensor two reading
  //Serial.print(F("4: "));
  if(measure4.RangeStatus != 4 && measure4.RangeMilliMeter <= 1000) {
    doc["s4"] = measure4.RangeMilliMeter;
  } else {
    doc["s4"] = -1;
  }
  serializeJson(doc, Serial);
  Serial.println();
}

void setup() {
  doc["s1"] = 0;
  doc["s2"] = 0;
  doc["s3"] = 0;
  doc["s4"] = 0;
  Serial.begin(115200);

  // wait until serial port opens for native USB devices
  while (! Serial) { delay(1); }

  pinMode(SHT_LOX1, OUTPUT);
  pinMode(SHT_LOX2, OUTPUT);
  pinMode(SHT_LOX3, OUTPUT);
  pinMode(SHT_LOX4, OUTPUT);

  Serial.println(F("Shutdown pins inited..."));

  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);
  digitalWrite(SHT_LOX4, LOW);

  Serial.println(F("Both in reset mode...(pins are low)"));
  
  
  Serial.println(F("Starting..."));
  setID();
 
}

void loop() {
   
  read_sensors();
  delay(100);
}
