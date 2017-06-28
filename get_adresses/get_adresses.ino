#include <DallasTemperature.h>
#include <OneWire.h>

const unsigned int DAQ_PIN = 2;

OneWire oneWire1(DAQ_PIN);
DallasTemperature sensors(&oneWire1);

void setup() {

  pinMode(DAQ_PIN, INPUT_PULLUP);
  
  Serial.begin(115200);
  while(!Serial){
    delay(10);
  }
}

void loop() {
  sensors.begin();
  sensors.requestTemperatures();
  
  unsigned int numSensors = sensors.getDeviceCount();
 
  Serial.print("Num devices: ");
  Serial.println(numSensors);
  
  DeviceAddress addr;
  
  for (unsigned int sensorID = 0; sensorID < numSensors; sensorID++) {
    bool success_flag = sensors.getAddress(addr, sensorID);
    Serial.print("{");
    for(int byteCounter = 0; byteCounter < 8; byteCounter++){

      Serial.print("0x");
      if (addr[byteCounter] < 16) {
        Serial.print("0");
      }
      Serial.print(addr[byteCounter], HEX);
      Serial.print(", ");
    }
    Serial.println("}");
  }
  Serial.println();
  delay(100);
}
