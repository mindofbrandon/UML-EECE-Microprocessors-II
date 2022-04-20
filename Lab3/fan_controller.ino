#include <DS3231.h>
#include <Wire.h>

#include <LiquidCrystal.h>
#define ENABLE 5
#define DIRA 3
#define DIRB 4

// Predefine pin values
int button = 2;  
const int offSpeed = 0;
const int halfSpeed = 127;
const int threeSpeed = 192;
const int fullSpeed = 255;
bool speedBottom;
bool speedTop;
int speedPrint;

// Variables will change:
int ledState = HIGH;         // the current state of the output pin
int buttonState = LOW;             // the current reading from the input pin
int lastButtonState = HIGH;  // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 5000;    // the debounce time; increase if the output flickers

bool state = HIGH;

DS3231 clock1;
RTCDateTime dt;
int i;

//initialize library with the numbers of the pins
LiquidCrystal lcd(7,8,9,10,11,12);


void interrupt_service() 
{ // function to change fan direction
   
   //sample the state of the button - is it pressed or not?
    buttonState = digitalRead(button);
    //Serial.print(buttonState);
    //filter out any noise by setting a time buffer
    if ( (millis() - lastDebounceTime) > debounceDelay) 
    {

      //if the button has been pressed, lets toggle the LED from "off to on" or "on to off"
      if (buttonState == HIGH) 
      {
        delay(50);
        state = !state;
    
      // Update directions
      //Serial.println("change dir");
      digitalWrite(DIRA, state);
      digitalWrite(DIRB, !state);
      //lcd.print("changing dir!");

      Serial.print("button pressed");
      }
  }
  //delay(500);
}





void setup() 
{

   
   // 1 second timer interrupt setup NOT WORKING
   /* FIXME timer interrupt creation not working
    cli();//stop interrupts

  //set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  sei();//allow interrupts
   
   
   */
   
  pinMode(button, INPUT);       // Button for change direction

  Serial.println("Initialize RTC module");
  // Initialize DS1307
  clock1.begin();

  // Manual (YYYY, MM, DD, HH, II, SS
  //clock.setDateTime(2016, 12, 9, 11, 46, 00);
  
  // Send sketch compiling time to Arduino
  clock1.setDateTime(__DATE__, __TIME__);    
  /*
  Tips:This command will be executed every time when Arduino restarts. 
       Comment this line out to store the memory of DS3231 module
  */

  // enable dc motor direction
  pinMode(ENABLE, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);
  digitalWrite(DIRA, state);
  digitalWrite(DIRB, !state);

  // serial monitor
  Serial.begin(9600);
  
  lcd.begin(16,2); // setup columns and rows

  
  // setup ISR to detect button press to change fan rotation "C" = clockwise "CC" = counterclockwise
  // When there is a change in pin values [0,1] (button pressed) then move to the ISR
  attachInterrupt(digitalPinToInterrupt(button), interrupt_service, CHANGE);

}

// FIXME add timer interrupt setup code
ISR(Timer1_COMPA_vect) // update display every second
{

  // print time
  lcd.clear(); // clear lcd before new print
  lcd.print("Time: ");
  lcd.print(dt.hour);   lcd.print(":");
  lcd.print(dt.minute); lcd.print(":");
  lcd.print(dt.second); lcd.print(" ");
  
  lcd.print(dt.second);

  // print clockwise or counterclockwise direction
  if (state) {
    lcd.print("C ");
  }
  else (!state) {
    lcd.print("CC ");
  }

  // print fan speed
  switch (speedPrint) 
  {
  case 0:
  lcd.print("0 ");
  break;
  
  case 1:
  lcd.print("1/2 ");
  break;
  
  case 2:
  lcd.print("3/4 ");
  break;
  
  case 3:
  lcd.print("Full ");
  break;
  }

  

  
}

void loop() 
{
  // put your main code here, to run repeatedly:

  
  
  dt = clock1.getDateTime();

  // For leading zero look to DS3231_dateformat example
 
  //---PWM example, full speed then slow

  // if clock is at beginning of a minute, turn fan on for 30s
  if (dt.second == 0 || dt.second == 30) // added 30 second interval to test dc motor on faster
  {
    Serial.println("Start The Motors!");

    // function to write fan speed and direction
    
    // function to write fan speed 
    if (dt.second == 30) 
    Serial.println("change fan speed");
    {
      speedBottom = !speedBottom;

      if (speedBottom) 
      {
        analogWrite(ENABLE, offSpeed);
        Serial.print("Speed is off");
        speedPrint = 0;
        //lcd.clear();
        //lcd.print("off");
      }
      else 
      {
        analogWrite(ENABLE, halfSpeed);
        Serial.print("Speed is half");
        speedPrint = 1;
        //lcd.clear();
        //lcd.print("half");
      }
    }

    if (dt.second == 0) 
    Serial.println("change fan speed");
    {
      speedTop = !speedTop;

      if (speedTop) 
      {
        analogWrite(ENABLE, threeSpeed);
        Serial.print("Speed is three fourths");
        speedPrint = 2;
        //lcd.clear();
        //lcd.print("3/4");
      }
      else 
      {
        analogWrite(ENABLE, fullSpeed);
        Serial.print("Speed is full");
        speedPrint = 3;
        
        //lcd.print("full");
      }
    }
  }
}
