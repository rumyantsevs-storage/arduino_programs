int D=100;
int a=0;

void setup()
{
  for(int i=0; i<=11; i++)
    pinMode(i, OUTPUT);
  pinMode(12, INPUT);
  for(int i=0; i<=11; i++)
    digitalWrite(i, HIGH);
}

void loop()
{
  if(digitalRead(12))
    a+=1;
  if(a>1)
    a=0;    
  if(a==0)
  {
    for(int i=0; i<=10; i+=2)
    {
      digitalWrite(i, LOW);
      delay(D);
      digitalWrite(i, HIGH);
    }
    for(int i=11; i>=1; i-=2)
    {
      digitalWrite(i, LOW);
      delay(D);
      digitalWrite(i, HIGH);
    }
  }
  else
  {
    for(int i=0; i<=10; i+=2)
    {
      digitalWrite(i, HIGH);
      delay(D);
      digitalWrite(i, LOW);
    }
    for(int i=11; i>=1; i-=2)
    {
      digitalWrite(i, HIGH);
      delay(D);
      digitalWrite(i, LOW);
    }
  }
}

