// Predefine pin values
int button = 2;                      
int buzzer = 10;
int red_led = 11;
int yellow_led = 12;
int green_led = 13;
int loop_index = 0;

// Define each state as boolean type
boolean state_1 = true;
boolean state_2 = false;
boolean state_3 = false;
boolean state_4 = false;

boolean blinking = false;

// Interrupt service routine (ISR) ... go here when button is pressed
void interrupt_service()
{
  state_1 = false;
  state_2 = true;
}

// Setup the component pinout and interrupt
void setup()
{
  pinMode(button, INPUT_PULLUP);       // Button
  pinMode(buzzer, OUTPUT);             // Buzzer
  pinMode(red_led, OUTPUT);            // Red LED
  pinMode(yellow_led, OUTPUT);         // Yellow LED
  pinMode(green_led, OUTPUT);          // Green LED

  // When there is a change in pin values [0,1] (button pressed) then move to the ISR
  attachInterrupt(digitalPinToInterrupt(button), interrupt_service, CHANGE);
}

// Flash red LED until button is pressed using an interrupt
// Constantly check for a button press -> trigger the ISR -> move to next state
void loop()
{
  while(true)
  {
    // State 1 ... Red LED blinks on and off for 1 second each until button is pressed -> move to State 2
    if(state_1 == true)
    {
      digitalWrite(red_led, HIGH);
      delay(1000);
      digitalWrite(red_led, LOW);
      delay(1000);
    }

    // State 2 ... Red LED turns on for 20 seconds and the buzzer turns on 3 seconds before moving to next state -> move to State 3
    else if(state_2 == true)
    {
      digitalWrite(red_led, HIGH);
      delay(17000);
      digitalWrite(buzzer, HIGH);
      delay(3000);
      digitalWrite(buzzer, LOW);
      digitalWrite(red_led, LOW);
      state_2 = false;
      state_3 = true;
    }
  
    // State 3 ... Green LED turns on for 20 seconds and the buzzer turns on 3 seconds before moving to next state -> move to State 4
    else if(state_3 == true)
    {
      digitalWrite(green_led, HIGH);
      delay(17000);
      digitalWrite(buzzer, HIGH);
      delay(3000);
      digitalWrite(buzzer, LOW);
      digitalWrite(green_led, LOW);
      state_3 = false;
      state_4 = true;
    }
  
    // State 4 ... Yellow LED turns on for 3 seconds then buzzer turns on while the LED blinks for 3 seconds -> move to State 2
    else if(state_4 == true)
    {
      digitalWrite(yellow_led, HIGH);
      delay(3000);
      digitalWrite(buzzer, HIGH);

      // blink yellow LED (200ms) for 3 seconds ... 3sec/0.2 = 15 times
      while(loop_index < 15){
        digitalWrite(yellow_led, blinking);
        delay(200);
        blinking = !blinking;
        loop_index++;
      }
        
      digitalWrite(buzzer, LOW);
      digitalWrite(yellow_led, LOW);
      state_4 = false;
      state_2 = true;
      loop_index = 0;
    }
  }
}
