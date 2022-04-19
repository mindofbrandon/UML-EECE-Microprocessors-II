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
const String C = "C"; // clockwise
const String CC = "CC"; // counter clockwise

bool state = HIGH;

DS3231 clock1;
RTCDateTime dt;
int i;

//initialize library with the numbers of the pins
LiquidCrystal lcd(7,8,9,10,11,12);


void interrupt_service() { // function to change direction
   state = !state;
    
    // Update directions
    Serial.println("change dir");
    digitalWrite(DIRA, state);
    digitalWrite(DIRB, !state);
    lcd.print("changing dir!");
}





void setup() {
  // put your setup code here, to run once:

  pinMode(button, INPUT_PULLUP);       // Button

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


  pinMode(ENABLE, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);
  Serial.begin(9600);
  
  lcd.begin(16,2); // setup columns and rows

  //lcd.print("Hello, World!");

   // setup ISR to detect button press to change fan rotation "C" = clockwise "CC" = counterclockwise
  // When there is a change in pin values [0,1] (button pressed) then move to the ISR
  attachInterrupt(digitalPinToInterrupt(button), interrupt_service, RISING);

}

void loop() {
  // put your main code here, to run repeatedly:

  dt = clock1.getDateTime();

  // For leading zero look to DS3231_dateformat example

  Serial.print("Raw data: ");
  Serial.print(dt.year);   Serial.print("-");
  Serial.print(dt.month);  Serial.print("-");
  Serial.print(dt.day);    Serial.print(" ");
  Serial.print(dt.hour);   Serial.print(":");
  Serial.print(dt.minute); Serial.print(":");
  Serial.print(dt.second); Serial.println("");


  


  delay(1000);


  
 
  //---PWM example, full speed then slow

  // if clock is at beginning of a minute, turn fan on for 30s
  if (dt.second == 0 || dt.second == 30) {
    Serial.println("Start The Motors!");

    // function to write fan speed and direction
    
    digitalWrite(DIRA, state);
    digitalWrite(DIRB, !state);
    analogWrite(ENABLE,70); //enable on
    
  }
  


  // setup ISR to update clock, rpm, and direction on LCD every second

 



}
