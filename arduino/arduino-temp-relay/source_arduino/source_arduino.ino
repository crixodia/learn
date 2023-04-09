#include <LiquidCrystal.h>
#include <DHT.h>

#define DHTPIN 2      // DHT11 data pin
#define DHTTYPE DHT11 // Sensor type

const int rele = 3, led = 4; // Relay and LED pin
long varPot, varPot2;        // Variables for potentiometer

DHT dht(DHTPIN, DHTTYPE);               // DHT11 sensor
LiquidCrystal lcd(7, 8, 9, 10, 11, 12); // LCD pins

void setup()
{
  pinMode(rele, OUTPUT);
  pinMode(led, OUTPUT);
  dht.begin();
  lcd.begin(16, 2); // LCD 16x2
  lcd.setCursor(0, 0);
  lcd.write("Loading....");
}

void loop()
{
  delay(2000);
  varPot = analogRead(A0);  // Pot
  varPot2 = analogRead(A1); // Pot
  varPot2 = varPot2 / 20;
  varPot = varPot / 20;

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t))
  {
    return;
  }

  if (t <= varPot2)
  {
    digitalWrite(rele, HIGH);
    digitalWrite(led, HIGH);
  }
  else if (t >= varPot)
  {
    digitalWrite(rele, LOW);
    digitalWrite(led, LOW);
  }

  // Print temperature
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.write("Temperatura: ");
  lcd.setCursor(12, 0);
  lcd.print(t);
  lcd.setCursor(14, 0);
  lcd.write((char)223);
  lcd.setCursor(15, 0);
  lcd.write("C");

  lcd.setCursor(0, 1);
  lcd.write("MIN: ");
  lcd.setCursor(4, 1);
  lcd.print(varPot);
  lcd.setCursor(8, 1);
  lcd.print("MAX: ");
  lcd.print(varPot2);
  lcd.setCursor(14, 1);
}
