#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

RF24 radio(9, 8); // CE, CSN
const byte address[10] = "ADDRESS01";

//Radio stuff above. END.

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Screen stuff above. END.

#define POT_L  A6
#define POT_R  A7
#define JOY_Y A1
#define JOY_X A0
#define SW_LEFT    2
#define SW_RIGHT   3

struct Data_Package {
  int16_t Rx;
  int16_t Ly;
  int16_t Rpot;
  int16_t Lpot;
  bool RSw;
  bool LSw;
};

Data_Package data;

bool RF_OK;

void setup() {
  Serial.begin(9600);

//
  RF_OK = 1;
  if (!radio.begin()) 
  {
    RF_OK = 0;
  }
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

//Radio stuff above. END.

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.setTextColor(WHITE);

//Screen stuff above. END.

  pinMode(SW_LEFT, INPUT_PULLUP);
  pinMode(SW_RIGHT, INPUT_PULLUP);
}

void loop() 
{
  data.Rx = analogRead(JOY_X);
  data.Ly = analogRead(JOY_Y);
  data.Rpot = 1023 - analogRead(POT_R); //Accounting for wiring fault. To be wired correctly.
  data.Lpot = analogRead(POT_L);
  data.RSw = digitalRead(SW_RIGHT); //Wired accordingly.
  data.LSw = digitalRead(SW_LEFT); //Wired accordingly.

  if (RF_OK)
  {
    radio.write(&data, sizeof(Data_Package));
  };

  updateDisplay(data.Ly, data.Rx, data.Lpot, data.Rpot, data.LSw, data.RSw, 3.3, RF_OK, 10, 20);

  
}

