#include "PS2X_lib.h"  //for v1.6
#include <Servo.h>

Servo myservo1;
Servo myservo2;

PS2X ps2x; // create PS2 Controller Class
int error = 0; 

int start_b,sel_b,up,down,left,right,L1,L2,R1,R2,tri,cir,cro,squ;
int ARx,ARy,ALx,ALy;
int UP,DOWN,LEFT,RIGHT,TRIANGLE,CIRCLE,CROSS,SQUARE,L1_B,L2_B,R1_B,R2_B;
int pre_sta = 0;
int fast = 0;

int IN1 = 3;
int IN2 = 5;
int IN3 = 6;
int IN4 = 11;

int PWM1,PWM2,PWM3,PWM4;
int PWM_max = 255;
int PWM_low = 100; 
int LED_red = A5;
int LED_green = A4;

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
  
  error = ps2x.config_gamepad(14,15,16,17, false, false);   //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error

  myservo1.attach(12);
  myservo2.attach(13);

  myservo1.write(170);
  myservo2.write(60);

  pinMode(LED_red,OUTPUT);
  pinMode(LED_green,OUTPUT);

  digitalWrite(LED_red,HIGH);
  digitalWrite(LED_green,LOW);
  
}

void loop() {
  // put your main code here, to run repeatedly:

  ps2x.read_gamepad();

  // Read from PS2 controller:
  start_b = ps2x.Button(PSB_START);
  up = ps2x.Button(PSB_PAD_UP);
  down = ps2x.Button(PSB_PAD_DOWN);
  left = ps2x.Button(PSB_PAD_LEFT);
  right = ps2x.Button(PSB_PAD_RIGHT);
  L1 = ps2x.Button(PSB_L1);
  L2 = ps2x.Button(PSB_L2);
  R1 = ps2x.Button(PSB_R1);
  R2 = ps2x.Button(PSB_R2);
  tri = ps2x.Button(PSB_TRIANGLE);
  cir = ps2x.Button(PSB_CIRCLE);
  cro = ps2x.Button(PSB_CROSS);
  squ = ps2x.Button(PSB_SQUARE);
  
  // Remap controller command:
  UP = up & right;
  DOWN = down & left;
  LEFT = left & L2;
  RIGHT = down & right;
  TRIANGLE = tri & cir;
  CIRCLE = cir & cro;
  CROSS = cro & squ;
  SQUARE = squ & !cro;
  L1_B = L1 & R1;
  L2_B = L2 & R2;
  R1_B = R1 & tri;
  R2_B = L1 & R2;

  // Robot arm servo controller:
  if(L1_B) myservo1.write(170);
  if(L2_B) myservo1.write(140);
  if(R1_B) myservo2.write(60);
  if(R2_B) myservo2.write(130);

  // Track wheel movement
  PWM1 = (!TRIANGLE & CROSS)|(!TRIANGLE & !LEFT & RIGHT);
  PWM2 = (TRIANGLE & !CROSS)|(!CROSS & LEFT & !RIGHT);
  PWM3 = (!TRIANGLE & CROSS)|(!TRIANGLE & LEFT & !RIGHT);
  PWM4 = (TRIANGLE & !CROSS)|(!CROSS & !LEFT & RIGHT);

  // PWM setting
  if((start_b - pre_sta) == 1){
    if(fast == 0){
      fast = 1;
      PWM_max = 255;
      PWM_low = 100; 
      digitalWrite(LED_red,HIGH);
      digitalWrite(LED_green,LOW);
      
    }
    else{
      fast = 0;
      PWM_max = 180;
      PWM_low = 100;
      digitalWrite(LED_red,LOW);
      digitalWrite(LED_green,HIGH);
    }
  }

  if(TRIANGLE & LEFT) PWM4*=PWM_low;
  else PWM4*=PWM_max;

  if(CROSS & RIGHT) PWM1*=PWM_low;
  else PWM1*=(PWM_max);

  if(TRIANGLE & RIGHT) PWM2*=PWM_low;
  else PWM2*=(PWM_max);

  if(CROSS & LEFT) PWM3*=PWM_low;
  else PWM3*=PWM_max;

  analogWrite(IN1,PWM1);
  analogWrite(IN2,PWM2);
  analogWrite(IN3,PWM3);
  analogWrite(IN4,PWM4);

}
