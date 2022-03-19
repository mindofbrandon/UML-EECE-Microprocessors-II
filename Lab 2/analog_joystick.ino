int incomingByte = 0;

int joystick_x = A0;
int joystick_y = A1;

int xVal = 0;
int yVal = 0;

int x_axis_output = 0;
int y_axis_output = 0;

void setup()
{
 
  Serial.begin(9600); 
  pinMode(joystick_x, INPUT);
  pinMode(joystick_y, INPUT);
  
}

// Function to get joystick value and convert it to the direction that the snake should be going
char snake_direction()
{
  xVal = analogRead(joystick_x);          // Read VRx from joystick module
  yVal = analogRead(joystick_y);          // Read VRy from joystick module
  
  // Changed initial x and y values to get an output of 0 when joystick is stationary
  x_axis_output = map(xVal, 8, 1023, -512, 512);    
  y_axis_output = map(yVal, 46, 1023, 512, -512);    

  // If x-axis outupt is greater than 100 print "R" for right
  if (x_axis_output > 20)
  {
    Serial.print("R");
  }

  // Else iff x-axis outupt is less than -100 print "L" for left
  else if (x_axis_output < -20)
  {
    Serial.print("L");
  }

  // If y-axis outupt is greater than 100 print "U" for up
  if (y_axis_output > 20)
  {
    Serial.print("U");
  }

  // If y-axis outupt is less than -100 print "D" for down
  else if (y_axis_output < -20)
  {
    Serial.print("D");
  }
  
}

void loop()
{
  // Read from the Serial port:
  if (Serial.available() > 0)
  {
    // Read the incoming byte:
    incomingByte = Serial.read(); 
  }

  // Call to function snake_direction()
  Serial.print(snake_direction());
  
  delay(100);
}