void updateDisplay(int throttle, int steering, int pot3, int pot4, bool sw1, bool sw2, float batteryV, bool RF_OK, int dzLower, int dzUpper) 
{
  display.clearDisplay();
  display.setTextSize(1);


  display.setTextColor(SSD1306_WHITE);
  display.setCursor(2, 1);
  display.print(F("CONTROLLER"));

  display.setCursor(101, 1);
  display.print(F("3.3v"));

  display.setCursor(67, 1);
  display.print(RF_OK ? "RF:OK": "RF:NA");


  // Throttle & steering
  display.setCursor(67, 14);
  display.print(F("Rx:"));
  display.print(steering);

  display.setCursor(3, 14);
  display.print(F("Ly:"));
  display.print(throttle);

  // Potentiometer 3 & 4

  display.setCursor(3, 23);
  display.print(F("LPot:"));
  display.print(pot3);

  display.setCursor(67, 23);
  display.print(F("RPot:"));
  display.print(pot4);

  // Switch 1 & 2
  display.setCursor(3, 32);
  display.print(F("LSw:"));
  display.print(sw1 ? "ON" : "OFF");
  
  display.setCursor(67, 32);
  display.print(F("RSw:"));
  display.print(sw2 ? "ON" : "OFF");

  //lines

    //header
    display.drawLine(98, 0, 98, 9, SSD1306_WHITE);



    display.drawLine(0, 9, 127, 9, SSD1306_WHITE);
    //id: 1 line 52 
    display.drawLine(63, 0, 63, 63, SSD1306_WHITE);
    //id: 2 line 53 
    display.drawLine(64, 0, 64, 63, SSD1306_WHITE);
    //id: 3 line 59 
    display.drawLine(127, 10, 127, 63, SSD1306_WHITE);
    //id: 4 line 60 
    display.drawLine(0, 10, 0, 63, SSD1306_WHITE);
    //id: 5 line 69 
    display.drawLine(56, 10, 56, 63, SSD1306_WHITE);
    //id: 6 line 70 
    display.drawLine(120, 10, 120, 63, SSD1306_WHITE);
    //id: 7 line 71 
    display.drawLine(0, 9, 0, 0, SSD1306_WHITE);
    //id: 8 line 72 
    display.drawLine(127, 8, 127, 0, SSD1306_WHITE);
    //id: 9 line 73 
    display.drawLine(57, 10, 62, 15, SSD1306_WHITE);
    //id: 10 line 74 
    display.drawLine(62, 15, 57, 20, SSD1306_WHITE);
    //id: 11 line 75 
    display.drawLine(57, 20, 62, 25, SSD1306_WHITE);
    //id: 12 line 76 
    display.drawLine(62, 25, 57, 30, SSD1306_WHITE);
    //id: 13 line 77 
    display.drawLine(57, 30, 62, 35, SSD1306_WHITE);
    //id: 14 line 78 
    display.drawLine(62, 35, 57, 40, SSD1306_WHITE);
    //id: 15 line 79 
    display.drawLine(57, 40, 62, 45, SSD1306_WHITE);
    //id: 16 line 80 
    display.drawLine(62, 45, 57, 50, SSD1306_WHITE);
    //id: 17 line 81 
    display.drawLine(57, 50, 62, 55, SSD1306_WHITE);
    //id: 18 line 82 
    display.drawLine(62, 55, 57, 60, SSD1306_WHITE);
    //id: 19 line 83 
    display.drawLine(121, 10, 126, 15, SSD1306_WHITE);
    //id: 20 line 84 
    display.drawLine(126, 15, 121, 20, SSD1306_WHITE);
    //id: 21 line 85 
    display.drawLine(121, 20, 126, 25, SSD1306_WHITE);
    //id: 22 line 86 
    display.drawLine(126, 25, 121, 30, SSD1306_WHITE);
    //id: 23 line 87 
    display.drawLine(121, 30, 126, 35, SSD1306_WHITE);
    //id: 24 line 88 
    display.drawLine(126, 35, 121, 40, SSD1306_WHITE);
    //id: 25 line 89 
    display.drawLine(121, 40, 126, 45, SSD1306_WHITE);
    //id: 26 line 90 
    display.drawLine(126, 45, 121, 50, SSD1306_WHITE);
    //id: 27 line 91 
    display.drawLine(121, 50, 126, 55, SSD1306_WHITE);
    //id: 28 line 92 
    display.drawLine(126, 55, 121, 60, SSD1306_WHITE);
    //id: 29 line 93 
    display.drawLine(62, 10, 57, 15, SSD1306_WHITE);
    //id: 30 line 94 
    display.drawLine(57, 15, 62, 20, SSD1306_WHITE);
    //id: 31 line 95 
    display.drawLine(62, 20, 57, 25, SSD1306_WHITE);
    //id: 32 line 96 
    display.drawLine(57, 25, 62, 30, SSD1306_WHITE);
    //id: 33 line 97 
    display.drawLine(62, 30, 57, 35, SSD1306_WHITE);
    //id: 34 line 98 
    display.drawLine(57, 35, 62, 40, SSD1306_WHITE);
    //id: 35 line 99 
    display.drawLine(62, 40, 57, 45, SSD1306_WHITE);
    //id: 36 line 100 
    display.drawLine(57, 45, 62, 50, SSD1306_WHITE);
    //id: 37 line 101 
    display.drawLine(62, 50, 57, 55, SSD1306_WHITE);
    //id: 38 line 102 
    display.drawLine(57, 55, 62, 60, SSD1306_WHITE);
    //id: 39 line 103 
    display.drawLine(126, 10, 121, 15, SSD1306_WHITE);
    //id: 40 line 105 
    display.drawLine(121, 15, 126, 20, SSD1306_WHITE);
    //id: 41 line 106 
    display.drawLine(126, 20, 121, 25, SSD1306_WHITE);
    //id: 42 line 107 
    display.drawLine(121, 25, 126, 30, SSD1306_WHITE);
    //id: 43 line 108 
    display.drawLine(126, 30, 121, 35, SSD1306_WHITE);
    //id: 44 line 109 
    display.drawLine(121, 35, 126, 40, SSD1306_WHITE);
    //id: 45 line 110 
    display.drawLine(126, 40, 121, 45, SSD1306_WHITE);
    //id: 46 line 111 
    display.drawLine(121, 45, 126, 50, SSD1306_WHITE);
    //id: 47 line 112 
    display.drawLine(126, 50, 121, 55, SSD1306_WHITE);
    //id: 48 line 113 
    display.drawLine(121, 55, 126, 60, SSD1306_WHITE);
    //id: 49 line 114 
    display.drawLine(57, 60, 60, 63, SSD1306_WHITE);
    //id: 50 line 116 
    display.drawLine(121, 60, 124, 63, SSD1306_WHITE);
    //id: 51 line 118 
    display.drawLine(62, 60, 59, 63, SSD1306_WHITE);
    //id: 52 line 119 
    display.drawLine(59, 63, 59, 63, SSD1306_WHITE);
    //id: 53 line 125 
    display.drawLine(126, 60, 123, 63, SSD1306_WHITE);
    //id: 54 circle 130 
    display.drawCircle(1, 63, 7, SSD1306_WHITE);
    //id: 55 line 131 
    display.drawLine(4, 56, 6, 50, SSD1306_WHITE);
    //id: 56 line 132 
    display.drawLine(6, 58, 12, 54, SSD1306_WHITE);
    //id: 57 line 134 
    display.drawLine(8, 60, 14, 62, SSD1306_WHITE);


  display.display();
}