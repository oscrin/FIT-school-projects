/*
 * Faculty of Information Technology at Brno University of Technology
 * Subject: IMP - Microprocessors and Embedded Systems
 * Topic:  Temperature measuring using DS18B20 and ESP8266 (IoT, cloud)
 * Date:  December 2017
 * Author: Jan Kubica (xkubic39@stud.fit.vutbr.cz)
 */
 
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
 
// Jako datovy pin je pouzit D4 na desce NodeMCU
#define ONE_WIRE_BUS D4
// presnost teplomeru je nastavena na 0.5C
#define TEMP_PRECISION 9
 
// Nastaveni oneWire instance pro komunikaci se zarizenimi 
OneWire oneWire(ONE_WIRE_BUS);
 
// Priradi teplotní sensor k datovému pinu
DallasTemperature sensors(&oneWire);

// Teplotni sensor
DeviceAddress t_sensor_1;

// promenna pro ulozeni teploty
float temp;

// udaje pro pripojeni k WiFi
const char *ssid = "IMP";
const char *pass = "NodeMCU8266";

const char *serverURI = "http://192.168.0.100/main.php";

// casovy interval snimani je jedna minuta
int timeInterval = 60000;

void setup(void)
{
  // zahajeni seriove komunikace o rychlosti 115200
  Serial.begin(115200);
  
  Serial.println("NodeMCU started ...");

  // cyklus pro pripojeni k WiFi siti
  WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Waiting for connection");
    }

  Serial.print("Connected to: ");
  Serial.println(ssid);
  Serial.print("NodeMCU IP address: ");
  Serial.println(WiFi.localIP());
  
  // inicializace sensoru pomoci knihoven
  sensors.begin();

  Serial.print("Pocet teplomeru: ");
  Serial.println(sensors.getDeviceCount(), DEC);

  oneWire.reset_search();
  
  // kontrola komponent
  if (!oneWire.search(t_sensor_1)) Serial.println("Thermomether not found!");

  //nastav rozlišení
  sensors.setResolution(t_sensor_1, TEMP_PRECISION);

  // zahaji snimani na senzoru
  sensors.requestTemperatures();
}
 
 
void loop(void)
{

  if (WiFi.status() == WL_CONNECTED) {

    // zadost o zmereni teploty
    Serial.print("Getting temperatures... ");
    sensors.requestTemperatures();
    Serial.println("OK");

    Serial.print("Temperature for t_sensor_1 is: ");

    float temp;
    // zapis teploty z daneho teplomeru, pripadne moznost odkazovani pomoci indexu
    temp = sensors.getTempC(t_sensor_1);
    Serial.println(temp);

    // vytvoreni json dat pro odeslani na server
    StaticJsonBuffer<300> JSONbuffer;
    JsonObject& JSONencoder = JSONbuffer.createObject();
  
    JSONencoder["sensorType"] = "Temperature";
    JSONencoder["sensorNumber"] = 1;
    JSONencoder["measuredValue"] = temp;
  
    char JSONmessageBuffer[300];
    JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));

    // kontrola informaci v bufferu
    Serial.println("JSONmessageBuffer: ");
    Serial.println(JSONmessageBuffer);

    // vytvoreni http klienta
    HTTPClient http;
  
    http.begin(serverURI);
    http.addHeader("Content-Type", "application/json");
    
    int httpCode = http.POST(JSONmessageBuffer);
    String payload = http.getString();

    // odpoved ze serveru
    Serial.print("Return httpCode is: ");
    Serial.println(httpCode);   //Print HTTP return code
      
    http.end();

    // teplotu mer jednou za casovy interval
    delay(timeInterval);

  } else {
    // pri nevytvoreni spojeni
    Serial.println("WiFi connection lost...");
  }
  
}
