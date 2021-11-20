#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <SPI.h>
#include <Wire.h>

#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
#include <SD.h>

RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2); // Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7)#include <SD.h>
const int RELAY = 7;

File myFile;

// change this to match your SD shield or module;
//     Arduino Ethernet shield: pin 4
//     Adafruit SD shields and modules: pin 10
//     Sparkfun SD shield: pin 8
const int chipSelect = 4;
#define DHTPIN 8      // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11 // DHT 22 (AM2302)
DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;
void setup()
{

    // Open serial communications and wait for port to open:
    Serial.begin(9600);
    dht.begin();

    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Sensor DHT11");

    while (!Serial)
    {
        ; // wait for serial port to connect. Needed for Leonardo only
    }

    Serial.print("Initializing SD card...");
    // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
    // Note that even if it's not used as the CS pin, the hardware SS pin
    // (10 on most Arduino boards, 53 on the Mega) must be left as an output
    // or the SD library functions will not work.
    pinMode(SS, OUTPUT);

    if (!SD.begin(chipSelect))
    {
        Serial.println("initialization failed!");
        return;
    }
    Serial.println("initialization done.");

    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.

    // ini adalah kode rtc
    if (!rtc.begin())
    {
        Serial.println("Couldn't find RTC");
        Serial.flush();
        while (1)
            delay(10);
    }

    if (rtc.lostPower())
    {
        Serial.println("RTC lost power, let's set the time!");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
}

void loop()
{
    // nothing happens after setup

    delay(delayMS);
    // Get temperature event and print its value.
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature))
    {
        Serial.println(F("Error reading temperature!"));
    }
    else
    {
        Serial.print(F("Temperature: "));
        Serial.print(event.temperature);
        Serial.println(F("°C"));
    }
    // Get humidity event and print its value.
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity))
    {
        Serial.println(F("Error reading humidity!"));
    }
    else
    {
        Serial.print(F("Humidity: "));
        Serial.print(event.relative_humidity);
        Serial.println(F("%"));
    }

    Serial.print("T :");
    Serial.print(event.temperature);
    Serial.print("*C , ");
    Serial.print("k :");
    Serial.print(event.relative_humidity);
    Serial.println("% , ");

    DateTime now = rtc.now();
    Serial.print(now.hour(), DEC);
    Serial.print(",");
    Serial.print(now.minute(), DEC);
    Serial.print(",");
    Serial.print(now.second(), DEC);
    Serial.println();

    // program layar lcd pertama
    lcd.setCursor(0, 0);
    lcd.print(now.hour(), DEC);
    lcd.print(":");
    lcd.print(now.minute(), DEC);
    lcd.print(":");
    lcd.print(now.second(), DEC);
    lcd.print("     ");
    delay(500);

    // ini adalah program layar lcd
    lcd.setCursor(0, 0);
    lcd.print("Suhu:");
    lcd.print(event.temperature);
    lcd.print("*C    ");
    // baris kedua
    lcd.setCursor(0, 1);
    lcd.print("Kel:");
    lcd.print(event.relative_humidity);
    lcd.print("%    ");
    delay(500);

    if (event.temperature > 35.00)
    {
        digitalWrite(RELAY, HIGH);
    }
    else
    {
        digitalWrite(RELAY, LOW);
    }

    myFile = SD.open("data.txt", FILE_WRITE);

    // if the file opened okay, write to it:
    if (myFile)
    {
        Serial.print("Writing to data.txt...");
        myFile.print(now.hour(), DEC);
        myFile.print(":");
        myFile.print(now.minute(), DEC);
        myFile.print(":");
        myFile.print(now.second(), DEC);
        myFile.print(":");
        myFile.print(event.temperature);
        myFile.print(":");
        myFile.print(event.relative_humidity);
        myFile.print(":");
        myFile.println("selesai !");
        myFile.close();
        Serial.println("done.");
    }
    else
    {
        // if the file didn't open, print an error:
        Serial.println("error opening data.txt");
    }
    delay(100);
}

// // // ini adalah kode full yang saya buat
// // .
// // .
// // .
// // .
// // .
// // .
// // .
// // .
// // .
// // .
// // .
// // .
// // .
// // .
// // ..

// #include <Arduino.h>
// #include <DHT.h>
// #include <SPI.h>
// #define DHTPIN 2
// #define DHTTYPE DHT11
// DHT dht(DHTPIN, DHTTYPE);
// #include <Wire.h>
// #include <LiquidCrystal_I2C.h>
// LiquidCrystal_I2C lcd(0x27, 16, 2);
// const int RELAY = 7;

// void setup()
// {
//     Serial.begin(9600);
//     dht.begin();
//     lcd.init();
//     lcd.backlight();
//     lcd.setCursor(0, 0);
//     lcd.print("Sensor DHT11");
//     pinMode(RELAY, OUTPUT);
// }

// void loop()
// {
//     lcd.clear();
//     float h = dht.readHumidity();
//     float t = dht.readTemperature();

//     // ini adalah penampilan pada lcd.
//     // baris pertama
//     lcd.setCursor(0, 0);
//     lcd.print("Suhu:");
//     lcd.print(t);
//     lcd.print("*C");
//     // baris kedua
//     lcd.setCursor(0, 1);
//     lcd.print("Kel:");
//     lcd.print(h);
//     lcd.print("%");

//     if (t > 35.00)
//     {
//         digitalWrite(RELAY, HIGH);
//     }
//     else
//     {
//         digitalWrite(RELAY, LOW);
//     }
//     // ini adalah penamilan data pada serial.
//     Serial.print("T :");
//     Serial.print(t);
//     Serial.println("*C");
//     Serial.print("k :");
//     Serial.print(h);
//     Serial.println("%");
//     delay(200);
// }