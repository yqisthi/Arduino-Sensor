# Sensor Monitoring System untuk Keramba Jaring Apung Bulat Bertingkat (KJABB-RNS) menggunakan ESP32

Dalam repo ini, ada beberapa tes kode untuk melakukan percobaan sensor yang digunakan. 
Sensor yang digunakan adalah
1. Sensor pH dengan DFRobot Gravity: Analog pH Sensor Kit V2.0
2. Sensor suhu DS18B20 Waterproof Digital Temperature Sensor
3. Sensor kekeruhan dengan DFRobot Gravity: Analog Turbidity Sensor
4. Sensor gas dengan MQ-135
5. Sensor tetesan hujan dengan MH-RD
6. Sensor TDS dengan DFRobot Gravity: Analog TDS Sensor

Otak yang digunakan dalam projek ini adalah ESP32. Harap install library ESP32 di Arduino IDE sebelum melakukan percobaan. 
Untuk menginstall library ESP32, silahkan ikuti langkah-langkah berikut:
1. Buka Arduino IDE
2. Pilih menu 'File' -> 'Preferences'
3. Di bagian 'Additional Boards Manager URLs', masukkan link berikut: https://dl.espressif.com/dl/package_esp32_index.json
4. Pilih menu 'Tools' -> 'Board' -> 'Boards Manager'
5. Cari 'esp32' dan install library 'esp32 by Espressif Systems'
6. Pilih menu 'Tools' -> 'Board' -> 'ESP32 Dev Module'

## Percobaan Sensor pH
Untuk melakukan percobaan Sensor pH, silahkan ikuti langkah-langkah berikut:
1. Download library 'DFRobot_PH' di laman berikut: https://github.com/greenponik/DFRobot_ESP_PH_BY_GREENPONIK dan download file zipnya
2. Buka Arduino IDE
3. Pilih menu 'Sketch' -> 'Include Library' -> 'Add .ZIP Library'
4. Pilih file zip yang sudah diunduh
5. Pilih menu 'File' -> 'Examples' -> 'DFRobot_ESP_PH_BY_GREENPONIK' -> 'DFRobot_ESP_PH_BY_GREENPONIK'
6. Pilih menu 'Tools' -> 'Board' -> 'ESP32 Dev Module'
7. Pilih menu 'Tools' -> 'Port' -> pilih port yang digunakan
8. Pilih menu 'Sketch' -> 'Upload'
9. Buka serial monitor dengan baud rate 115200
10. Pada serial monitor, akan muncul nilai pH yang terbaca
11. Lakukan kalibrasi pH dengan menggunakan solusi pH 4 dan pH 7
12. Pada serial monitor, akan muncul nilai pH yang terbaca setelah dilakukan kalibrasi
Untuk pin yang digunakan dalam percobaan ini adalah:

| Sensor pH | ESP32   |
|-----------|---------|
| VCC       | 3.3V    |
| GND       | GND     |
| A         | GPIO 35 |

## Percobaan Sensor Suhu
Untuk melakukan percobaan Sensor suhu, silahkan ikuti langkah-langkah berikut:
1. Install library OneWire library by Paul Stoffregen dan DallasTemperature library by Miles Burton di Arduino IDE
2. Buatlah file baru di Arduino IDE dan salin kode berikut:
```c++
#include <OneWire.h>
#include <DallasTemperature.h>

// GPIO where the DS18B20 is connected to
const int oneWireBus = 4;     

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);
  // Start the DS18B20 sensor
  sensors.begin();
}

void loop() {
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);
  Serial.print(temperatureC);
  Serial.println("ºC");
  Serial.print(temperatureF);
  Serial.println("ºF");
  delay(5000);
}
```
3. Pilih menu 'Sketch' -> 'Upload'
4. Buka serial monitor dengan baud rate 115200
5. Pada serial monitor, akan muncul nilai suhu yang terbaca
Untuk pin yang digunakan dalam percobaan ini adalah:

