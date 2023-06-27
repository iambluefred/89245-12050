/*
0xaa  8 40-84 WHEEL_SPEEDS
0x25  8 40-84 STEER_ANGLE_SENSOR
0xb4  8 40-84 SPEED
0x1c4 8 40-84 Engine rpm
0x1d2 8 40-84 PCM_CRUISE
0x1d3 8 40-84 PCM_CRUISE_2
0x4xb 8 40-84 ? 

0xaa  8 1a 6f 1a 6f 1a 6f 1a 6f
0xb4  8 00 00 00 00 00 00 00 bc
0x1c4 8 06 98 00 00 00 00 00 6b

0x1d2 8 30 00 00 00 00 00 80 8B << enable
        00 00 00 00 00 00 00 DB << disable

0x1d3 8 00 A8 3C 00 00 00 00 C0       
0x49b 8 1c d2 dd 30 00 00 42 00

Edited dy Fred.Wang
 */
#include <CAN.h>

int C_0x1C4[]={0x06,0x98,0x00,0x00,0x00,0x00,0x00,0x6b} ;
int C_0x49B[]={0x1c,0xd2,0xdd,0x30,0x00,0x00,0x42,0x00} ;

int C_0xAA[] ={0x36,0xd2,0x36,0xd2,0x36,0xd2,0x36,0xd2} ;
int C_0xB4[] ={0x00,0x00,0x00,0x00,0x00,0x1c,0xe6,0xbe} ;

int C_0x1D2_Enadle[]  ={0x30,0x00,0x00,0x00,0x00,0x00,0x80,0x8b} ;
int C_0x1D2_Disable[] ={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xdb} ;
int C_0x1D3[] ={0x00,0xa8,0x3c,0x00,0x00,0x00,0x00,0xc0} ;

int conuter_12mS  = 0; // 80HZ
int conuter_24mS  = 0; // 40HZ
int conuter_500mS = 0; // 2HZ
int buttonState   = 0;

void setup() {
  pinMode(7, INPUT);  // button I/O
  
  // start the CAN bus at 500 kbps
  if (!CAN.begin(500E3)) {
    // Serial.println("Starting CAN failed!");
    while (1);
  }

}

void loop() {

// 80HZ
 if (conuter_12mS >= 12){
     CanAddrDataLen(0xB4,C_0xB4,8);
     CanAddrDataLen(0x1C4,C_0x1C4,8);
     conuter_12mS = 0;
    }
// 40HZ
 if (conuter_24mS >= 24){
     buttonState = digitalRead(7); // Check button
     CanAddrDataLen(0xAA,C_0xAA,8);
     CanAddrDataLen(0x1D3,C_0x1D3,8);
     if (buttonState) {
        CanAddrDataLen(0x1D2,C_0x1D2_Enadle,8);
      }
     else {
        CanAddrDataLen(0x1D2,C_0x1D2_Disable,8);
      }
     conuter_24mS = 0;
    }
// 2HZ
 if (conuter_500mS >= 500){
     CanAddrDataLen(0x49B,C_0x49B,8);
     conuter_500mS = 0;
    }


  delay(1);
  conuter_12mS  = conuter_12mS + 1;
  conuter_24mS  = conuter_24mS + 1;
  conuter_500mS = conuter_500mS + 1;
}




int CanAddrDataLen (int addr, int *dat, char len) {
  char k ;
  k = len;
  for (int j = 0; j < len; j++) {
    char byte = (dat[j]);
  }
  CAN.beginPacket(addr);
  for (int ii = 0; ii < k; ii++) {
    CAN.write(dat[ii]);
  }
  CAN.endPacket();
}
