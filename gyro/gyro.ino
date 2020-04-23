/* Keep track of gyro angle over time
 * Connect Gyro to Analog Pin 0
 *
 * Sketch by eric barch / ericbarch.com
 * v. 0.1 - simple serial output
 *
 */

int gyroPin = 0;               //Gyro is connected to analog pin 0
float gyroVoltage = 5;         //Gyro is running at 5V
float gyroZeroVoltage = 0.0;   //Gyro is zeroed upon start calibration
float gyroSensitivity = .007;  //Our example gyro is 7mV/deg/sec
float rotationThreshold = 1;   //Minimum deg/sec to keep track of - helps with gyro drifting // too high -- undershoots, but better than constantly incrementing

float currentAngle = 0;          //Keep track of our current angle
float avgGyroCal = 0;

void setup() {
  Serial.begin(9600);
  
  for(int i = 0; i < 10; i++) {
    float gyroCal = (analogRead(gyroPin) * gyroVoltage) / 1023;
    avgGyroCal += gyroCal;
    Serial.print("cal: ");
    Serial.println(gyroCal);
  }
  avgGyroCal /= 10;
  gyroZeroVoltage = avgGyroCal;
  
}

void loop() {

  
  //This line converts the 0-1023 signal to 0-5V
  float gyroRate = (analogRead(gyroPin) * gyroVoltage) / 1023;

  //This line finds the voltage offset from sitting still
  gyroRate -= gyroZeroVoltage;

  //This line divides the voltage we found by the gyro's sensitivity
  gyroRate /= gyroSensitivity;

  //Ignore the gyro if our angular velocity does not meet our threshold
  if (gyroRate >= rotationThreshold || gyroRate <= -rotationThreshold) {
    //This line divides the value by 100 since we are running in a 10ms loop (1000ms/10ms)
    gyroRate /= 100;
    currentAngle += gyroRate;
  }

  //Keep our angle between 0-359 degrees
  if (currentAngle < 0)
    currentAngle += 360;
  else if (currentAngle > 359)
    currentAngle -= 360;

  //DEBUG
  Serial.println(currentAngle);

  delay(10);
  
  //float gyroRate = (analogRead(gyroPin) * gyroVoltage) / 1023;
  //Serial.println(gyroZeroVoltage);
}
