#include "case4_main.h"
#include "main.h"


//global variables
uint8_t masterMACAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t cameraMACAddress[] = {0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
int camera_resx = 96;
int camera_resy = 96;
int camera_fov = 30;




//private variables
result_message resultData;
CommSystem commSystem;












void case5setup() {
 Serial.begin(115200);
 // put your setup code here, to run once:
 commSystem.init_comm();
}








bool scanTarget() {
 //Comms内の関数によってresultDataがアップデートされる
 commSystem.send_command();
 int attempts = 0;
 while(true) {
   if(commSystem.data_available == true) {
     resultData = commSystem.get_data();
     return true;
   }
   delay(500);
   if(attempts > 30) {
     Serial.println("timed out before receiving result data from camera");
     return false;
   }
   attempts += 1;
 }
}




void case5() {
 // put your main code here, to run repeatedly:
  float scanned_field =0;
  while(true) {
    if(scanTarget()) {
        Serial.println("Fount target. Direction adjusting...");
        break;
    }
    turn_right_angle(100, camera_fov);
    scanned_field += camera_fov;
    if(scanned_field >= 360) {
      break;
    }
  }
  int delta = resultData.x - (camera_resx/2);
  if(abs(delta) > 10) {
    if(delta < 0) {
      turn_left_angle(100, 10);
    } else {
      turn_right_angle(100, 10);
    }
  }
  delay(100);
}

















