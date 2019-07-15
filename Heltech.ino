#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include<LiquidCrystal.h>
 
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);                 
TinyGPSPlus gps;
SoftwareSerial GPS(6, 7);                              // (Rx, Tx)GPS
SoftwareSerial PHdis(8, 9);                            // (Rx, Tx)bluetooth
SoftwareSerial gsm(10,13);                             //(Rx,Tx)GSM

int shock=1;
int threshold= 10;
int led1 = 2;

void setup()
{
  Serial.begin(9600);
  gsm.begin(9600);
  PHdis.begin(9600);
  GPS.begin(9600);
  pinMode (led1, OUTPUT);
  lcd.begin(16, 2);                                    
}
  void loop()
  {
    int val= analogRead(shock);
    Serial.println(val);
  
    if (val < threshold)
    {
    digitalWrite(led1, LOW);
    }
    else
    {
     digitalWrite(led1, HIGH);
     delay(100);
     digitalWrite(led1, LOW);
     delay(100);
     digitalWrite(led1, HIGH);
     delay(100);
     digitalWrite(led1, LOW);
      
     smartDelay(1000);
     unsigned long start;
     double lat_val, lng_val, alt_m_val;
     bool loc_valid, alt_valid, time_valid;
     lat_val = gps.location.lat();
     loc_valid = gps.location.isValid();
     lng_val = gps.location.lng();
     alt_m_val = gps.altitude.meters();
     alt_valid = gps.altitude.isValid();
      
     if (!loc_valid || !alt_valid )
      {
        Serial.print("Latitude : ");
        Serial.println("*****");
        Serial.print("Longitude : ");
        Serial.println("*****");
        Serial.print("Altitude : ");
        Serial.println("*****");
        lcd.setCursor(0, 0);        
        lcd.print("error");
      }
      else
      {
        Serial.print("Latitude in Decimal Degrees : ");
        Serial.println(lat_val, 6);
        Serial.print("Longitude in Decimal Degrees : ");
        Serial.println(lng_val, 6);
        Serial.print("Altitude : ");
        Serial.println(alt_m_val, 6);

        Serial.print("http://maps.google.com/maps?q=");
        Serial.print(lat_val,6);
        Serial.print(",");
        Serial.print(lng_val,6);

        lcd.setCursor(0, 0);         
        lcd.print("latitude:");
        lcd.print(lat_val);
        lcd.setCursor(0, 1);          
        lcd.print("longitude:");
        lcd.print(lng_val);
 
        gsm.print("\r");
        delay(1000);                  
        gsm.print("AT+CMGF=1\r");    
        delay(1000);
        gsm.print("AT+CMGS=\"+919408218288\"\r");    
        delay(1000);
        gsm.print("http://maps.google.com/maps?q=");  
        gsm.print(lat_val);
        gsm.print(",");
        gsm.print(lng_val); 
        delay(1000);
        gsm.write(0x1A);
        delay(1000);

        PHdis.print(lat_val);
        PHdis.print(",");
        PHdis.print(lng_val);
        PHdis.print(",");
        PHdis.print(alt_m_val);
        PHdis.print(",");
        PHdis.print("");
        PHdis.print(",");
        PHdis.print(" ");
        PHdis.print(";");      
      }
    }
  }
  static void smartDelay(unsigned long ms)
  {
    unsigned long start = millis();
    do
    {
      while (GPS.available())
        gps.encode(GPS.read());
    } while (millis() - start < ms);
  }
