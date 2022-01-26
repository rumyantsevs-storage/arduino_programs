int d = 200;
float c;
float f;

void setup()
{
  Serial.begin(9600);
  for(int i = 0; i <= 11; i++)
  {
    pinMode(i, OUTPUT);
  }
}

void loop()
{
  c = analogRead(A5);
  f = c * 5 / 1024;
  Serial.print(f); Serial.println(" B");
  delay(d);
  if(c != 0) {digitalWrite(0, HIGH); digitalWrite(1, HIGH);}
  else {digitalWrite(0, LOW); digitalWrite(1, LOW);}
  if(c > (1024 / 5)) {digitalWrite(2, HIGH); digitalWrite(3, HIGH);}
  else {digitalWrite(2, LOW); digitalWrite(3, LOW);}
  if(c > (1024 / 5 * 2)) {digitalWrite(4, HIGH); digitalWrite(5, HIGH);}
  else {digitalWrite(4, LOW); digitalWrite(5, LOW);}
  if(c > (1024 / 5 * 3)) {digitalWrite(6, HIGH); digitalWrite(7, HIGH);}
  else {digitalWrite(6, LOW); digitalWrite(7, LOW);}
  if(c > (1024 / 5 * 4)) {digitalWrite(8, HIGH); digitalWrite(9, HIGH);}
  else {digitalWrite(8, LOW); digitalWrite(9, LOW);}
  if(c > (1024 / 5 * 5)) {digitalWrite(10, HIGH); digitalWrite(11, HIGH);}
  else {digitalWrite(10, LOW); digitalWrite(11, LOW);}
}
