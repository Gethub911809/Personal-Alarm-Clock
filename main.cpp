#include <Arduino.h>
#include <Adafruit_GFX.h>
//#include <Adafruit_ST7789.h>
#include <Adafruit_ILI9341.h>
#include <SPI.h>

#define BL 6
#define SCREEN_CS 7
#define SCREEN_DC 8
#define SCREEN_RST 10
#define SCREEN_SDA 20
#define SCREEN_SCL 21

#define BUZZER 5

#define TOGGLEMODE_SW 4
#define OFF_SET 3
#define NEXT_INCREMENT 2
#define FORWARD 1
#define BACKWARD 0

//Adafruit_ST7789 SCREEN = Adafruit_ST7789(SCREEN_CS, SCREEN_DC, SCREEN_SDA, SCREEN_SCL, SCREEN_RST);
Adafruit_ILI9341 tft(SCREEN_CS, SCREEN_DC, SCREEN_RST);

int currentHour = 0;
int currentMinute = 0;

int alarmHour = 7;
int alarmMinute = 30;

bool alarmActive = false;

unsigned long lastTickMs = 0;
const unsigned long TICK_MS = 1000;

void updateClock();
void checkOffButton();
void drawScreen();

void setup() {
  Serial.begin(115200);

  pinMode(BL, OUTPUT);
  digitalWrite(BL, HIGH);

  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);

  pinMode(OFF_SET, INPUT_PULLUP);

  SPI.begin(SCREEN_SCL, -1, SCREEN_SDA, SCREEN_CS);
  tft.begin();
  tft.setRotation(3);


//   SCREEN.init(76, 284);; // SCREEN dimensions
//   SCREEN.setRotation(2); // this makes the SCREEN landscape, could be different, try 0-3
//   Serial.println("SCREEN Initialized!");
//   SCREEN.fillScreen(ST77XX_BLACK); // make sure there is nothing in the buffer

//   SCREEN.setCursor(0,0); // make the cursor at the top left


  tft.fillScreen(ILI9341_BLACK);
  drawScreen();
}

void loop() {
  checkOffButton();
  updateClock();

  if (currentHour == alarmHour && currentMinute == alarmMinute) {
    alarmActive = true;
  }

  if (alarmActive) {
    digitalWrite(BUZZER, HIGH);
  } else {
    digitalWrite(BUZZER, LOW);
  }

  drawScreen();
  delay(50);
}

void updateClock() {
  //Sped up so then the simulation runs faster 1min = 1 second
  if (millis() - lastTickMs < TICK_MS) {
    return;
  }

  lastTickMs = millis();
  currentMinute++;

  if (currentMinute >= 60) {
    currentMinute = 0;
    currentHour++;

    if (currentHour >= 24) {
      currentHour = 0;
    }
  }
}

void checkOffButton() {
  if (!digitalRead(OFF_SET)) {
    alarmActive = false;
    digitalWrite(BUZZER, LOW);
    delay(200);
  }
}

void drawScreen() {
  tft.fillScreen(ILI9341_BLACK);

  tft.setCursor(20, 40);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(3);

  tft.print("CURRENT TIME");

  tft.setCursor(40, 80);
  tft.setTextColor(ILI9341_RED);
  tft.setTextSize(4);

  if (currentHour < 10) tft.print("0");
  tft.print(currentHour);

  tft.print(":");

  if (currentMinute < 10) tft.print("0");
  tft.print(currentMinute);

  tft.setCursor(20, 140);
  tft.setTextColor(ILI9341_YELLOW);
  tft.setTextSize(2);

  tft.print("ALARM SET TO - ");

  if (alarmHour < 10) tft.print("0");
  tft.print(alarmHour);

  tft.print(":");
  
  if (alarmMinute < 10) tft.print("0");
  tft.print(alarmMinute);

  tft.setCursor(20, 180);
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(2);
  tft.print("PRESS OFF TO SILENCE");
}
