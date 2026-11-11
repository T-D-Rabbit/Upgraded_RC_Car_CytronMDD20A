#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

RF24 radio(33,32); // CE, CSN
// MOSI : 23
// MISO : 19
// SCK : 18
// SS : 5
//NOTE: Using ESP32 Dev Module.

const byte address[10] = "ADDRESS01"; //Note to self: Figure out this ADDRE business.


//Motor pins. Right-left pinout may not be accurate for user.
const int PWM_R = 13;   // right motor speed (PWM) 
const int DIR_R = 12;   // right motor direction
// const int IN2 = 14; Editing out normal code..
const int PWM_L = 27;  // left motor speed (PWM)
const int DIR_L = 26;   // left motor direction
// const int IN4 = 25; Editing out normal code..

//Bot is receiving this from the controller. If changing,
//make sure you change controller data package alongside.
struct Data_Package {
  int16_t Rx;
  int16_t Ly;
  int16_t Rpot;
  int16_t Lpot;
  bool RSw;
  bool LSw;
};

Data_Package data;
//

int deadZoneUpper;
int deadZoneLower;

unsigned long lastPacketTime = 0;
const unsigned long TIMEOUT_MS = 300; // tune as needed

void setup() 
{
//RadioSetupCode
  Serial.begin(9600);
  if (!radio.begin()) 
  {
    Serial.println("radio init FAILED");
    while (1);
  }
  radio.openReadingPipe(1, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

//MotorDriverCode
  pinMode(DIR_R, OUTPUT);

  pinMode(DIR_L, OUTPUT);

  ledcAttach(PWM_R, 1000, 8);   // pin, freq, resolution
  ledcAttach(PWM_L, 1000, 8);

  //

  //Change 512 to midpoint as required. Deadzone values will also need calibration.
  deadZoneUpper = 512 + 20;
  deadZoneLower = 512 - 10;

}

//Initial joystick values (0-1023 for me)
int axisX;
int axisY;

//Mapped joystick values
int mapX; // Mapping explained later in code
int mapY; // -255 to 255

int baseSpeed; //Essentially absolute value of mapY
int LSpeed; // Final value to input in left speed function
int RSpeed; // Final value to input in right speed function

void loop() 
{

  //Radio available else abandon as per TIMEOUT_MS
  if (radio.available()) 
  {
    radio.read(&data, sizeof(Data_Package));
    axisY = data.Ly;
    axisX = data.Rx;
    lastPacketTime = millis();
  }
  else if (millis() - lastPacketTime > TIMEOUT_MS) //midpoint
  {
    axisY = 512;
    axisX = 512;
    Lspd(0);
    Rspd(0);
    return;
  }
  //

  //Deadzone based speed setting for Y axis
  if ((axisY < deadZoneUpper) && (axisY > deadZoneLower))
  {
    baseSpeed = 0;
  }
  else //Map from 255 to -255
  {
    mapY = map(axisY,0,1023,255,-255);
    baseSpeed = abs(mapY);
  }
  //

  //Deadzone based speed setting for X axis
  if ((axisX < deadZoneUpper) && (axisX > deadZoneLower))
  {
    mapX = 0;
  }
  else
  {
    mapX = map(axisX,0,1023,(-2*baseSpeed),(2*baseSpeed));
    //Explanation: If my current speed is 100 (forward), mapX = 200 at max.
    //0 to 200 will be subtracted from either right or left wheel speed
    //Depending on X joystick value
  }
  //

  //Currently input for speed functions relies on X, Y is computed within
  //Will eventually change so that Y related things are also 
  //Computed outside the function
  if (mapX < 0)
  {
    Lspd(baseSpeed - abs(mapX));
    Rspd(baseSpeed);
  }
  else if (mapX > 0)
  {
    Lspd(baseSpeed);
    Rspd(baseSpeed - abs(mapX));
  }
  else
  {
    Lspd(baseSpeed);
    Rspd(baseSpeed);
  }
  //Note: Some of the comparison operator directions
  //Can be changed to match wiring.
}


void Lspd(int spd)
{
  if (mapY < 0)
  {

    if (spd > 0)
    {
      ledcWrite(PWM_L, spd);
      digitalWrite(DIR_L, HIGH);
    }
    else if (spd < 0)
    {
      ledcWrite(PWM_L, abs(spd));
      digitalWrite(DIR_L, LOW);
    }
    else //Stop..
    {
      ledcWrite(PWM_L, 0);
      digitalWrite(DIR_L,LOW);
    }
  }

  else if (mapY > 0)
  {

    if (spd < 0)
    {
      ledcWrite(PWM_L, spd);
      digitalWrite(DIR_L, HIGH);
    }
    else if (spd > 0)
    {
      ledcWrite(PWM_L, abs(spd));
      digitalWrite(DIR_L, LOW);
    }
    else
    {
      ledcWrite(PWM_L, 0);
      digitalWrite(DIR_L,HIGH);
    }
  }
}

void Rspd(int spd)
{
  if (mapY < 0)
  {

    if (spd > 0)
    {
      ledcWrite(PWM_R, spd);
      digitalWrite(DIR_R, HIGH);
    }
    else if (spd < 0)
    {
      ledcWrite(PWM_R, abs(spd));
      digitalWrite(DIR_R, LOW);
    }
    else
    {
      ledcWrite(PWM_R, 0);
      digitalWrite(DIR_R,HIGH);
    }
  }

  else if (mapY > 0)
  {

    if (spd < 0)
    {
      ledcWrite(PWM_R, spd);
      digitalWrite(DIR_R, HIGH);
    }
    else if (spd > 0)
    {
      ledcWrite(PWM_R, abs(spd));
      digitalWrite(DIR_R, LOW);
    }
    else
    {
      ledcWrite(PWM_R, 0);
      digitalWrite(DIR_R,HIGH);
    }
  }
}