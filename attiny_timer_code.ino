#define Power_Switch 3     // Pin 2 on attiny 13A.
#define Battery_ADC_PORT A1  // Pin 2 acc. to Micro Core and Pin 7 on attiny 13A.
#define Low_Power_LED 1 // Pin 6 on attiny 13A.
#define Power_Enable 0 // Pin 5 on attiny 13A.

int bat_voltage = 0;

void setup()
{
  pinMode(Power_Enable, OUTPUT);
  pinMode(Low_Power_LED, OUTPUT);
  pinMode(Power_Switch, INPUT_PULLUP);
  digitalWrite(Power_Enable, LOW);
  digitalWrite(Low_Power_LED, LOW);
}

void loop()
{
  check_voltage();
  check_switch();
}

void check_voltage()
{
  bat_voltage = analogRead(Battery_ADC_PORT);
  if (bat_voltage < 700)
  {
    digitalWrite(Low_Power_LED, HIGH);
  }
  else
  {
    digitalWrite(Low_Power_LED, LOW);
  }
}

void check_switch()
{
  if (digitalRead(Power_Switch) == LOW)
  {
    delay(10);
    if (digitalRead(Power_Switch) == LOW)
    {
      digitalWrite(Power_Enable, HIGH);
      delay(3000);
      digitalWrite(Power_Enable, LOW);
    }
  }
  else
  {
    digitalWrite(Power_Enable, LOW);
  }
}

