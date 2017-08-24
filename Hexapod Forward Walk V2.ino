
#include <SoftwareSerial.h>

//which pins go where on the arduino and minimaestro
#define PoltxPin 3
#define PolrxPin 12

//~~~~~~~~~~~~~~~~~~~~~//
//which leg is which port

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
//DEF - back + forward
#define shift_COX_ACDF 29
#define shift_COX_BE 20

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

//tibia (outer limb)
#define A_TIB 12
#define B_TIB 13
#define C_TIB 14
#define D_TIB 15
#define E_TIB 16
#define F_TIB 17

#define default_TIB 90

//~~~~~~~~~~~~~~~~~~~~~~~~~//

SoftwareSerial PolSerial(PolrxPin, PoltxPin);

  //set 1 - ACE; set2 - BDF
  
void setup() {
  // baud rate related
  //minimaestro
  PolSerial.begin(9600);
  delay(1000);
  pinMode(13,OUTPUT);

  set_Target(A_COX, default_A_COX);
  set_Target(B_COX, default_B_COX);
  set_Target(C_COX, default_C_COX);
  set_Target(D_COX, default_D_COX);
  set_Target(E_COX, default_E_COX);
  set_Target(F_COX, default_F_COX);
  delay(100);
  
  set_Target(A_TIB, default_TIB);
  set_Target(B_TIB, default_TIB);
  set_Target(C_TIB, default_TIB);
  set_Target(D_TIB, default_TIB);
  set_Target(E_TIB, default_TIB);
  set_Target(F_TIB, default_TIB);
  delay(100);

  //set2down
  set_Target(B_FEM, 110);
  set_Target(D_FEM, 70);
  set_Target(F_FEM, 70);

  delay(100);
  //set1 down
  set_Target(A_FEM, 110);
  set_Target(C_FEM, 110);
  set_Target(E_FEM, 70);
  
  delay(1000);

  //set1 up
  set_Target(A_FEM, default_A_FEM - shift_FEM);
  set_Target(C_FEM, default_C_FEM - shift_FEM);
  set_Target(E_FEM, default_E_FEM + shift_FEM);
  delay(100);
  //set1forward //slightly decreased distance, explained below
  set_Target(A_COX, default_A_COX - shift_COX_ACDF+4);
  set_Target(C_COX, default_C_COX - shift_COX_ACDF+4);
  set_Target(E_COX, default_E_COX + shift_COX_BE-3);
  delay(100);
  delay(100);
  //set1 down
  set_Target(A_FEM, default_A_FEM + shift_FEM+30);      //back-legs weaker
  set_Target(C_FEM, default_C_FEM + shift_FEM);
  set_Target(E_FEM, default_E_FEM - shift_FEM);
  delay(1000);
}

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

void loop() {
  
  //---part 1----//
  
  //set2up
  set_Target(B_FEM, default_B_FEM - shift_FEM);
  set_Target(D_FEM, default_D_FEM + shift_FEM);
  set_Target(F_FEM, default_F_FEM + shift_FEM);
  delay(100);
  
  //set1back (values 4 and 3 decrease distance travelled to account for weaker legs)
  set_Target(A_COX, default_A_COX + shift_COX_ACDF-4);
  set_Target(C_COX, default_C_COX + shift_COX_ACDF-4);
  set_Target(E_COX, default_E_COX - shift_COX_BE+3);
  delay(100);

  //set2forward
  set_Target(B_COX, default_B_COX - shift_COX_BE);
  set_Target(D_COX, default_D_COX + shift_COX_ACDF);
  set_Target(F_COX, default_F_COX + shift_COX_ACDF);
  delay(100);

  //set2down
  set_Target(B_FEM, default_B_FEM + shift_FEM);
  set_Target(D_FEM, default_D_FEM - shift_FEM);
  set_Target(F_FEM, default_F_FEM - shift_FEM-30);      //back-legs weaker
  delay(100);

  //-------part 2-----//
  
  //set1 up
  set_Target(A_FEM, default_A_FEM - shift_FEM);
  set_Target(C_FEM, default_C_FEM - shift_FEM);
  set_Target(E_FEM, default_E_FEM + shift_FEM);
  delay(100);

  //set2back
  set_Target(B_COX, default_B_COX + shift_COX_BE);  
  set_Target(D_COX, default_D_COX - shift_COX_ACDF);  
  set_Target(F_COX, default_F_COX - shift_COX_ACDF);  
  delay(100);  

  //set1forward //same thing here for decrease distance
  set_Target(A_COX, default_A_COX - shift_COX_ACDF+4);
  set_Target(C_COX, default_C_COX - shift_COX_ACDF+4);
  set_Target(E_COX, default_E_COX + shift_COX_BE-3);
  delay(100);

  //set1 down
  set_Target(A_FEM, default_A_FEM + shift_FEM+30);      //back-legs weaker
  set_Target(C_FEM, default_C_FEM + shift_FEM);
  set_Target(E_FEM, default_E_FEM - shift_FEM);
  delay(100);

}
