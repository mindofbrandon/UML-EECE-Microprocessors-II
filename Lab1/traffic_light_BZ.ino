const byte ledRed = 13;
const byte ledYellow = 12;
const byte ledGreen = 11;
const byte buzzer = 10;
const byte interruptPin = 2;
volatile byte state = LOW;
String next = "";

void setup() {
  pinMode(ledRed, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);
}

void loop() {
  Serial.begin(9600);

  if ((state == LOW) && (next != "green") && (next != "yellow")){
    digitalWrite(ledRed, HIGH);
    delay(1000);
    digitalWrite(ledRed, LOW);
    delay(1000);
  }
  else if (state == HIGH) {
    digitalWrite(ledRed, HIGH);
    delay(17000);
    tone(buzzer, 500);
    delay(3000);
    noTone(buzzer);
    digitalWrite(ledRed, LOW);
    //delay(500);
    next = "green";
    state = LOW;
  }
  else if (next == "green") {
    digitalWrite(ledGreen, HIGH);
    delay(17000);
    tone(buzzer, 500);
    delay(3000);
    noTone(buzzer);
    digitalWrite(ledGreen, LOW);
    //delay(500);
    next = "yellow";
    state = LOW;
  }
  else if (next == "yellow") {
    digitalWrite(ledYellow, HIGH);
    delay(3000);
    digitalWrite(ledYellow, LOW);
    delay(200);
    digitalWrite(ledYellow, HIGH);
    delay(200);
    digitalWrite(ledYellow, LOW);
    delay(200);
    digitalWrite(ledYellow, HIGH);
    delay(200);
    digitalWrite(ledYellow, LOW);
    delay(200);
    digitalWrite(ledYellow, HIGH);
    delay(200);
    digitalWrite(ledYellow, LOW);
    delay(200);
    digitalWrite(ledYellow, HIGH);
    delay(200);
    digitalWrite(ledYellow, LOW);
    delay(200);
    digitalWrite(ledYellow, HIGH);
    delay(200);
    digitalWrite(ledYellow, LOW);
    delay(200);
    digitalWrite(ledYellow, HIGH);
    delay(200);
    digitalWrite(ledYellow, LOW);
    delay(200);
    digitalWrite(ledYellow, HIGH);
    delay(200);
    digitalWrite(ledYellow, LOW);
    delay(200);
    digitalWrite(ledYellow, HIGH);
    delay(200);
    digitalWrite(ledYellow, LOW);
    delay(200);
    tone(buzzer, 500);
    delay(3000);
    noTone(buzzer);
    //delay(500);
    state = HIGH;
    //next = "null";
  }
}

void blink() {
  state = HIGH;
}
