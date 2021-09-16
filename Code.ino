#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
String ssid     = "Simulator Wifi"; 
String password = ""; 
String host = "api.thingspeak.com"; 
const int httpPort   = 80;
String uri= "/update?api_key=ZXAQDPJSA6KLI8IG&field1=";

int red = 13;
int green = 6;
int yellow = 7;
int buzzer = 10;

int value=0;
int echoPin = 8; 
int trigPin = 9; 
int distance; 

int setupESP8266(void) {

  Serial.begin(115200);  
  Serial.println("AT");  
  delay(10);      
  if (!Serial.find("OK")) return 1;
  Serial.println("AT+CWJAP=\"" + ssid + "\",\"" + password + "\"");
  delay(10);     
  if (!Serial.find("OK")) return 2;
  Serial.println("AT+CIPSTART=\"TCP\",\"" + host + "\"," + httpPort);
  delay(50);    
  if (!Serial.find("OK")) return 3;
  return 0;
}

void anydata(void) {
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  distance = 0.01723 * pulseIn(echoPin, HIGH);
  
 
  String httpPacket = "GET " + uri + String(distance) + " HTTP/1.1\r\nHost: " + host + "\r\n\r\n"; 
  int length = httpPacket.length();
  Serial.print("AT+CIPSEND=");
  Serial.println(length);
  delay(10);
  Serial.print(httpPacket);
  delay(10);   
  if (!Serial.find("SEND OK\r\n")) return;
}

void setup() 
{ 
   Serial.begin(9600);
   setupESP8266(); 
   lcd.begin(16, 2);
  
   pinMode(red, OUTPUT);
   pinMode(yellow, OUTPUT);
   pinMode(green, OUTPUT);
  
   pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
   pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  
}

void loop() 
{
  lcd.setCursor(0,0);
  lcd.print("");
  lcd.setCursor(0,1);
  lcd.print("");
  anydata();
  delay(500);
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  distance = 0.01723 * pulseIn(echoPin, HIGH);

  
  if (distance > 250) {
    digitalWrite(red, LOW);
    digitalWrite(yellow, LOW);
    digitalWrite(green, LOW);
  }
  if (distance <= 250 && distance > 150) {
    digitalWrite(red, LOW);
    digitalWrite(yellow, LOW);
    digitalWrite(green, HIGH);
    lcd.setCursor(0,0);
    lcd.print("");
    lcd.setCursor(0,1);
    lcd.print("");
    lcd.setCursor(0,0);
    lcd.print("DUSTBIN EMPTY");
  }
  if (distance <= 150 && distance > 50) {
    digitalWrite(red, LOW);
    digitalWrite(yellow, HIGH);
    digitalWrite(green, LOW);
    lcd.setCursor(0,0);
    lcd.print("");
    lcd.setCursor(0,1);
    lcd.print("");
    lcd.setCursor(0,0);
    lcd.print("HALF_FILLED");
  }
  if (distance < 50) {
    digitalWrite(red, HIGH);
    digitalWrite(yellow, LOW);
    digitalWrite(green, LOW);
    lcd.setCursor(0,0);
    lcd.print("");
    lcd.setCursor(0,1);
    lcd.print("");
    lcd.setCursor(0,0);
    lcd.print("DUSTBIN FULL");
  }
}