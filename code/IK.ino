#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <math.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <FastLED.h>

bool START = true;

const float a = 60.0; //femur
const float b = 107.0; //tibia
const float l = 44.0; //coxa

const int relay1 = 4;
const int relay2 = 7;

CRGB leds1[5];
CRGB leds2[5];

int light;

Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x41);

int minPulses[18] = {113, 97, 130, 118, 87, 126, 105, 101, 117, 125, 113, 117, 126, 117, 117, 89, 134, 122};
int maxPulses[18] = {518, 502, 535, 523, 492, 531, 510, 506, 522, 530, 518, 522, 531, 522, 522, 494, 539, 527};

int pointsX[] = {175, 210, 175, -175, -210, -175};
int pointsY[] = {175, 0, -175, -175, 0, 175};

//------------------------------------------------------GAITs---------------------------------------------------------

int GAIT[3][8] = {
  {0, 0, 0, 0, 0, 0, 4, 4},   // Tripod
  {0, 0, 0, 0, 0, 0, 4, 4},    // Wave
  {0, 0, 0, 0, 0, 0, 4, 4}       // Ripple
};

//---------------------------------------------------Mode2 Variables--------------------------------------------------

unsigned long lastStep[6] = {0};
unsigned long stepDelay = 20;

int lastOption = 0;
int lastMode = 3;
int state[6];
int i[6];

//---------------------------------------------------receiverSetup----------------------------------------------------

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

//-------------------------------------------------------SETUP---------------------------------------------------------
void setup()
{
  Serial.begin(9600);
  
  //enable PCA servo drivers
  pwm1.begin();
  pwm2.begin();
  pwm1.setPWMFreq(50);
  pwm2.setPWMFreq(50);
  yield();

  //enable relays
  pinMode(4, OUTPUT);
  pinMode(7, OUTPUT);
  digitalWrite(4, LOW);
  digitalWrite(7, LOW);

  radio.begin();                                 //----NRF transmission setup
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();

  
  FastLED.addLeds<WS2812B, 5, GRB>(leds1, 5); //-----LED setup
  FastLED.addLeds<WS2812B, 6, GRB>(leds2, 5);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
  FastLED.setBrightness(60);
  FastLED.clear();
  FastLED.show();
}


//-------------------------------------------------------LOOP----------------------------------------------------------
void loop()
{
  if(START == true){
  startAnim();
  START = false;
  }

  if (radio.available())
  {
    radio.read(&data, sizeof(Data_Package));

    // Debug print
    //Serial.print("Mode: "); Serial.println(data.mode);
    //Serial.print("Height: "); Serial.println(data.height);
    //Serial.print("Joy X: "); Serial.println(data.joystick_x);
    //Serial.print("Joy Y: "); Serial.println(data.joystick_y);
    //Serial.print("Joy Btn: "); Serial.println(data.joystick_select);
    //Serial.print("Option: "); Serial.println(data.option);
  }
  
  light = analogRead(A3); // LED check
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

  switch(data.mode)
  {
    case 1: mode1(); break;
    case 2: mode2(); break;
    case 3: mode3(); break;
    case 4: mode4(); break;
  }
}

int mode1(){ // in development

}

int mode2() {
  if (data.option != lastOption) // checks for previous cycle
  lastMode = 0;
  lastOption = data.option;

  if (lastMode != 2) {
    state[0] = 1; i[0] = GAIT[data.option - 1][0]; // call selected GAIT
    state[1] = 2; i[1] = GAIT[data.option - 1][1];
    state[2] = 1; i[2] = GAIT[data.option - 1][2];
    state[3] = 2; i[3] = GAIT[data.option - 1][3];
    state[4] = 1; i[4] = GAIT[data.option - 1][4];
    state[5] = 2; i[5] = GAIT[data.option - 1][5];
    lastMode = 2;
  }

  legSwingC(1); // call leg swing
  legSwingC(2);
  legSwingC(3);
  legSwingC(4);
  legSwingC(5);
  legSwingC(6);
}

