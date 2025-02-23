//Included Library
#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include "DFRobot_ESP_PH.h"
#include "EEPROM.h"
#include "GravityTDS.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <MQ135.h>

//Defined pin and state to be used 
DFRobot_ESP_PH ph;
GravityTDS gravityTds;
#define ESPADC 4096.0       //the esp Analog Digital Convertion value
#define VREF 3300     //the esp voltage supply value
#define PH_PIN 35           //the esp gpio data pin number for pH
#define Gas_PIN 32          //the esp gpio data pin number for Gas
#define RD_PIN 34           //the esp gpio data pin number for Raindrop
#define Turb_PIN 39         //the esp gpio data pin number for Turbidity
#define TdsSensorPin 36     //the esp gpio data pin number for TDS
const int oneWireBus = 33;  //the esp gpio data pin number for Temperature
float voltage, phValue;
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

// Replace with your network credentials
const char* ssid = "Beansinside";
const char* password = "Shortseal 10";

//Web/Server address to read/write from 
const char *host = "monitoring.cemebsa.com";
WiFiClient client;

// variable
float temp, temperature;
float gas;
float turbidity, turbi;
float pHvalue, pHsensor;
float rain;
float tdsValue = 0;
HTTPClient http;
String postData;
String postVariable;

void setup() {
  Serial.begin(115200);
  pinMode(PH_PIN, INPUT);
  pinMode(Gas_PIN, INPUT);
  pinMode(RD_PIN, INPUT);
  pinMode(Turb_PIN, INPUT);
  pinMode(TdsSensorPin, INPUT);
  gravityTds.setPin(TdsSensorPin);
  gravityTds.setAref(3.3);  //reference voltage on ADC, default 5.0V on Arduino UNO
  gravityTds.setAdcRange(ESPADC);  //1024 for 10bit ADC;4096 for 12bit ADC
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  EEPROM.begin(32);//needed to permit storage of calibration value in eeprom
  ph.begin();
  gravityTds.begin();  //initialization
  sensors.begin();
  
}

void readTemperature() {
  //Temp Sensor
  sensors.requestTemperatures();
  temp = sensors.getTempCByIndex(0);
//  Serial.print("Temperature:");
//  Serial.print(temp);
//  Serial.println("ºC");
}

void readRaindrop() {
  rain = analogRead(RD_PIN);
//  Serial.print("Raindrop:");
//  Serial.println(rain);  // print out the value you read:   
}

void readGas() {
  MQ135 gasSensor = MQ135(analogRead(Gas_PIN));
  gas = (gasSensor.getPPM())/10000.0; // Get the ppm of CO2 sensed (assuming only CO2 in the air)
//  Serial.print("Gas:");
//  Serial.print(gas);  // print out the value you read:
//  Serial.println("ppm");
}

void readTurbidity() {
  int sensorValue = analogRead(Turb_PIN);// read the input on analog pin 39:
  turbi = map(sensorValue, 0, 1400, 5, 1);
//  Serial.print("Turbidity:");
//  Serial.print(turbi);  // print out the value you read:
//  Serial.println("NTU");
}

void readpH() {
  static unsigned long timepoint = millis();
  if (millis() - timepoint > 1000U) //time interval: 1s
  {
    timepoint = millis();
    //voltage = rawPinValue / esp32ADC * esp32Vin
    voltage = analogRead(PH_PIN) / ESPADC * VREF; // read the voltage
    phValue = ph.readPH(voltage, temp); // convert voltage to pH with temperature compensation
//    Serial.print("pH:");
//    Serial.println(phValue, 4);
  }
}

void readTDS() {
  temperature = temp;  //add your temperature sensor and read it
  gravityTds.setTemperature(temperature);  // set the temperature and execute temperature compensation
  gravityTds.update();  //sample and calculate 
  tdsValue = gravityTds.getTdsValue();  // then get the value
//  Serial.print(tdsValue,0);
//  Serial.println("ppm");
}

void sendtoDB() {
  turbidity = turbi;
  postVariable = "suhu= ";
  //Post Data
  postData = postVariable + temp + " &ph= " + phValue + " &turbidity= " + turbidity + " &raindrop= " + rain + " &gas= " + gas + " &TDS= " + tdsValue;

  http.begin(client, "http://monitoring.cemebsa.com/api/datasensor");  //Specify request destination
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");       //Specify content-type header

  int httpCode = http.POST(postData);  //Send the request
  String payload = http.getString();   //Get the response payload

  Serial.println(postData);
  Serial.println(httpCode);  //Print HTTP return code
  Serial.println(payload);   //Print request response payload

  http.end();  //Close connection
}

void loop() {
  readTemperature();
  readTurbidity();
  readpH();
  readGas();
  readRaindrop();
  readTDS();
  sendtoDB();
  delay(1000);
}
