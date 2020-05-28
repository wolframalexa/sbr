/*
   Arduino and MPU6050 Accelerometer and Gyroscope Sensor Tutorial
   by Dejan, https://howtomechatronics.com
*/

#include <Wire.h>

const int MPU = 0x68; // MPU6050 I2C address
const float LSB_SENSITIVITY_ACCEL = 16384.0; // for +-2g
const float LSB_SENSITIVITY_GYRO = 131.0;
float AccErrorX, AccErrorY, GyroErrorX, GyroErrorY, GyroErrorZ;

float AccX, AccY, AccZ;
float GyroX, GyroY, GyroZ;
float accAngleX, accAngleY, gyroAngleX, gyroAngleY, gyroAngleZ;
float roll, pitch, yaw;
float elapsedTime, currentTime, previousTime;
int c = 0;

void setup() {
  Serial.begin(19200);
  Wire.begin();                      // Initialize comunication
  Wire.beginTransmission(MPU);       // Start communication with MPU6050 // MPU=0x68
  Wire.write(0x6B);                  // Talk to the register 6B
  Wire.write(0x00);                  // Make reset - place a 0 into the 6B register
  Wire.endTransmission(true);        //end the transmission
  
  // Call this function if you need to get the IMU error values for your module
  calculate_IMU_error();
  delay(10000);

}

void loop() {
  // === Read acceleromter data === //
  Wire.beginTransmission(MPU);
  Wire.write(0x3B); // Start with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
  //For a range of +-2g, we need to divide the raw values by LSB_SENSITIVITY_ACCEL, according to the datasheet
  AccX = (Wire.read() << 8 | Wire.read()) / LSB_SENSITIVITY_ACCEL; // X-axis value
  AccY = (Wire.read() << 8 | Wire.read()) / LSB_SENSITIVITY_ACCEL; // Y-axis value
  AccZ = (Wire.read() << 8 | Wire.read()) / LSB_SENSITIVITY_ACCEL; // Z-axis value
  // Calculating Roll and Pitch from the accelerometer data
  
  accAngleY = (atan(-1 * AccX / sqrt(pow(AccY, 2) + pow(AccZ, 2))) * 180 / PI) + AccErrorY;

  // === Read gyroscope data === //
  previousTime = currentTime;        // Previous time is stored before the actual time read
  currentTime = millis();            // Current time actual time read
  elapsedTime = (currentTime - previousTime) / 1000; // Divide by 1000 to get seconds
  Wire.beginTransmission(MPU);
  Wire.write(0x43); // Gyro data first register address 0x43
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // Read 4 registers total, each axis value is stored in 2 registers
  GyroX = (Wire.read() << 8 | Wire.read()) / LSB_SENSITIVITY_GYRO; // For a 250deg/s range we have to divide first the raw value by 131.0, according to the datasheet
  GyroY = (Wire.read() << 8 | Wire.read()) / LSB_SENSITIVITY_GYRO;
  GyroZ = (Wire.read() << 8 | Wire.read()) / LSB_SENSITIVITY_GYRO;
  // Correct the outputs with the calculated error values

  GyroY = GyroY - GyroErrorY; 
  
  // Currently the raw values are in degrees per seconds, deg/s, so we need to multiply by sendonds (s) to get the angle in degrees
  gyroAngleY = gyroAngleY + GyroY * elapsedTime;
 
  // Complementary filter - combine acceleromter and gyro angle values
  pitch = 0.96 * gyroAngleY + 0.04 * accAngleY;

  // Print the values on the serial monitor
  Serial.print("gyroY: "); Serial.print(gyroAngleY);
  Serial.print("accY: "); Serial.print(accAngleY);
  Serial.print("pitch: "); Serial.print(pitch);
  Serial.println();

}


void calculate_IMU_error() {
  // We can call this funtion in the setup section to calculate the accelerometer and gyro data error. From here we will get the error values used in the above equations printed on the Serial Monitor.
  // Note that we should place the IMU flat in order to get the proper values, so that we then can the correct values
  // Read accelerometer values 200 times
  while (c < 200) {
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true); //2 bytes per register
    AccX = (Wire.read() << 8 | Wire.read()) / LSB_SENSITIVITY_ACCEL;
    AccY = (Wire.read() << 8 | Wire.read()) / LSB_SENSITIVITY_ACCEL;
    AccZ = (Wire.read() << 8 | Wire.read()) / LSB_SENSITIVITY_ACCEL;
    // Sum all readings
    AccErrorX = AccErrorX + ((atan((AccY) / sqrt(pow((AccX), 2) + pow((AccZ), 2))) * 180 / PI));
    AccErrorY = AccErrorY + ((atan(-1 * (AccX) / sqrt(pow((AccY), 2) + pow((AccZ), 2))) * 180 / PI));
    c++;
  }
  //Divide the sum by 200 to get the error value
  AccErrorX = AccErrorX / 200;
  AccErrorY = AccErrorY / 200;
  c = 0;
  // Read gyro values 200 times
  while (c < 200) {
    Wire.beginTransmission(MPU);
    Wire.write(0x43);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true);
    GyroX = Wire.read() << 8 | Wire.read();
    GyroY = Wire.read() << 8 | Wire.read();
    GyroZ = Wire.read() << 8 | Wire.read();
    // Sum all readings
    GyroErrorX = GyroErrorX + (GyroX / LSB_SENSITIVITY_GYRO);
    GyroErrorY = GyroErrorY + (GyroY / LSB_SENSITIVITY_GYRO);
    GyroErrorZ = GyroErrorZ + (GyroZ / LSB_SENSITIVITY_GYRO);
    c++;
  }
  //Divide the sum by 200 to get the error value
  GyroErrorX = GyroErrorX / 200;
  GyroErrorY = GyroErrorY / 200;
  GyroErrorZ = GyroErrorZ / 200;
  // Print the error values on the Serial Monitor
  Serial.print("AccErrorX: ");
  Serial.println(AccErrorX);
  Serial.print("AccErrorY: ");
  Serial.println(AccErrorY);
  Serial.print("GyroErrorX: ");
  Serial.println(GyroErrorX);
  Serial.print("GyroErrorY: ");
  Serial.println(GyroErrorY);
  Serial.print("GyroErrorZ: ");
  Serial.println(GyroErrorZ);
}
