#include <SoftwareSerial.h>
//which pins go where on the arduino and minimaestro
#define PoltxPin 3
#define PolrxPin 12

#define BTtxPin 2
#define BTrxPin 10

//~~~~~~~~~~~~~~~~~~~~~//
//which leg is which port

//set 1 - ACE; set2 - BDF
  
//coxa (closest to body)
#define A_COX 0
#define B_COX 1
#define C_COX 2
#define D_COX 3
#define E_COX 4
#define F_COX 5

#define default_A_COX 140
#define default_B_COX 96
#define default_C_COX 44
#define default_D_COX 140
#define default_E_COX 103
#define default_F_COX 50

//ABC + back - forward
//DEF - back + forward    //set 1 servos slightly stronger so values are lower than set 2
#define set1_shift_COX_ACDF 25
#define set1_shift_COX_BE 17
#define set2_shift_COX_ACDF 29
#define set2_shift_COX_BE 20

#define rotate_COX 10

//femur (middle)
#define A_FEM 6
#define B_FEM 7
#define C_FEM 8
#define D_FEM 9
#define E_FEM 10
#define F_FEM 11

#define default_A_FEM 90
#define default_B_FEM 90
#define default_C_FEM 85
#define default_D_FEM 98
#define default_E_FEM 95
#define default_F_FEM 90

//ABC - up + down
//DEF + up - down
#define shift_FEM 20
//servos A and F are weaker. Added value offsets body tilt
#define shift_FEM_AF_Down 50 

//tibia (outer limb)
#define A_TIB 12
#define B_TIB 13
#define C_TIB 14
#define D_TIB 15
#define E_TIB 16
#define F_TIB 17

#define default_TIB 90

//~~~~~~~~~~~~~~~~~~~~~~~~~//
SoftwareSerial BTSerial(BTrxPin, BTtxPin);
SoftwareSerial PolSerial(PolrxPin, PoltxPin);
  
void setup() {
  // baud rate related
  //minimaestro and BlueSmirf
   PolSerial.begin(9600);
  while(!Serial){}
  delay(1000);
  pinMode(13,OUTPUT);
   BTSerial.begin(115200);
   while(!Serial){}
  delay(1000);
}

void loop() {
  // Wait for a character to be received through BT
  while(BTSerial.available()<=0){
   // While you have not received anything , Do nothing 
  ;}
  
  /// Read Received Character
   char c= BTSerial.read();
   
   // Echo back the received character to confirm system is alive
   BTSerial.write(c); //start byte
   BTSerial.write('\r'); //start byte
   BTSerial.write('\n'); //start byte
   
  switch(c) {
    case 'n': 
      neutral();
      delay(100);
      break;
    case 'w': 
      walk_forwards();
      delay(100);
      break;
    case 's': 
      walk_backwards();
      delay(100);
      break;
    case 'a': 
      turn_left();
      delay(100);
      break;
    case 'd': 
      turn_right();
      delay(100);
      break;
    default:
      break;
  }
}

//~~~~~~~~~~~~~~functions~~~~~~~~~~~~~~~~~~//

//function for communication between arduino and pololu-minimaestro servos
void set_Target(unsigned char servo, unsigned int target)
{
  target = map(target, 0, 180, 2400, 9500);
  PolSerial.write(0xAA); //start byte
  PolSerial.write(0x0C) ; //device id
  PolSerial.write(0x04); //command number
  PolSerial.write(servo); //servo number
  PolSerial.write(target & 0x7F);
  PolSerial.write((target >> 7) & 0x7F);
}

//command functions-----------------------------
void neutral(){
  neutral_FEM();
  neutral_TIB();

  set1_up();
  set1_neutral_COX();
  set1_down();

  set2_up();
  set2_neutral_COX();
  set2_down();
}

void walk_forwards() {
  //readying for first step
  set1_up();
  set1_forward();
  set1_down();
  
  //actual walking
  set2_up();
  set1_back();
  set2_forward();
  set2_down();
  
  set1_up();
  set2_back();
  set1_forward();
  set1_down();

  neutral();
}

void walk_backwards(){
  //readying for first step
  set1_up();
  set1_back();
  set1_down();

  //actual walking
  set2_up();
  set1_forward();
  set2_back();
  set2_down();

  set1_up();
  set2_forward();
  set1_back();
  set1_down();

  neutral();
}