int mode3(){
  switch(data.option){
    case 1: {
      int H = map(data.height, 0, 255, 30, -30);       // option one, translation math
      int X = map(data.joystick_y, 0, 255, 40, -40);
      int Y = map(data.joystick_x, 0, 255, 40, -40);
      IK(175 + X, 175 + Y, -70 + H, 1);
      IK(210 + X,   0 + Y, -70 + H, 2);
      IK(175 + X, -175 + Y, -70 + H, 3);
      IK(-175 + X,-175 + Y, -70 + H, 4);
      IK(-210 + X,  0 + Y, -70 + H, 5);
      IK(-175 + X, 175 + Y, -70 + H, 6);
    break;
    }
    case 2: {
      int H = map(data.height, 0, 255, 30, -30);       // option one, Z rotation math
      float angle = map(data.joystick_y, 0, 255, 25, -25) * PI / 180.0;
      for(int i = 0; i < 6; i++){
        int X = pointsX[i];
        int Y = pointsY[i];
        int XR = X*cos(angle) - Y*sin(angle);
        int YR = X*sin(angle) + Y*cos(angle);
        IK(XR, YR, -70 + H, i+1);
      }
    break;
    }
    case 3: {
      int H = map(data.height, 0, 255, 0, -15);          // option one, X and Y dual rotation math
      float angleX = map(data.joystick_y, 0, 255, -10, 10) * PI / 180.0;
      float angleY = map(data.joystick_x, 0, 255, -10, 10) * PI / 180.0;
      for(int i = 0; i < 6; i++){
        int X = pointsX[i];
        int Y = pointsY[i];
        int Z = -70;

        int XR = X*cos(angleX) - Z*sin(angleX); //first rotation
        int ZR = X*sin(angleX) + Z*cos(angleX);

        int YR = Y*cos(angleY) - ZR*sin(angleY); //second rotation
        Z = Y*sin(angleY) + ZR*cos(angleY);

        IK(XR, YR, Z + H, i+1);
      }
    break;
    }
  }
}

int mode4(){ // in development

}


