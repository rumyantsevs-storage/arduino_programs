#include <cmath>

// Входы драйвера двигателя и соответствующие пины адруины
#define IN1 2
#define IN2 3
#define IN3 4
#define IN4 5

#define smooth 50 // делитель для сглаживания (загубления) датчиков
#define CCW -1    // против часовой стрелки
#define CW 1      // по часовой стрелке
#define Kmax 10   // макс. значение калиброванного датчика

#define minSpeed 70 // Минимальная cкорость двигателей

short SensMin[7] = {0}; //{0, 1023, 1023, 1023, 1023, 1023, 1023};
short SensMax[7] = {0};
short Sens[7] = {0};
float KK[7] = {0, 1.25, 1.67, 1.33, 1.54, 1.82, 1.25};
short i;
short lSens;
short rSens;
short k = 0;
float s = 0.6 * Kmax;
short r;
float dif = 1.25;

short sensorsRead(short Pin1, short PinN)
{
  for (i = 1; i <= PinN; i++)
  {
     Sens[i] = KK[i] * ((analogRead(Pin1 + i - 1) / smooth) - SensMin[i]);
     if (Sens[i] > Kmax) Sens[i] = Kmax;
     k = Sens[i] % Kmax;
     Serial.println(Sens[i]);
  }
  
  lSens = -ifAlgorithm(3, 2, 1);
  rSens = ifAlgorithm(4, 5, 6);
  r = lSens + rSens;
  
  Serial.println("------");
  Serial.println(r);
  Serial.println("------");
  delay(250);
}

short ifAlgorithm(short n1, short n2, short n3)
{
  short rrr;
  /* 0 */      if ((Sens[n3] == 0) && (Sens[n2] == 0) && (Sens[n1] == 0))
    rrr = 0;
  /* 1 */ else if ((Sens[n3] == 0) && (Sens[n2] == 0) && (Sens[n1] != 0))
    rrr = Sens[n1] - Sens[0];
  /* 2 */ else if ((Sens[n3] == 0) && (Sens[n2] != 0) && (Sens[n1] > s))
    rrr = Sens[n1] + Sens[n2];
  /* 3 */ else if ((Sens[n3] == 0) && (Sens[n2] > s) && (Sens[n1] < Kmax))
    rrr = 3 * Sens[n2] - Sens[n1];
  /* 4 */ else if ((Sens[n3] != 0) && (Sens[n2] > s) && (Sens[n1] == 0))
    rrr = 3 * Sens[n2] + Sens[n3];
  /* 5 */ else if ((Sens[n3] > s) && (Sens[n2] < Kmax) && (Sens[n1] == 0))
    rrr = 5 * Sens[n3] - Sens[n2];
  /* 6 */ else if ((Sens[n3] != Kmax) && (Sens[n2] == 0) && (Sens[n1] == 0))
    rrr = 6 * Kmax - Sens[n3];
  /*6.1*/ else if ((lSens * rSens) >= -100)
    Serial.println("Stop");
  return rrr;
}

void motorCtr()
{
  analogWrite(IN2, minSpeed + dif * abs(r));
  analogWrite(IN4, minSpeed + dif * abs(r));
}

void setup()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN3, LOW);
  
  pinMode(IN1, OUTPUT);
  pinMode(IN3, OUTPUT);
  
  Serial.begin(9600);
  
  delay(2000); // Ожидание 2 секунды (с кнопкой, или без)
}

void loop()
{
  sensorsRead(A0, 6);
  motorCtr();
}
