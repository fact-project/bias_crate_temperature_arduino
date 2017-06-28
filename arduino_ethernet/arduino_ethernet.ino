#include <TimeLib.h>
#include <Time.h>
#include <ArduinoJson.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <Ethernet.h>


const unsigned int TEMP_PIN_1 = 2;
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

String sensorStringAddresses[NUM_SENSORS];

StaticJsonBuffer<512> jsonBuffer;

OneWire oneWire1(TEMP_PIN_1);
DallasTemperature sensors(&oneWire1);

byte mac[] = {
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};
IPAddress ip(192, 168, 2, 10);
EthernetServer server(80);

void setup() {

  pinMode(TEMP_PIN_1, INPUT_PULLUP);
  
  Serial.begin(115200);
  while(!Serial){
    delay(10);
  }
  Ethernet.begin(mac, ip);
  server.begin();

  // build address strings for json
  for(unsigned int id=0; id < NUM_SENSORS; id++) {
    sensorStringAddresses[id] = String((int) sensorAddresses[id][0], HEX);
    for (int b = 1; b < 8; b++) {
      int cur = (int) sensorAddresses[id][b];
      if (cur < 16) {
        sensorStringAddresses[id] = sensorStringAddresses[id] + " 0" + String(cur, HEX);
      } else {
        sensorStringAddresses[id] = sensorStringAddresses[id] + " " + String(cur, HEX);
      }
    }
  }
}

void loop() {

  EthernetClient client = server.available();
  if (client) {
    sensors.begin();
    Serial.println("Client available");
    sensors.requestTemperatures();
    jsonBuffer.clear();
    
    time_t timestamp = now();
    JsonObject& temperatureData = jsonBuffer.createObject();
    temperatureData["timestamp"] = timestamp;
    JsonObject& temperatures = temperatureData.createNestedObject("temperatures_deg_C");
    
    for (unsigned int sensorID = 0; sensorID < NUM_SENSORS; sensorID++){
      if (sensors.isConnected(sensorAddresses[sensorID])) {
        temperatures[sensorStringAddresses[sensorID]] = (sensors.getTempC(sensorAddresses[sensorID]));
      } else {
        temperatures[sensorStringAddresses[sensorID]] = "NaN";
      }
    }
    
    // A http response should start with this  stuff, but if its uncommented
    // it does not work
    //client.println("HTTP/1.1 200 OK");
    //client.println("Content-Type: application/json");
    //client.println("Connection: Closed");
  
    temperatureData.printTo(client);
    client.println();
    delay(10);
    client.stop();
  } else {
    Serial.println("No client available");
  }
}
