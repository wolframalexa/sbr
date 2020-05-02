float target_ang = 0;
float now_ang = 0;
float now_error = 0;
float last_error = 0;

float Kp = 0;
float Ki = 0;
float Kd = 0;
float Kf = 0;

float P = 0;
float I = 0;
float D = 0;
float F = 0;

float error = 0;
float error_integral = 0;
float error_derivative = 0;

float output = 0;

unsigned long now_time, last_time, elapsed_time;

void setup() {


}

void loop() {

  current_time = millis();
  elapsed_time = (double)(current_time - previous_time);

  target_ang = 0; 
  now_ang = 0; //get tilt from accel+gyro

  now_error = target_ang - now_ang;

  P = Kp * now_error;

  error_integral += now_error;
  I = Ki * error_integral;

  error_derivative = (now_error - last_error) / elapsed_time;
  D = Kd * error_derivative;

  F = Kf * error;
  
  output = P + I + D + F;

  last_error = error;
  last_time = now_time;

  //set motor to output
  
}
