#ifndef MPU_H
#define MPU_H

#include "tests.h"
#include "colors.h"
#include "magicstick.h"

// Motion motherfucker.
// TODO: #1 orientation check for pre-fire loop, etc
//       #2 ???
//       #3 profit!

// Fixme: Globals, ugly. 
float alpha, angle_x, angle_y, angle_z;

void motionLoop() {
  
  
  //angle_x = ;
  //angle_y = alpha*gyro_angle_y + (1.0 - alpha)*accel_angle_y;
  //angle_z = gyro_angle_z;  //Accelerometer doesn't give z-angle
  
  //drawSensorData(angle_x, angle_y, angle_z);
  #ifdef SERIAL_DEBUG
  Serial.print("X:");
  Serial.print(angle_x);
  Serial.print(", y:");
  Serial.print(angle_y);
  Serial.print(", z:");
  Serial.print(angle_z);
  Serial.print("\n");
  #endif
  
  // Update the saved data with the latest values
  //set_last_read_angle_data(millis(), angle_x, angle_y, angle_z, unfiltered_gyro_angle_x, unfiltered_gyro_angle_y, unfiltered_gyro_angle_z);
  // END MPU--------------------------------------------------
  
  // Globals
  updateTrickleUp(angle_y);
}

void motionTests() {  
  for (int x =0; x < 1000; x++){
    #ifndef NOMOTION
    motionLoop();
    #endif
    // Draw ring pattern based on MPU sensor
    // http://youtu.be/9qfS48hTFzU
    //ring(strip.Color(  angle_x,   angle_y, angle_z), 50, false, 25);
    
    // Used for calibrating batteries:
    // Notes: 2100 maH 11.1 v battery
    // 1) 4.02 original voltage
    // 2) 10 minutes of runtime
    // 3) 3.97 final voltage
    // 4) Brightness 20
    //fill(strip.Color(  angle_x,   angle_y, angle_z), 5);
    
    // Calibration demo loop
    //motionCalibration(angle_x, angle_y, angle_z);
  
    // Trickle effect  
    trickle(angle_x, angle_y, angle_z);
    if (isTrigger()) {
      return;
    }
  }
}
#endif
