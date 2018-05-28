
void setup() {
  // put your setup code here, to run once:
pinMode(0, OUTPUT);
pinMode(1, INPUT_PULLUP);
digitalWrite(0, LOW);
}

void loop() {
  if(digitalRead(1) == LOW)
  {
    delay(10);
    if(digitalRead(1) == LOW)
    {
      digitalWrite(0, HIGH);
      delay(3000);
      digitalWrite(0, LOW);
    }
  }
  else
  {
    digitalWrite(0, LOW);
  }
}
