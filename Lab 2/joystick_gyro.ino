// In reference to: MPU-6050.ino, Analog_Joystick.ino from ELEGOO The Most Complete Starter Kit for MEGA V1.0.2021.05.13
#include<Wire.h>

const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t accelerometer_x, accelerometer_y;

  // Brandon's zones:
  // def,x defaulty = -3,38
  
  // ymin, ymax = -512,560
  // xmin, xmax = -520, 512


int incomingByte = 0;

int joystick_x = A0;
int joystick_y = A1;

int buzzer = 7;

int xVal = 0;
int yVal = 0;

int x_axis_output = 0;
int y_axis_output = 0;

void setup()
{
 
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
 
  Serial.begin(9600); 
 
  pinMode(joystick_x, INPUT);
  pinMode(joystick_y, INPUT);
  pinMode(buzzer, OUTPUT);
  
}

// Function to get joystick value and convert it to the direction that the snake should be going
char snake_direction_joystick()
{
  xVal = analogRead(joystick_x);          // Read VRx from joystick module
  yVal = analogRead(joystick_y);          // Read VRy from joystick module
  
  // Changed initial x and y values to get an output of 0 when joystick is stationary
  x_axis_output = map(xVal, 8, 1023, -512, 512);    
  y_axis_output = map(yVal, 46, 1023, 512, -512);    

  // If x-axis outupt is greater than 20 print "R" for right
  if (x_axis_output > 20)
  {
    Serial.print("R");
  }

  // Else iff x-axis outupt is less than -20 print "L" for left
  else if (x_axis_output < -20)
  {
    Serial.print("L");
  }

  // If y-axis outupt is greater than 20 print "U" for up
  if (y_axis_output > 20)
  {
    Serial.print("U");
  }

  // If y-axis outupt is less than -20 print "D" for down
  else if (y_axis_output < -20)
  {
    Serial.print("D");
  }
}

char snake_direction_gyro()
{
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  
  accelerometer_x=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  accelerometer_y=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)

  // If accelerometer x_axis val is greater than 2000 print "D_gyro"
  if (accelerometer_x > 2000)
  {
    Serial.print("D_gyro");
  }

  // Else if accelerometer x_axis val is less than -2000 print "U_gyro"
  else if (accelerometer_x < -2000)
  {
    Serial.print("U_gyro");
  }
  
  // If accelerometer y_axis val is greater than 2000 print "R_gyro"
  if (accelerometer_y > 2000){
    Serial.print("R_gyro");
  }

  // Else if accelerometer y_axis val is less than -2000 print "L_gyro"
  else if (accelerometer_y < -2000)
  {
    Serial.print("L_gyro");
  }
}

void loop()
{
  // Read from the Serial port:
  if (Serial.available() > 0)
  {
    // Read the incoming byte:
    incomingByte = Serial.read(); 
   
    // If the incoming byte icludes 'E' then turn on buzzer because the apple is eaten
    if (incomingByte == 'E'){
      digitalWrite(buzzer, HIGH);
    }
  }

  // Call to function snake_direction_joystick()
  Serial.print(snake_direction_joystick());
   
  // Call to function snake_direction_gyro() to receive snake direction from accelerometer
  Serial.print(snake_direction_gyro());
 
  delay(100);
 
  // Turn off buzzer after a 100ms delay
  digitalWrite(buzzer, LOW);
}
