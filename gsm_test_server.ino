#include <SoftwareSerial.h>
#include <SimpleDHT.h>
#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 RTC;


String latitude,longitude;
String inputString = ""; // a string to hold incoming data
boolean stringComplete = false; // whether the string  is complete
String signal = "$GPGLL";

  //RTC Library https://github.com/JChristensen/DS3232RTC
// for DHT22, 
//      VCC: 5V or 3V
//      GND: GND
//      DATA: 2
int pinDHT22 = 2;
int delay_minute;
int delay_hour;
SimpleDHT22 dht22(pinDHT22);
#define anInput     A0                        //analog feed from MQ135
#define digTrigger   10                        //digital feed from MQ135
#define co2Zero     55
/* Create object named SIM900 of the class SoftwareSerial */
SoftwareSerial SIM900(8, 7);//(Rx,Tx)
void setup() {
  Wire.begin();
  //inputString.reserve(200);
  
  
  // DHT22 sampling rate is 0.5HZ.
  delay(2500);
  Serial.println("HTTP post method :");
  Serial.print("AT\\r\\n");
  SIM900.println("AT"); /* Check Communication */
  delay(5000);
  ShowSerialData(); /* Print response on the serial monitor */
  delay(5000);
  /* Configure bearer profile 1 */
  Serial.print("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\\r\\n");
  SIM900.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");  /* Connection type GPRS */
  delay(5000);
  ShowSerialData();
  delay(5000);
  Serial.print("AT+SAPBR=3,1,\"APN\",\"airtelgprs.com\"\\r\\n");
  SIM900.println("AT+SAPBR=3,1,\"APN\",\"airtelgprs.com\"");  /* APN of the provider */
  delay(5000);
  ShowSerialData();
  delay(5000);
  Serial.print("AT+SAPBR=1,1\\r\\n");
  SIM900.println("AT+SAPBR=1,1"); /* Open GPRS context */
  delay(5000);
  ShowSerialData();
  delay(5000);
  Serial.print("AT+SAPBR=2,1\\r\\n");
  SIM900.println("AT+SAPBR=2,1"); /* Query the GPRS context */
  delay(5000);
  ShowSerialData();
  delay(5000);
  Serial.print("AT+HTTPINIT\\r\\n");
  SIM900.println("AT+HTTPINIT");  /* Initialize HTTP service */
  delay(5000); 
  ShowSerialData();
  delay(5000);
  Serial.print("AT+HTTPPARA=\"CID\",1\\r\\n");
  SIM900.println("AT+HTTPPARA=\"CID\",1");  /* Set parameters for HTTP session */
  delay(5000);
  ShowSerialData();
  delay(5000);
  Serial.print("AT+HTTPPARA=\"URL\",\"ptsv2.com/t/1hp6i-1583133118/post\"\\r\\n");
  SIM900.println("AT+HTTPPARA=\"URL\",\"ptsv2.com/t/1hp6i-1583133118/post\"");  /* Set parameters for HTTP session */
  delay(5000);
  ShowSerialData();
  delay(5000);
  Serial.print("AT+HTTPDATA=120,10000\\r\\n");
  SIM900.println("AT+HTTPDATA=120,10000"); /* POST data of size 33 Bytes with maximum latency time of 10seconds for inputting the data*/ 
  delay(2000);
  ShowSerialData();
  delay(2000);
//  int month_value=06;
//  int day_value=07;
//  int hour_value=13;
//  int min_value=37;
//  int sec_value=30;
//  int year=2019;
if (stringComplete) {
        String BB = inputString.substring(0, 6);
        if (BB == signal) {
            String LAT = inputString.substring(7, 17);
            int LATperiod = LAT.indexOf('.');
            int LATzero = LAT.indexOf('0');
            if (LATzero == 0) {
                LAT = LAT.substring(1);
            }

            String LON = inputString.substring(20, 31);
            int LONperiod = LON.indexOf('.');
            int LONTzero = LON.indexOf('0');
            if (LONTzero == 0) {
                LON = LON.substring(1);
            }
            Serial.println("Latitude= ");
            Serial.println(LAT);
            Serial.println("Longitude= ");
            Serial.println(LON);
            latitude= LAT;
            longitude= LON;

        }

         //Serial.println(inputString);
        // clear the string:
        inputString = "";
        stringComplete = false;
    }
  String Date_data = String(year_value) + "-" + String(month_value) + "-" + String(day_value) + "_" + String(hour_value) + ":" + String(min_value) + ":" + String(sec_value);
  String data= "Temperature= " + String(temperature) + "& " + "Humidity= " + String(humidity) + "& " + "Date Time=" + Date_data + "Co2 " + String(co2ppm) + "Longitude " + longitude + "Latitude " + latitude;
  Serial.print(data);  /* Data to be sent */
  SIM900.println(data);
  delay(5000);
  ShowSerialData();
  delay(5000);
  Serial.print("AT+HTTPACTION=1\\r\\n");
  SIM900.println("AT+HTTPACTION=1");  /* Start POST session */
  delay(5000);
  ShowSerialData();
  delay(5000);
  Serial.print("AT+HTTPTERM\\r\\n");  
  SIM900.println("AT+HTTPTERM");  /* Terminate HTTP service */
  delay(5000);
  ShowSerialData();
  delay(5000);
  Serial.print("AT+SAPBR=0,1\\r\\n");
  SIM900.println("AT+SAPBR=0,1"); /* Close GPRS context */
  delay(5000);
  ShowSerialData();
  delay(5000);
  Serial.println("Done");
  delay_minute= delay_minute+10;
  //delay_hour= hour;
  if(delay_minute>=60)
  {
    delay_minute=delay_minute-60;
    delay_hour=delay_hour+1;
  }
  if(delay_hour>=24)
  {
    delay_hour=delay_hour-24;
  }
  }
  delay(5000);
}

void ShowSerialData()
{
  while(SIM900.available()!=0)  /* If data is available on serial port */
  Serial.write(char (SIM900.read())); /* Print character received on to the serial monitor */
}

void serialEvent() {
    while (Serial.available()) {
        // get the new byte:
        char inChar = (char) Serial.read();
        // add it to the inputString:
        inputString += inChar;
        // if the incoming character is a newline, set a flag
        // so the main loop can do something about it:
        if (inChar == '\n') {
            stringComplete = true;
        }
    }
}