| Sensor suhu | ESP32 |
|-------------|-------|
| VCC         | 3.3V  |
| GND         | GND   |
| DATA        | GPIO 4 + 4,7K Ohm + VCC |

## Percobaan Sensor Kekeruhan
Untuk melakukan percobaan Sensor kekeruhan, silahkan ikuti langkah-langkah berikut:
1. Buka Arduino IDE, dan buatlah file baru
2. Salin kode berikut:
```c++
void setup() {
  Serial.begin(115200); //Baud rate: 115200
}
void loop() {
  int sensorValue = analogRead(39);// read the input on analog pin 39
  float turbidity = map(sensorValue, 0, 1400, 5, 1);
  String text = (String)turbidity;
  Serial.println(text); // print out the value you read:
  delay(500);
}
```
3. Pilih menu 'Sketch' -> 'Upload'
4. Buka serial monitor dengan baud rate 115200
5. Pada serial monitor, akan muncul nilai kekeruhan yang terbaca
Untuk pin yang digunakan dalam percobaan ini adalah:

| Sensor kekeruhan | ESP32 |
|------------------|-------|
| VCC              | 5V  |
| GND              | GND   |
| DATA             | GPIO 39 |

⚠️ **Catatan**: Karena sensor ini menggunakan 5V dan ESP32 hanya dapat memberikan 3.3V, maka digunakan resistor 470 ohm dan 1K ohm untuk mengurangi tegangan menjadi sekitar 3.3V.

## Percobaan Sensor Gas
Untuk melakukan percobaan Sensor gas, silahkan ikuti langkah-langkah berikut:
1. Unduh library 'mq135' di laman berikut: https://github.com/GeorgK/MQ135 dan download file zipnya
2. Buka Arduino IDE, lalu pilih menu 'Sketch' -> 'Include Library' -> 'Add .ZIP Library' dan pilih file zip yang sudah diunduh
3. Buatlah file baru di Arduino IDE dan salin kode berikut:
```c++
#include "MQ135.h"
#define Gas_PIN 36
float gas;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  MQ135 gasSensor = MQ135(analogRead(Gas_PIN););
  gas = (gasSensor.getPPM())/10000.0; // Get the ppm of CO2 sensed (assuming only CO2 in the air)
  Serial.print(" Gas: ");
  Serial.print(gas);  // print out the value you read:
}
```
4. Pilih menu 'Sketch' -> 'Upload'
5. Buka serial monitor dengan baud rate 115200
6. Pada serial monitor, akan muncul nilai gas yang terbaca
Untuk pin yang digunakan dalam percobaan ini adalah:

| Sensor gas | ESP32 |
|------------|-------|
| VCC        | 5V  |
| GND        | GND   |
| DATA       | GPIO 36 |

⚠️ **Catatan**: Karena sensor ini menggunakan 5V dan ESP32 hanya dapat memberikan 3.3V, maka digunakan resistor 470 ohm dan 1K ohm untuk mengurangi tegangan menjadi sekitar 3.3V.

## Percobaan Sensor Tetesan Hujan
Untuk melakukan percobaan Sensor tetesan hujan, silahkan ikuti langkah-langkah berikut:
1. Buka Arduino IDE, dan buatlah file baru
2. Salin kode berikut:
```c++
#define RD_PIN 34
float rain;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}
void loop() {
  rain = analogRead(RD_PIN);
  Serial.print(" Rain: ");
  Serial.print(rain);  // print out the value you read:
}
```
3. Pilih menu 'Sketch' -> 'Upload'
4. Buka serial monitor dengan baud rate 115200
5. Pada serial monitor, akan muncul nilai tetesan hujan yang terbaca
Untuk pin yang digunakan dalam percobaan ini adalah:

| Sensor tetesan hujan | ESP32 |
|----------------------|-------|
| VCC                  | 3.3V  |
| GND                  | GND   |
| DATA                 | GPIO 34 |

