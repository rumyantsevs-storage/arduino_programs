float c = 261.63;
float d = 293.33;
float e = 329.63;
float f = 349.23;
float g = 392.0;
float a = 440.0;
float b = 493.88;

// A0: 490 ч - 40 б
// A3: 496 ч - 36 б

int n;
int nmax = 300;
float dd;

void setup()
{
  pinMode(A0, INPUT);
  pinMode(A3, INPUT);
  
  for (n = 3; n < 6; ++n)
    pinMode(n, OUTPUT);
  Serial.begin(9600);
}

void nota(int del)
{
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  delayMicroseconds(del);
  
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  delayMicroseconds(del);
}

void loop()
{
  //Serial.println(analogRead(A0)); Serial.println(analogRead(A3)); Serial.println("-----"); delay(500);

  if (analogRead(A3) > 200) n = 2; else n = 1;
  if (analogRead(A3) > nmax) nmax = analogRead(A3);
  
  /* тш */      if (analogRead(A0) < (nmax / 8 * 1)) dd = 0;
  /* до */ else if (analogRead(A0) < (nmax / 8 * 2)) dd = b;
  /* ре */ else if (analogRead(A0) < (nmax / 8 * 3)) dd = a;
  /* ми */ else if (analogRead(A0) < (nmax / 8 * 4)) dd = g;
  /* фа */ else if (analogRead(A0) < (nmax / 8 * 5)) dd = f;
  /* со */ else if (analogRead(A0) < (nmax / 8 * 6)) dd = e;
  /* ля */ else if (analogRead(A0) < (nmax / 8 * 7)) dd = d;
  /* си */ else if (analogRead(A0) < (nmax / 8 * 8)) dd = c;
  
  nota(dd * 1.612);
}

