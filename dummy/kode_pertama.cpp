#include <Arduino.h>
#include <DHT.h>
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
const int RELAY = 7;

void setup()
{
  Serial.begin(9600);
  dht.begin();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Sensor DHT11");
  pinMode(RELAY, OUTPUT);
}

void loop()
{
  lcd.clear();
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // ini adalah penampilan pada lcd.
  // baris pertama
  lcd.setCursor(0, 0);
  lcd.print("Suhu:");
  lcd.print(t);
  lcd.print("*C");
  // baris kedua
  lcd.setCursor(0, 1);
  lcd.print("Kel:");
  lcd.print(h);
  lcd.print("%");

  if (t > 35.00)
  {
    digitalWrite(RELAY, HIGH);
  }
  else
  {
    digitalWrite(RELAY, LOW);
  }
  // ini adalah penamilan data pada serial.
  Serial.print("T :");
  Serial.print(t);
  Serial.println("*C");
  Serial.print("k :");
  Serial.print(h);
  Serial.println("%");
  delay(200);
}