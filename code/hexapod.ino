#include <SPI.h>s
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <FastLED.h>
//IK
int legID;

int OffsetX; //leg position X offset
int OffsetY; //leg potion Y offset
int OffsetA; // angle offset

const int CoxaL = 44; // coxa lenght
const int FemurL = 60; // femur lenght
const int TibiaL = 107; // tibia lenght

//PINS
const int relay1 = 4;
const int relay2 = 7;

//VARIABLES
int light;

//start anim controller
bool startDone = false;

//-------------------------------LED NIGHTLIGHT-----------------------------------------------

CRGB leds1[5];
CRGB leds2[5];

//----------------------------Servo driver settings-------------------------------------------

Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x40); //servo driver setup
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x41);
#define SERVOMINcoxa  125
#define SERVOMAXcoxa  575
#define SERVOMINfemur  125
#define SERVOMAXfemur  575
#define SERVOMINtibia  125
#define SERVOMAXtibia  575

//----------------------------------NRF SETTINGS----------------------------------------------

RF24 radio(8, 9); // CE, CSN

const byte address[6] = "00001";

unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;

struct Data_Package { // data package sent to hexapod
  byte mode;
  byte height;
  byte joystick_x;
  byte joystick_y;
  byte joystick_select;
  byte option;
};
Data_Package data; //Create a variable with the above structure

struct Gait_creator { // data package sent to hexapod
  int time1;
  int time2;
  int time3;
  int time4;
  int time5;
  int time6;
};

//---------------------------------------GAIT TYPES------------------------------------------

Gait_creator trypod;

//////////////////////////////////////////SETUP///////////////////////////////////////////////

void setup() 
{
  Serial.begin(9600);

  FastLED.addLeds<WS2812B, 5, GRB>(leds1, 5);    //-----FAST LED
  FastLED.addLeds<WS2812B, 6, GRB>(leds2, 5);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
  FastLED.setBrightness(60);
  FastLED.clear();
  FastLED.show();

  radio.begin();                                 //----NRF transmission setup
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  
  pwm1.begin();                                  //-----servo driver setup
  pwm2.begin();
  pwm1.setPWMFreq(60);
  pwm2.setPWMFreq(60);
  yield();

//-----------------------------------------PIN SETUP----------------------------------------------

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(10, OUTPUT);
  digitalWrite(4, LOW);
  digitalWrite(7, LOW);

//-----------------------------------------GAIT INFORMATION---------------------------------------

//trypod gait
trypod.time1 = 1;
trypod.time2 = 2;
trypod.time3 = 3;
trypod.time4 = 4;
trypod.time5 = 5;
trypod.time6 = 6;

}

////////////////////////////////////////////////LOOP//////////////////////////////////////////////////

//----------------------------------------------Light sensor check/start anim-------------------------------------------
void loop()
{
  if (!startDone) {                    //start anim
    delay(500);
    startAnim();
    startDone = true;
  }                                   //light check

  light = analogRead(A3);
  //Serial.println(light);
  if (light < 15) {
    for (int i = 0; i < 5; i++) {
      leds1[i] = CRGB(255, 110, 0);
      leds2[i] = CRGB(255, 110, 0);
      FastLED.show();
    }
  }
  else {
    for (int i = 0; i < 5; i++) {
      leds1[i] = CRGB(0, 0, 0);
      leds2[i] = CRGB(0, 0, 0);
      FastLED.show();
    }
  }

//------------------------------------NRF COMMUNICATION----------------------------------------------

  if (radio.available())                    //read radio and reset data if connection is lost, prevent locking into a movement
  {
    //hello world test
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);

    //radio.read(&data, sizeof(Data_Package));
    lastReceiveTime = millis(); 
    //Serial.println(data.mode); Serial.println(data.height); 
    //Serial.println(data.joystick_x); 
    //Serial.println(data.joystick_y); Serial.println(data.joystick_select); Serial.println(data.option);
  }
  currentTime = millis();

  if ( currentTime - lastReceiveTime > 1000 ) 
  resetData();

//-----------------Mode select-------------------------

 switch(data.mode)
 {
  case 1: {walkMode(); break;}
  case 2: {crabMode(); break;}
  case 3: {bodymovementMode(); break;}
  case 4: {animationMode(); break;}
 }
}

void resetData()
{
  //innitial controller values
  data.mode = 127; //mode selection
  data.height = 127; // height offset
  data.joystick_x = 127;
  data.joystick_y = 127;
  data.joystick_select = 0; // joystick button
  data.option = 1; // the option for the current mode (1-3)
}

void startAnim()
{
  delay(10000);
  pwm1.setPWM(15, 0, 368);
  pwm1.setPWM(10, 0, 368);
  pwm1.setPWM(2, 0, 368);
  pwm2.setPWM(0, 0, 368);
  pwm2.setPWM(5, 0, 368);
  pwm2.setPWM(13, 0, 368);
  delay(2000);
  pwm1.setPWM(13, 0, 368);
  pwm1.setPWM(8, 0, 368);
  pwm1.setPWM(0, 0, 368);
  pwm2.setPWM(2, 0, 368);
  pwm2.setPWM(7, 0, 368);
  pwm2.setPWM(15, 0, 368);
  delay(2000);
  pwm1.setPWM(14, 0, 268);
  pwm1.setPWM(9, 0, 268);
  pwm1.setPWM(1, 0, 268);
  pwm2.setPWM(1, 0, 468);
  pwm2.setPWM(6, 0, 468);
  pwm2.setPWM(14, 0, 468);
  delay(2000);
}

void walkMode()
{
  switch(data.option)
  {
    case 1: data.joystick_x; break;
    case 2: data.joystick_y; break;
    case 3: data.height; break;
  }
}

void crabMode()
{

}

void bodymovementMode()
{
  IK();
}

void animationMode()
{

}

void IK()
{
  switch(legID)
  {
    case 1: {OffsetX = 64; OffsetY = 98; OffsetA = 45; break;}
    case 2: {OffsetX = 81; OffsetY = 0; OffsetA = 0; break;}
    case 3: {OffsetX = 64; OffsetY = -98; OffsetA = -45; break;}
    case 4: {OffsetX = -64; OffsetY = -98; OffsetA = -45; break;}
    case 5: {OffsetX = -81; OffsetY = 0; OffsetA = 0; break;}
    case 6: {OffsetX = -64; OffsetY = 98; OffsetA = 45; break;}
  }
}
