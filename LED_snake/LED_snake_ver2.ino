int d=250;

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
    digitalWrite(i+1, HIGH);
    delay(d);
    digitalWrite(i-1, LOW);
    digitalWrite(i-2, LOW);
  }
  delay(d);
  for(int i=11; i>=1; i-=2)
  {
    digitalWrite(i, HIGH);
    digitalWrite(i-1, HIGH);
    delay(d);
    digitalWrite(i+1, LOW);
    digitalWrite(i+2, LOW);
  }
  delay(d);
}

