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
#define Kmax 20

// скорость двигателей при калибровке
#define CalSpeed 100

int SensMin[7] = {0, 1000, 1000, 1000, 1000, 1000, 1000};
int SensMax[7] = {0};
int Sens[7] = {0};
float KK[7] = {0.0};
int k = 0;

// функция поворота робота на месте 
// direction - направление поворота, time - время работы двигателей
void Rotation(int dir, int time) {
   if (dir == CW) {
     digitalWrite(IN1,LOW);
     analogWrite(IN2,CalSpeed);
     digitalWrite(IN3,HIGH);
     analogWrite(IN4,255-CalSpeed);
   } else {
     digitalWrite(IN1,HIGH);
     analogWrite(IN2,255-CalSpeed);
     digitalWrite(IN3,LOW);
     analogWrite(IN4,CalSpeed);
   }
   delay(time);
   // стоп
   digitalWrite(IN1,LOW);
   analogWrite(IN2,0);
   digitalWrite(IN3,LOW);
   analogWrite(IN4,0);
}

void Calibration(int Pin1, int PinN) {

   Serial.println("Calibration start");

   int tempS; // сумма показаний датчиков для определения положения их всех на белом поле
   boolean black = false; // хоть один датчик, но на линии
   boolean Cend = false; // окончание калибровки
   int Dir = CCW; // направление поворота робота
   int NL = 0; // число пересечений линии при повороте по часовой стрелке

   while (!Cend) {
     tempS = 0;
     for (int i = 0; i < PinN; i++)
     {
       int temp = analogRead(Pin1 + i) / smooth;      // не забываем "губить" точность датчика
       if (temp < SensMin[i+1])  SensMin[i+1] = temp;
       if (temp > SensMax[i+1])  SensMax[i+1] = temp;
       tempS += temp / 3; // сумма очень загубленных показаний датчиков
     }
     // если стоим на белом
     if (tempS == 0) {
       // если перед этим была черная линия и вращались против часовой стрелки, то начинаем вращаться по часовой
       if ((black) && (Dir == CCW) && (NL == 0)) {black = false; Dir = CW; NL++;}
       // если перед этим была только одна черная линия и вращались по часовой стрелки, то продолжаем вращаться
       if ((black) && (Dir == CW) && (NL == 1)) {black = false; NL++;}
       // если перед этим было "две" черных линии и вращались по часовой стрелки, то возвращаемся в исходное положение
       if ((black) && (Dir == CW) && (NL == 2)) {black = false; Dir = CCW; NL++;}
       // если перед этим были уже "все" черные линии и вращались против часовой стрелки, то калибровка завершена
       if ((black) && (Dir == CCW) && (NL == 3)) {Cend = true;}
     } else {
       // если на черном (т.е. tempS <> 0)
       black = true;
     }
     Rotation(Dir, 40); // поворот в течении 100 мс
   }
   // Вычисляем коэффициенты калибровки
   for (int i = 0; i < PinN; i++) {
     KK[i+1] = (float)Kmax / (SensMax[i+1]-SensMin[i+1]);
     Serial.print(SensMin[i+1]);
     Serial.print(", ");
     Serial.print(SensMax[i+1]);
     Serial.print(", ");
     Serial.println(KK[i+1]);
   }

   Serial.println("Calibration end");
}

int sensorsRead(int Pin1, int PinN) {
  int R = 0;
  int M = 1;
  for (int i = 1; i <= PinN; i++)
  {
     Sens[i] = KK[i] * ((analogRead(Pin1 + i - 1) / smooth) - SensMin[i]);
     k = Sens[i] % Kmax;
     Serial.println(Sens[i]);
  }
  Serial.println("------");
  Serial.println(R);
  Serial.println("------");
}

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN3, OUTPUT);
  
  Serial.begin(9600);
  
  delay(1000);        // после включения секунду просто ждем
  Calibration(A0, 6); // Калибровка датчиков
  delay(1000);        // Надо будет сделать кнопку и ожидание её нажатия

}

void loop() {
  int R = sensorsRead(A0, 6);
  delay(300);
}
