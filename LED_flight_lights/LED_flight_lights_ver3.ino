// Входы драйвера двигателя и соответствующие пины адруины
#define IN1 2
#define IN2 3
#define IN3 4
#define IN4 5

// делитель для сглаживания (загубления) датчиков
#define smooth 50
// против часовой стрелки
#define CCW -1
// по часовой стрелке
#define CW 1
// макс. значение калиброванного датчика
#define Kmax 10

// скорость двигателей при калибровке
#define CalSpeed 70

short SensMin[7] = {0}; //{0, 1023, 1023, 1023, 1023, 1023, 1023};
short SensMax[7] = {0};
short Sens[7] = {0};
float KK[7] = {0, 1.25, 1.67, 1.33, 1.54, 1.82, 1.25};
float t = 1;
short k = 0;
short i;
short s = Kmax * 0.6;
short lSens;
short rSens;

// функция поворота робота на месте 
// direction - направление поворота, time - время работы двигателей
void Rotation(short dir, short time) {
   if (dir == CW)
   {
     digitalWrite(IN1, LOW);
     analogWrite(IN2, CalSpeed);
     digitalWrite(IN3, HIGH);
     analogWrite(IN4, 255 - CalSpeed);
   }
   else
   {
     digitalWrite(IN1, HIGH);
     analogWrite(IN2, 255 - CalSpeed);
     digitalWrite(IN3, LOW);
     analogWrite(IN4, CalSpeed);
   }
   delay(time);
   // стоп
   digitalWrite(IN1, LOW);
   analogWrite(IN2, 0);
   digitalWrite(IN3, LOW);
   analogWrite(IN4, 0);
}

void Calibration(short Pin1, short PinN) {

   Serial.println("Calibration start");

   short tempS; // сумма показаний датчиков для определения положения их всех на белом поле
   boolean black = false; // хоть один датчик, но на линии
   boolean Cend = false; // окончание калибровки
   short Dir = CCW; // направление поворота робота
   short NL = 0; // число пересечений линии при повороте по часовой стрелке

   while (!Cend) {
     tempS = 0;
     for (i = 0; i < PinN; i++)
     {
       short temp = analogRead(Pin1 + i) / smooth;      // не забываем "губить" точность датчика
       if (temp < SensMin[i+1])  SensMin[i+1] = temp;
       if (temp > SensMax[i+1])  SensMax[i+1] = temp;
       tempS += temp / 3; // сумма очень загубленных показаний датчиков
     }
     // если стоим на белом
     if (tempS == 0)
     {
       // если перед этим была черная линия и вращались против часовой стрелки, то начинаем вращаться по часовой
       if ((black) && (Dir == CCW) && (NL == 0)) {black = false; Dir = CW; NL++;}
       // если перед этим была только одна черная линия и вращались по часовой стрелки, то продолжаем вращаться
       if ((black) && (Dir == CW) && (NL == 1)) {black = false; NL++;}
       // если перед этим было "две" черных линии и вращались по часовой стрелки, то возвращаемся в исходное положение
       if ((black) && (Dir == CW) && (NL == 2)) {black = false; Dir = CCW; NL++;}
       // если перед этим были уже "все" черные линии и вращались против часовой стрелки, то калибровка завершена
       if ((black) && (Dir == CCW) && (NL == 3)) {Cend = true;}
     }
     else
     {
       // если на черном (т.е. tempS <> 0)
       black = true;
     }
     Rotation(Dir, 40); // поворот в течении 40 мс
   }
   // Вычисляем коэффициенты калибровки
   for (i = 0; i < PinN; i++) {
     KK[i+1] = float(Kmax) / (SensMax[i+1]-SensMin[i+1]);
     Serial.print(SensMin[i+1]);
     Serial.print(", ");
     Serial.print(SensMax[i+1]);
     Serial.print(", ");
     Serial.println(KK[i+1]);
   }

   Serial.println("Calibration end");
}

short sensorsRead(short Pin1, short PinN) {
  //short M = 1;
  for (i = 1; i <= PinN; i++)
  {
     Sens[i] = KK[i] * ((analogRead(Pin1 + i - 1) / smooth) - SensMin[i]);
     if (Sens[i]>Kmax) Sens[i]=Kmax;
     // k = Sens[i] % Kmax;
//     Serial.println(Sens[i]);
  }
  
  lSens = getSensors(3, 2, 1);
  rSens = getSensors(4, 5, 6);
  
//  Serial.println("------");
//  Serial.println(lSens);
//  Serial.println(rSens);
//  Serial.println("------");
}

short getSensors(short n1, short n2, short n3) {
  short r;
  /* 0 */      if ((Sens[n3] == 0) && (Sens[n2] == 0) && (Sens[n1] == 0))
    r = 0;
  /* 1 */ else if ((Sens[n3] == 0) && (Sens[n2] == 0) && (Sens[n1] != 0))
    r = Sens[n1] - Sens[0];
  /* 2 */ else if ((Sens[n3] == 0) && (Sens[n2] != 0) && (Sens[n1] >= s))
    r = Sens[n1] + Sens[n2];
  /* 3 */ else if ((Sens[n3] == 0) && (Sens[n2] >= s) && (Sens[n1] < Kmax))
    r = 3 * Sens[n2] - Sens[n1];
  /* 4 */ else if ((Sens[n3] != 0) && (Sens[n2] >= s) && (Sens[n1] == 0))
    r = 3 * Sens[n2] + Sens[n3];
  /* 5 */ else if ((Sens[n3] >= s) && (Sens[n2] < Kmax) && (Sens[n1] == 0))
    r = 5 * Sens[n3] - Sens[n2];
  /* 6 */ else if ((Sens[n3] != Kmax) && (Sens[n2] == 0) && (Sens[n1] == 0))
    r = 6 * Kmax - Sens[n3];
  /*6.1*/ else if ((lSens * rSens) >= -100)
    Serial.println("Stop");
  return r;
}

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN3, OUTPUT);
  
  Serial.begin(9600);
  
  delay(1000);        // после включения секунду просто ждем
  //Calibration(A0, 6); // Калибровка датчиков
  delay(1000);        // Надо будет сделать кнопку и ожидание её нажатия

}

void loop() {
  sensorsRead(A0, 6);
  digitalWrite(IN1, LOW);
  if ((rSens > 5) || (lSens > 5)) // 5
    t = 1.125; // 1
  else if ((rSens > 11) || (lSens > 11)) // 6
    t = 1.25; // 2
  else if ((rSens > 18) || (lSens > 18)) // 7
    t = 1.7; // 3
  else if ((rSens > 26) || (lSens > 26)) // 8
    t = 2.2; // 4
  else if ((rSens > 35) || (lSens > 35)) // 9
    t = 2.8; // 5
  else if ((rSens > 45) || (lSens > 45)) // 10
    t = 3.9; // 6
  else if ((rSens > 56) || (lSens > 56)) // 11
    t = 3.1; // 7
  else if ((rSens > 68) || (lSens > 68)) // 12
    t = 3.6; // 8
  analogWrite(IN2, CalSpeed + rSens * t - lSens);
  digitalWrite(IN3, LOW);
  analogWrite(IN4, CalSpeed + lSens * t - rSens);
  //delay(250);
}