void turn_left(){
  set1_up();
  set1_rotate_left();
  set2_rotate_right(); //twist body left
  set1_down();

  set2_up();
  set2_rotate_left();
  set1_rotate_right(); //twist body left
  set2_down();

  neutral();
}

void turn_right(){
  set1_up();
  set1_rotate_right();
  set2_rotate_left(); //twist body right
  set1_down();

  set2_up();
  set2_rotate_right();
  set1_rotate_left(); //twist body right
  set2_down();

  neutral();
}
//neutral functions----------------------------
void set1_neutral_COX(){
  set_Target(A_COX, default_A_COX);
  set_Target(C_COX, default_C_COX);
  set_Target(E_COX, default_E_COX);
  delay(100);
}

void set2_neutral_COX(){
  set_Target(B_COX, default_B_COX);
  set_Target(D_COX, default_D_COX);
  set_Target(F_COX, default_F_COX);
  delay(100);
}

void neutral_FEM(){
  set1_down();
  set2_down();
}

void neutral_TIB() {
  set_Target(A_TIB, default_TIB);
  set_Target(B_TIB, default_TIB);
  set_Target(C_TIB, default_TIB);
  set_Target(D_TIB, default_TIB);
  set_Target(E_TIB, default_TIB);
  set_Target(F_TIB, default_TIB);
  delay(100);
}

//movement functions--------------------------------

void set1_up(){
  set_Target(A_FEM, default_A_FEM - shift_FEM);
  set_Target(C_FEM, default_C_FEM - shift_FEM);
  set_Target(E_FEM, default_E_FEM + shift_FEM);
  delay(100);
}

void set2_up(){
  set_Target(B_FEM, default_B_FEM - shift_FEM);
  set_Target(D_FEM, default_D_FEM + shift_FEM);
  set_Target(F_FEM, default_F_FEM + shift_FEM);
  delay(100);
}

void set1_down(){
  set_Target(A_FEM, default_A_FEM + shift_FEM_AF_Down);
  set_Target(C_FEM, default_C_FEM + shift_FEM);
  set_Target(E_FEM, default_E_FEM - shift_FEM);
  delay(100);
}

void set2_down(){
  set_Target(B_FEM, default_B_FEM + shift_FEM);
  set_Target(D_FEM, default_D_FEM - shift_FEM);
  set_Target(F_FEM, default_F_FEM - shift_FEM_AF_Down);
  delay(100);
}

void set1_forward(){
  set_Target(A_COX, default_A_COX - set1_shift_COX_ACDF);
  set_Target(C_COX, default_C_COX - set1_shift_COX_ACDF);
  set_Target(E_COX, default_E_COX + set1_shift_COX_BE);
  delay(100);
}

void set2_forward(){
  set_Target(B_COX, default_B_COX - set2_shift_COX_BE);
  set_Target(D_COX, default_D_COX + set2_shift_COX_ACDF);
  set_Target(F_COX, default_F_COX + set2_shift_COX_ACDF);
  delay(100);
}

void set1_back(){
  set_Target(A_COX, default_A_COX + set1_shift_COX_ACDF);
  set_Target(C_COX, default_C_COX + set1_shift_COX_ACDF);
  set_Target(E_COX, default_E_COX - set1_shift_COX_BE);
  delay(100);
}

void set2_back(){
  set_Target(B_COX, default_B_COX + set2_shift_COX_BE);  
  set_Target(D_COX, default_D_COX - set2_shift_COX_ACDF);  
  set_Target(F_COX, default_F_COX - set2_shift_COX_ACDF);  
  delay(100); 
}

void set1_rotate_left(){
  set_Target(A_COX, default_A_COX + rotate_COX);
  set_Target(C_COX, default_C_COX + rotate_COX);
  set_Target(E_COX, default_E_COX + rotate_COX);
  delay(100);
}

void set2_rotate_left(){
  set_Target(A_COX, default_A_COX + rotate_COX);
  set_Target(C_COX, default_C_COX + rotate_COX);
  set_Target(E_COX, default_E_COX + rotate_COX);
  delay(100);
}

void set1_rotate_right(){
  set_Target(A_COX, default_A_COX - rotate_COX);
  set_Target(C_COX, default_C_COX - rotate_COX);
  set_Target(E_COX, default_E_COX - rotate_COX);
  delay(100);
}

void set2_rotate_right(){
  set_Target(B_COX, default_B_COX - rotate_COX);
  set_Target(D_COX, default_D_COX - rotate_COX);
  set_Target(F_COX, default_F_COX - rotate_COX);
  delay(100);
}