//-------------------------------------------------------IK------------------------------------------------------------
int IK(float y, float x, float z, int leg )
{
  float x0; // x rotation placeholder
  float y0; // y rotation placeholder

  // offsets and rotation matrixes
  switch(leg)
  {
    case 1: {y -= 64; x -= 98; x0 = x; y0 = y;
    x = x0*cos(PI/4) - y0*sin(PI/4);
    y = x0*sin(PI/4) + y0*cos(PI/4);
    break;}
    case 2: {y -= 81; x -= 0; break;}
    case 3: { y -= 64; x -= -98; x0 = x; y0 = y;
    x = x0*cos(7*PI/4) - y0*sin(7*PI/4);
    y = x0*sin(7*PI/4) + y0*cos(7*PI/4);
    break;}
    case 4: {y -= -64; x -= -98; y *= -1; x *= -1; x0 = x; y0 = y;
    x = x0*cos(PI/4) - y0*sin(PI/4);
    y = x0*sin(PI/4) + y0*cos(PI/4);
    break;}
    case 5: {y -= -81; x -= 0; y *= -1; x *= -1;  break;}
    case 6: {y -= -64; x -= 98; y *= -1; x *= -1; x0 = x; y0 = y;
    x = x0*cos(7*PI/4) - y0*sin(7*PI/4);
    y = x0*sin(7*PI/4) + y0*cos(7*PI/4);
    break;}
  }

  // IK equations
  float d = sqrt(x * x + y * y);
  float r = d - l;
  float c = sqrt(z * z + r * r);

  // Angles in degrees
  float theta1 = atan2(y, x) * 180.0 / PI;
  float theta2 = atan2(r,-z) * 180.0 / PI + acos((a * a + c * c - b * b) / (2 * a * c)) * 180.0 / PI;
  float theta3 = 180.0 - acos((a * a + b * b - c * c) / (2 * a * b)) * 180.0 / PI;
  //Serial.print("coxaT: "); Serial.println(theta1);
  //Serial.print("femurT: "); Serial.println(theta2);
  //Serial.print("tibiaT: "); Serial.println(theta3);

  // angles to write
  float coxaA;
  float femurA;
  float tibiaA;

  // femur and tibia flip based on right or left legs
  if(leg <=3){
  coxaA = theta1;
  femurA =  180.0 - theta2;
  tibiaA = 180.0 - theta3;
  writeAngles(coxaA, femurA, tibiaA, leg);
  //Serial.print("coxaA: "); Serial.println(coxaA);
  //Serial.print("femurA: "); Serial.println(femurA);
  //Serial.print("tibiaA: "); Serial.println(tibiaA);
  }
  else{
  coxaA = theta1;
  femurA = theta2;
  tibiaA = theta3;
  writeAngles(coxaA, femurA, tibiaA, leg);
  }
}
//-------------------------------------------------------writeAngles---------------------------------------------------
int writeAngles(float C, float F, float T, int leg)
{
  switch(leg){
    case 1:
    {
      pwm1.setPWM(15, 0, map(C, 0, 180, minPulses[0], maxPulses[0]) );
      pwm1.setPWM(14, 0, map(F, 0, 180, minPulses[1], maxPulses[1]) );
      pwm1.setPWM(13, 0, map(T, 0, 180, minPulses[2], maxPulses[2]) );
      break;
    }
    case 2:
    {
      pwm1.setPWM(10, 0, map(C, 0, 180, minPulses[3], maxPulses[3]) );
      pwm1.setPWM(9, 0, map(F, 0, 180, minPulses[4], maxPulses[4]) );
      pwm1.setPWM(8, 0, map(T, 0, 180, minPulses[5], maxPulses[5]) );
      break;
    }
    case 3:
    {
      pwm1.setPWM(2, 0, map(C, 0, 180, minPulses[6], maxPulses[6]) );
      pwm1.setPWM(1, 0, map(F, 0, 180, minPulses[7], maxPulses[7]) );
      pwm1.setPWM(0, 0, map(T, 0, 180, minPulses[8], maxPulses[8]) );
      break;
    }
    case 4:
    {
      pwm2.setPWM(13, 0, map(C, 0, 180, minPulses[9], maxPulses[9]) );
      pwm2.setPWM(14, 0, map(F, 0, 180, minPulses[10], maxPulses[10]) );
      pwm2.setPWM(15, 0, map(T, 0, 180, minPulses[11], maxPulses[11]) );
      break;
    }
    case 5:
    {
      pwm2.setPWM(5, 0, map(C, 0, 180, minPulses[12], maxPulses[12]) );
      pwm2.setPWM(6, 0, map(F, 0, 180, minPulses[13], maxPulses[13]) );
      pwm2.setPWM(7, 0, map(T, 0, 180, minPulses[14], maxPulses[14]) );
      break;
    }
    case 6:
   {
      pwm2.setPWM(0, 0, map(C, 0, 180, minPulses[15], maxPulses[15]) );
      pwm2.setPWM(1, 0, map(F, 0, 180, minPulses[16], maxPulses[16]) );
      pwm2.setPWM(2, 0, map(T, 0, 180, minPulses[17], maxPulses[17]) );
      break;
    }
  }
}
//------------------------------------------------------legSwingC------------------------------------------------------
int legSwingC(int legID) {

  legID--;
  int R = 40;
  int Y;
  int Z;

  if (millis() - lastStep[legID] >= stepDelay && (data.joystick_x > 132)){

    // ---------------- SEMICIRCLE ----------------
    if (state[legID] == 1){

      Y = i[legID];
      Z = sqrt(R * R - Y * Y);

      i[legID]+= 4;

      if (i[legID] == 40)
      state[legID] = 2;

      lastStep[legID] = millis();
      IK(pointsX[legID], pointsY[legID] + Y, -90 + Z, legID + 1);
    }

    // ---------------- STRAIGHT LINE ----------------
    else if (state[legID] == 2){

      Y = i[legID];
      Z = 0;

      i[legID]-= 8;
      
      if (i[legID] == -40)
        state[legID] = 1;

      lastStep[legID] = millis();

      IK(pointsX[legID], pointsY[legID] + Y, -90 + Z, legID + 1);
    }
  }
  if (millis() - lastStep[legID] >= stepDelay && (data.joystick_x < 120)){

    // ---------------- SEMICIRCLE ----------------
    if (state[legID] == 1){

      Y = i[legID];
      Z = sqrt(R * R - Y * Y);

      i[legID]-= 4;

      if (i[legID] == -40)
      state[legID] = 2;

      lastStep[legID] = millis();
      IK(pointsX[legID], pointsY[legID] + Y, -90 + Z, legID + 1);
    }

    // ---------------- STRAIGHT LINE ----------------
    else if (state[legID] == 2){

      Y = i[legID];
      Z = 0;

      i[legID]+= 8;
      
      if (i[legID] == 40)
        state[legID] = 1;

      lastStep[legID] = millis();

      IK(pointsX[legID], pointsY[legID] + Y, -90 + Z, legID + 1);
    }
  }




    if (millis() - lastStep[legID] >= stepDelay && (data.joystick_y > 132)){

    // ---------------- SEMICIRCLE ----------------
    if (state[legID] == 1){

      Y = i[legID];
      Z = sqrt(R * R - Y * Y);

      i[legID]+= 1;

      if (i[legID] == 40)
      state[legID] = 2;

      lastStep[legID] = millis();
      IK(pointsX[legID] + Y, pointsY[legID], -90 + Z, legID + 1);
    }

    // ---------------- STRAIGHT LINE ----------------
    else if (state[legID] == 2){

      Y = i[legID];
      Z = 0;

      i[legID]-= 1;
      
      if (i[legID] == -40)
        state[legID] = 1;

      lastStep[legID] = millis();

      IK(pointsX[legID] + Y, pointsY[legID], -90 + Z, legID + 1);
    }
  }
  if (millis() - lastStep[legID] >= stepDelay && (data.joystick_y < 120)){

    // ---------------- SEMICIRCLE ----------------
    if (state[legID] == 1){

      Y = i[legID];
      Z = sqrt(R * R - Y * Y);

      i[legID]-= 1;

      if (i[legID] == -40)
      state[legID] = 2;

      lastStep[legID] = millis();
      IK(pointsX[legID] + Y, pointsY[legID], -90 + Z, legID + 1);
    }

    // ---------------- STRAIGHT LINE ----------------
    else if (state[legID] == 2){

      Y = i[legID];
      Z = 0;

      i[legID]+= 1;
      
      if (i[legID] == 40)
        state[legID] = 1;

      lastStep[legID] = millis();

      IK(pointsX[legID] + Y, pointsY[legID], -90 + Z, legID + 1);
    }
  }

  lastMode = 2;
}

int startAnim(){ // first frame
  IK(240, 98, 20, 1);
  IK(240, 0, 20, 2);
  IK(240, -98, 20, 3);
  IK(-240, -98, 20, 4);
  IK(-240, 0, 20, 5);
  IK(-240, 98, 20, 6);
  delay(1000);   //second frame
  IK(190, 190, 0, 1);
  IK(220, 0, 0, 2);
  IK(190, -190, 0, 3);
  IK(-190, -190, 0, 4);
  IK(-220, 0, 0, 5);
  IK(-190, 190, 0, 6);
  delay(1000);   //push up frames sequence
  for(int i = 0; i >= -70; i -= 5)
{
  IK(175, 175, i, 1);
  IK(210,   0, i, 2);
  IK(175, -175, i, 3);
  IK(-175,-175, i, 4);
  IK(-210,  0, i, 5);
  IK(-175, 175, i, 6);
  delay(5);
}
delay(1000);
}