## Percobaan Sensor TDS
Untuk melakukan percobaan Sensor TDS, silahkan ikuti langkah-langkah berikut:
1. Buka Arduino IDE, dan buatlah file baru
2. Salin kode berikut:
```c++
#include <OneWire.h>
#include <DallasTemperature.h>
 
const int oneWireBus = 33; // GPIO where the DS18B20 is connected to
 
#define TdsSensorPin 36
#define VREF 3.3      // analog reference voltage(Volt) of the ADC
#define SCOUNT  30           // sum of sample point 
int analogBuffer[SCOUNT];    // store the analog value in the array, read from ADC
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0;
int copyIndex = 0;
float averageVoltage = 0;
float tdsValue = 0;
float temperature = 0;
 
 
OneWire oneWire(oneWireBus);    // Setup a oneWire instance to communicate with any OneWire devices
 
DallasTemperature sensors(&oneWire);    // Pass our oneWire reference to Dallas Temperature sensor
 
void setup()
{
  Serial.begin(115200);
  pinMode(TdsSensorPin, INPUT);
  sensors.begin();
}
 
void loop()
{
  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);
 
  static unsigned long analogSampleTimepoint = millis();
  if (millis() - analogSampleTimepoint > 50U)  //every 40 milliseconds,read the analog value from the ADC
  {
    analogSampleTimepoint = millis();
    analogBuffer[analogBufferIndex] = analogRead(TdsSensorPin);    //read the analog value and store into the buffer
    analogBufferIndex++;
    if (analogBufferIndex == SCOUNT)
      analogBufferIndex = 0;
  }
  static unsigned long printTimepoint = millis();
  if (millis() - printTimepoint > 1000U)
  {
    printTimepoint = millis();
    for (copyIndex = 0; copyIndex < SCOUNT; copyIndex++)
      analogBufferTemp[copyIndex] = analogBuffer[copyIndex];
    averageVoltage = getMedianNum(analogBufferTemp, SCOUNT) * (float)VREF / 1024.0; // read the analog value more stable by the median filtering algorithm, and convert to voltage value
    float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0); //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
    float compensationVolatge = averageVoltage / compensationCoefficient; //temperature compensation
    tdsValue = (133.42 * compensationVolatge * compensationVolatge * compensationVolatge - 255.86 * compensationVolatge * compensationVolatge + 857.39 * compensationVolatge) * 0.5; //convert voltage value to tds value
 
    Serial.print("TDS Value:");
    Serial.print(tdsValue, 0);
    Serial.println("ppm");
 
    Serial.print("Temperature:");
    Serial.print(temperature);
    Serial.println("ÂºC");
  }
}
int getMedianNum(int bArray[], int iFilterLen)
{
  int bTab[iFilterLen];
  for (byte i = 0; i < iFilterLen; i++)
    bTab[i] = bArray[i];
  int i, j, bTemp;
  for (j = 0; j < iFilterLen - 1; j++)
  {
    for (i = 0; i < iFilterLen - j - 1; i++)
    {
      if (bTab[i] > bTab[i + 1])
      {
        bTemp = bTab[i];
        bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
      }
    }
  }
  if ((iFilterLen & 1) > 0)
    bTemp = bTab[(iFilterLen - 1) / 2];
  else
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
  return bTemp;
}
```
3. Pilih menu 'Sketch' -> 'Upload'
4. Buka serial monitor dengan baud rate 115200
5. Pada serial monitor, akan muncul nilai TDS yang terbaca
Untuk pin yang digunakan dalam percobaan ini adalah:

| Sensor TDS | ESP32 |
|------------|-------|
| VCC        | 3.3V  |
| GND        | GND   |
| DATA       | GPIO 36 |

⚠️ **Catatan**: Untuk sensor TDS, kode diatas merupakan kode tes dan bukan kode untuk penggunaan di lapangan. Kode ini hanya untuk mengetahui nilai TDS yang terbaca oleh sensor. Untuk penggunaan di lapangan, silahkan gunakan kode yang telah disediakan oleh pihak DFRobot pada link https://wiki.dfrobot.com/Gravity__Analog_TDS_Sensor___Meter_For_Arduino_SKU__SEN0244#target_4.
