#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <ezButton.h>

RF24 radio(8, 9); // CE, CSN, for NRF module

const byte address[6] = "00001"; //NRF module adress

const int LED = 3; //status LED

ezButton button_j(7); // joystick button
ezButton button_1(4); // button 1
ezButton button_2(5); // button 2
ezButton button_3(6); // button 3

bool lastButtonJ = false; // logic check for buttons
bool lastButton1 = false;
bool lastButton2 = false;
bool lastButton3 = false;

struct Data_Package { // data package sent to hexapod
  byte mode;
  byte hight;
  byte joystick_x;
  byte joystick_y;
  byte joystick_select;
  byte option;

};

Data_Package data; //Create a variable with the above structure


void setup() 
{
  Serial.begin(9600);
  int debounce = 50; // debounce value
  button_1.setDebounceTime(debounce);button_2.setDebounceTime(debounce); // debounce assignment
  button_3.setDebounceTime(debounce);button_j.setDebounceTime(debounce); // debounce assignment

  pinMode(LED, OUTPUT);//power indicator LED
  digitalWrite(LED, HIGH);
  
  radio.begin();// NRF transmission setup
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

  //innitial controller values
  data.mode = 127; //mode selection
  data.hight = 127; // height offset
  data.joystick_x = 127;
  data.joystick_y = 127;
  data.joystick_select = 0; // joystick button
  data.option = 1; // the option for the current mode (1-3)

}

void loop() 
{
  button_1.loop(); //bytton loop call
  button_2.loop();
  button_3.loop();
  button_j.loop();

  if (button_j.isPressed() && !lastButtonJ) data.joystick_select = !data.joystick_select; // button rising edge detection
  if (button_1.isPressed() && !lastButton1) data.option = 1;
  if (button_2.isPressed() && !lastButton2) data.option = 2;
  if (button_3.isPressed() && !lastButton3) data.option = 3;
  lastButtonJ = button_j.isPressed();
  lastButton1 = button_1.isPressed();
  lastButton2 = button_2.isPressed();
  lastButton3 = button_3.isPressed();
  Serial.print("Joystick mode set to "); // serial monitor
  Serial.println(data.joystick_select);
  //Serial.println(data.option);

  
  data.mode = map(analogRead(A0), 0, 1023, 0, 255); // potentiometer value mapping
  data.hight = map(analogRead(A1), 0, 1023, 0, 255);
  data.joystick_x = map(analogRead(A2), 0, 1023, 0, 255);
  data.joystick_y = map(analogRead(A3), 0, 1023, 255, 0);
  //Serial.println(data.mode);// serial monitor
  //Serial.println(data.hight);
  Serial.println(data.joystick_x);
  Serial.println(data.joystick_y);

  radio.write(&data, sizeof(Data_Package)); // send info to hexapod
}
