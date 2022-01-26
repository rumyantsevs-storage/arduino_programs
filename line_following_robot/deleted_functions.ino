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

void Calibration(short Pin1, short PinN)
{

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

void motorCtrl()
{
  if ((rSens > 5) || (lSens > 5)) // 5
    t = 1.12; // 1
  else if ((rSens > 11) || (lSens > 11)) // 6
    t = 1.24; // 2
  else if ((rSens > 18) || (lSens > 18)) // 7
    t = 1.48; // 3
  else if ((rSens > 26) || (lSens > 26)) // 8
    t = 1.96; // 4
  else if ((rSens > 35) || (lSens > 35)) // 9
    t = 2.92; // 5
  else if ((rSens > 45) || (lSens > 45)) // 10
  
  digitalWrite(IN1, LOW);
  analogWrite(IN2, CalSpeed + rSens * t - lSens);
  digitalWrite(IN3, LOW);
  analogWrite(IN4, CalSpeed + lSens * t - rSens);
}