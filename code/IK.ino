#include <math.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

bool START = true;

const float a = 60.0; //femur
const float b = 107.0; //tibia
const float l = 44.0; //coxa

const int relay1 = 4;
const int relay2 = 7;

Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x41);

int minPulses[18] = {113, 97, 130, 118, 87, 126, 105, 101, 117, 125, 113, 117, 126, 117, 117, 89, 134, 122};
int maxPulses[18] = {518, 502, 535, 523, 492, 531, 510, 506, 522, 530, 518, 522, 531, 522, 522, 494, 539, 527};


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
}


//-------------------------------------------------------LOOP----------------------------------------------------------
void loop()
{
  delay(10000);
  if(START == true){
  startAnim();
  START = !START;
  }
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

int startAnim(){
  IK(240, 98, 20, 1);
  IK(240, 0, 20, 2);
  IK(240, -98, 20, 3);
  IK(-240, -98, 20, 4);
  IK(-240, 0, 20, 5);
  IK(-240, 98, 20, 6);
  delay(2000);
  IK(190, 190, 0, 1);
  IK(220, 0, 0, 2);
  IK(190, -190, 0, 3);
  IK(-190, -190, 0, 4);
  IK(-220, 0, 0, 5);
  IK(-190, 190, 0, 6);
  delay(2000);
  for(int i = 0; i >= -60; i -= 5)
{
  IK(190, 190, i, 1);
  IK(220,   0, i, 2);
  IK(190, -190, i, 3);
  IK(-190,-190, i, 4);
  IK(-220,  0, i, 5);
  IK(-190, 190, i, 6);
  delay(5);
}
delay(2000);
}