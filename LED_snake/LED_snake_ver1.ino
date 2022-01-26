int d=150;

void setup()
{
  for(int i=0; i<=11; i++)
  {
    pinMode(i, OUTPUT);
  }
}

void loop()
{
  for(int i=0; i<=10; i+=2)
  {
    digitalWrite(i, HIGH);
    delay(d);
    digitalWrite(i+2, HIGH);
    delay(d);
    digitalWrite(i, LOW);
  }
  for(int i=11; i>=1; i-=2)
  {
    digitalWrite(i, HIGH);
    delay(d);
    digitalWrite(i-2, HIGH);
    delay(d);
    digitalWrite(i, LOW);
  }
}

