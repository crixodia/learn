#include <LiquidCrystal.h> // LCD library

#define TONE 100       // Frequency of the beep
#define TONE1 50       // Frequency of the timer
#define INTERVALO1 250 // Interval of the timer
#define INTERVALO 1000 // Interval of the beep

LiquidCrystal lcd(7, 8, 9, 10, 11, 12); // LCD pin definitions
int buzz = 6;                           // Pin buzzer
int relay = 5;                          // Pin relé

// Hours, minutes and seconds
int lcdHoras = 0;
int lcdMinutos = 0;
int lcdSegundos = 0;

int totalSegundos = 0; // Total seconds
int mensaje = 0;       // Message limit
// int start = A2;
int start = 4;
int empieza = 1024;

// Hours, minutes and seconds buttons
int btnHoras = A5;
int btnMinutos = A4;
int btnSegundos = A3;

// Button storage
int varbtnHoras = 0;
int varbtnMinutos = 0;
int varbtnSegundos = 0;

void setup()
{
  lcd.begin(16, 2);

  // Set up
  pinMode(relay, OUTPUT);
  pinMode(buzz, OUTPUT);
  pinMode(btnHoras, INPUT);
  pinMode(btnMinutos, INPUT);
  pinMode(btnSegundos, INPUT);

  // pinMode(start, INPUT);
  pinMode(start, INPUT_PULLUP);

  // digitalWrite(relay, LOW);

  mensaje = 0;
  empieza = 1024;

  varbtnHoras = 1;
  varbtnMinutos = 1;
  varbtnSegundos = 1;
}

void loop()
{
  if (mensaje == 0)
  {
    // Init mesage
    lcd.setCursor(0, 0);
    lcd.print("TEMPORIZADOR");
    lcd.setCursor(1, 1);
    lcd.print("ESPERE...");
    delay(2500);
    mensaje = 1;
    lcd.clear();
  }
  do
  {
    varbtnHoras = analogRead(btnHoras);       // Hour button
    varbtnMinutos = analogRead(btnMinutos);   // Minute button
    varbtnSegundos = analogRead(btnSegundos); // Second button

    if (varbtnHoras > 0) // Add hours
    {
      lcdHoras = lcdHoras + 1;
      delay(250);
    }

    if (varbtnMinutos > 0) // Add minutes
    {
      lcdMinutos = lcdMinutos + 1;
      if (lcdMinutos == 60)
        lcdMinutos = 0;
      delay(250);
    }

    if (varbtnSegundos > 0) // Add seconds
    {
      lcdSegundos = lcdSegundos + 1;
      if (lcdSegundos == 60)
        lcdSegundos = 0;
      delay(250);
    }

    lcd.setCursor(0, 0);
    lcd.print("Configure el tiempo");
    lcd.setCursor(4, 1);

    // Prints the time
    if (lcdHoras < 10)
      lcd.print("0");
    lcd.print(lcdHoras);
    lcd.print(":");
    if (lcdMinutos < 10)
      lcd.print("0");
    lcd.print(lcdMinutos);
    lcd.print(":");
    if (lcdSegundos < 10)
      lcd.print("0");
    lcd.print(lcdSegundos);
  } while (digitalRead(start) == HIGH);

  totalSegundos = lcdSegundos + (lcdMinutos * 60) + (lcdHoras * 60 * 60);
  digitalWrite(relay, HIGH);

  while (totalSegundos > 0)
  {
    delay(1000);

    totalSegundos--;
    lcdHoras = ((totalSegundos / 60) / 60);
    lcdMinutos = (totalSegundos / 60) % 60;
    lcdSegundos = totalSegundos % 60;
    lcd.setCursor(0, 0);
    lcd.print("Tiempo restante");
    lcd.setCursor(4, 1);

    if (lcdHoras < 10)
      lcd.print("0");

    lcd.print(lcdHoras);
    lcd.print(":");
    if (lcdMinutos < 10)
      lcd.print("0");

    lcd.print(lcdMinutos);
    lcd.print(":");
    if (lcdSegundos < 10)
      lcd.print("0");

    lcd.print(lcdSegundos);
    if (totalSegundos <= 30)
      sonarTono(TONE1, INTERVALO1);

    if (totalSegundos == 0)
    {
      while (1)
      {
        lcd.clear();
        lcd.setCursor(5, 0);
        lcd.print("Tiempo");
        lcd.setCursor(3, 1);
        lcd.print("Finalizado");

        digitalWrite(relay, LOW);
        sonarTono(TONE, INTERVALO);
        lcd.setCursor(0, 1);

        for (int i = 0; i < 16; ++i)
        {
          lcd.write(' ');
        }
        break;
      }
    }
  }
}

// Beep function
void sonarTono(int tono, int duracion)
{
  tone(buzz, tono, duracion);
  delay(duracion);
}
