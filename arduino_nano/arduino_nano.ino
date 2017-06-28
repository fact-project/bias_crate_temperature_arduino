#include <DallasTemperature.h>
#include <OneWire.h>


const unsigned int DAQ_PIN = 2;
const unsigned int NUM_SENSORS = 10;
const DeviceAddress sensorAddresses[NUM_SENSORS] = {
  {0x10, 0x50, 0x77, 0x2D, 0x03, 0x08, 0x00, 0x096},
  {0x10, 0xCC, 0xF3, 0x2D, 0x03, 0x08, 0x00, 0x08E},
  {0x10, 0xCA, 0x24, 0x2E, 0x03, 0x08, 0x00, 0x0BB},
  {0x10, 0x16, 0x75, 0x2D, 0x03, 0x08, 0x00, 0x0D2},
  {0x10, 0xA5, 0xF0, 0x2D, 0x03, 0x08, 0x00, 0x095},
  {0x10, 0xE5, 0x54, 0x2D, 0x03, 0x08, 0x00, 0x0CC},
  {0x10, 0x1D, 0xCE, 0x2D, 0x03, 0x08, 0x00, 0x015},
  {0x10, 0xB7, 0x6D, 0x2D, 0x03, 0x08, 0x00, 0x0FB},
  {0x10, 0x9F, 0x02, 0x2E, 0x03, 0x08, 0x00, 0x01A},
  {0x10, 0x9F, 0x51, 0x2D, 0x03, 0x08, 0x00, 0x049}
};

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

  for (unsigned int sensorID = 0; sensorID < NUM_SENSORS; sensorID++) {
    Serial.print("Address: ");
    for(int addrByte = 0; addrByte < 8; addrByte++){
      if (sensorAddresses[sensorID][addrByte] < 16) {
        Serial.print("0");
      }
      Serial.print(sensorAddresses[sensorID][addrByte], HEX);
      Serial.print(" ");
    }
    Serial.print("Temperature: ");
    if (sensors.isConnected(sensorAddresses[sensorID])) {
      Serial.println(sensors.getTempC(sensorAddresses[sensorID]));
    } else {
      Serial.println("NaN");
    }
  }
  Serial.println();
  delay(5000);
}