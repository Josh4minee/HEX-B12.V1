#include <SPI.h>
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
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();
  
  pwm1.begin();                                  //-----servo driver setup
  pwm2.begin();
  pwm1.setPWMFreq(60);
  pwm2.setPWMFreq(60);
  yield();

//-----------------------------------------PIN SETUP----------------------------------------------

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(10, OUTPUT); // for proper NRF behavior
  digitalWrite(4, LOW);
  digitalWrite(7, LOW);

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
  {if (radio.available())
{
  radio.read(&data, sizeof(Data_Package));  // read struct directly
  lastReceiveTime = millis();

  // Debug print
  //Serial.print("Mode: "); Serial.println(data.mode);
  //Serial.print("Height: "); Serial.println(data.height);
  //Serial.print("Joy X: "); Serial.println(data.joystick_x);
  //Serial.print("Joy Y: "); Serial.println(data.joystick_y);
  //Serial.print("Joy Btn: "); Serial.println(data.joystick_select);
  //Serial.print("Option: "); Serial.println(data.option);
}
  }
  currentTime = millis();

  if ( currentTime - lastReceiveTime > 1000 ) 
  {
    //innitial controller values
    data.mode = 1; //mode selection
    data.height = 127; // height offset
    data.joystick_x = 127;
    data.joystick_y = 127;
    data.joystick_select = 0; // joystick button
    data.option = 1; // the option for the current mode (1-3)
  }

 selectMode();
}

void startAnim()
{

}

void selectMode(){
  switch(data.mode){
  case 1: walkMode(); break;
  case 2: crabMode(); break;
  case 3: bodymovementMode(); break;
  case 4: animationMode(); break;
 }
}

void walkMode()
{
  switch(data.option){
    case 1: ; break;
    case 2: ; break;
    case 3: ; break;
  }
}

void crabMode()
{
  switch(data.option){
    case 1: ; break;
    case 2: ; break;
    case 3: ; break;
  }
}

void bodymovementMode()
{
  switch(data.option){
    case 1: ; break;
    case 2: ; break;
    case 3: ; break;
  }
}

void animationMode()
{
  if(data.joystick_y > 200){
    switch(data.option){
      case 1: ; break;
      case 2: ; break;
      case 3: ; break;
    }
  }

}

void IK()
{

}