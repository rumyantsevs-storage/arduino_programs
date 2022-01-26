int d=75;

void setup()
{
  for(int i=0; i<=11; i++)
    pinMode(i, OUTPUT);
  pinMode(12, INPUT);
}

void loop()
{
  if(digitalRead(12))
  {
    delay(50);
    for(int i=0; i<=10; i+=2)
    {
      digitalWrite(i, HIGH);
      digitalWrite(i+1, HIGH);
      delay(d);
      digitalWrite(i, LOW);
      digitalWrite(i+1, LOW);
    }
  }
  else
  {
    delay(50);
    for(int i=11; i>=1; i-=2)
    {
      digitalWrite(i, HIGH);
      digitalWrite(i-1, HIGH);
      delay(d);
      digitalWrite(i, LOW);
      digitalWrite(i-1, LOW);
    }
  }
}

