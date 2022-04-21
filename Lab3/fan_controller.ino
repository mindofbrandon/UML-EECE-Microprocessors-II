#include <DS3231.h>
#include <Wire.h>

#include <LiquidCrystal.h>
#define ENABLE 5
#define DIRA 3
#define DIRB 4

// Predefine pin values
int button_direction = 2;  
int button_speed = 18;

int speed_level = 0;
int motor_speed = 0;

const int offSpeed = 0;
const int halfSpeed = 128;
const int threeSpeed = 192;
const int fullSpeed = 255;
bool speedBottom;
bool speedTop;
int speedPrint;

bool button_pressed = false;
bool button_pressed_speed = false;
bool cwise = true;
bool ccwise = false;

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long lastDebounceTime_speed = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 500;    // the debounce time; increase if the output flickers

bool state = HIGH;

DS3231 clock;

RTCDateTime dt;
int i;

//initialize library with the numbers of the pins
LiquidCrystal lcd(7,8,9,10,11,12);

/* Interrupt Service Routine (ISR) for direction change */
void interrupt_service_direction() 
{ 
    //filter out any noise by setting a time buffer
    if ( (millis() - lastDebounceTime) > debounceDelay) 
    {
      button_pressed = true;
      lastDebounceTime = millis();
      Serial.print("direction button pressed");
    }
}

/* Interrupt Service Routine (ISR) for speed change */
void interrupt_service_speed() 
{ 
    //filter out any noise by setting a time buffer
    if ( (millis() - lastDebounceTime) > debounceDelay) 
    {
      button_pressed_speed = true;
      lastDebounceTime_speed = millis();
      Serial.print("speed button pressed");
    }
}

/* Function to print the time on lcd screen */
void print_time()
{
  lcd.clear(); // clear lcd before new print
  lcd.print("Time: ");
  lcd.print(dt.hour);   lcd.print(":");
  lcd.print(dt.minute); lcd.print(":");
  lcd.print(dt.second); lcd.print(" ");
}

/* Function to print the direction of motor on lcd screen */
void print_direction()
{
  lcd.setCursor(0, 1);
  lcd.print("D:");
  // print clockwise or counterclockwise direction
  if (state==true)
  {
    lcd.print(" C");
  }
  else if(state==false)
  {
    lcd.print(" CC");
  }
}

/* Function to print the speed of motor on lcd screen */
void print_speed()
{
  lcd.print(" ");
  lcd.print("- S: ");
  
  if (speed_level == 0)
  {
    lcd.print("0 ");
  }
  else if (speed_level == 1)
  {
    lcd.print("3/4 ");
  }
  else if (speed_level == 2)
  {
    lcd.print("1/2 ");
  }
  else if (speed_level == 3)
  {
    lcd.print("Full ");
  }
}

/* Setup function */
void setup() 
{
  pinMode(button_direction, INPUT_PULLUP);   // Button for changing direction
  pinMode(button_speed, INPUT_PULLUP);       // Button for changing speed

  Serial.println("Initialize RTC module");
  
  // Initialize DS1307
  clock.begin();

  // Send sketch compiling time to Arduino
  clock.setDateTime(__DATE__, __TIME__);    

  // Enable dc motor direction
  pinMode(ENABLE, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);
  digitalWrite(DIRA, cwise);
  digitalWrite(DIRB, ccwise);
  delay(100);

  // Serial monitor
  Serial.begin(9600);
  
  lcd.begin(16, 2); // Setup columns and rows
  
  // Setup ISR to detect button press to change fan rotation "C" = clockwise "CC" = counterclockwise
  // When there is a change in pin values [0,1] (button pressed) then move to the ISR
  attachInterrupt(digitalPinToInterrupt(button_direction), interrupt_service_direction, CHANGE);
  attachInterrupt(digitalPinToInterrupt(button_speed), interrupt_service_speed, CHANGE);

}

 
/* Loop function */
void loop() 
{
  if(button_pressed == true)
  {
    digitalWrite(ENABLE, LOW);
    delay(500);

    cwise = !cwise;
    ccwise = !ccwise;
    
    digitalWrite(DIRA, cwise);  //turn off cw
    digitalWrite(DIRB, ccwise);   //turn on ccw
    
    state = !state;
    button_pressed = false;    
  }

  if (button_pressed_speed == true)
  {
    if (speed_level > 2){
      speed_level = 0;  // Reset index back to 0 
    }
    else{
      speed_level++;
    }

    /* Change the motor speed depending on the state */
    if (speed_level == 0){
      motor_speed = offSpeed;
    }
    else if (speed_level == 1){
      motor_speed = threeSpeed;
    }
    else if (speed_level == 2){
      motor_speed = halfSpeed;
    }
    else if (speed_level == 3){
      motor_speed = fullSpeed;
    } 

    delay(500); // debounce delay
    button_pressed_speed = false;
  }

  /* Print information on the lcd screen */
  print_time();
  print_direction();
  print_speed();

  dt = clock.getDateTime();

  // If clock is at beginning of a minute, turn fan on for 30s
  if (dt.second == 0) 
  {
    analogWrite(ENABLE, motor_speed);
    delay(30000);
  }
  
  else
  {
    analogWrite(ENABLE, offSpeed);
  }

  delay(500);
}
