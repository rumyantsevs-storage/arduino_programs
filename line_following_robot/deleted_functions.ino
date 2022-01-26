// ������� �������� ������ �� ����� 
// direction - ����������� ��������, time - ����� ������ ����������
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
   // ����
   digitalWrite(IN1, LOW);
   analogWrite(IN2, 0);
   digitalWrite(IN3, LOW);
   analogWrite(IN4, 0);
}

void Calibration(short Pin1, short PinN)
{

   Serial.println("Calibration start");

   short tempS; // ����� ��������� �������� ��� ����������� ��������� �� ���� �� ����� ����
   boolean black = false; // ���� ���� ������, �� �� �����
   boolean Cend = false; // ��������� ����������
   short Dir = CCW; // ����������� �������� ������
   short NL = 0; // ����� ����������� ����� ��� �������� �� ������� �������

   while (!Cend) {
     tempS = 0;
     for (i = 0; i < PinN; i++)
     {
       short temp = analogRead(Pin1 + i) / smooth;      // �� �������� "������" �������� �������
       if (temp < SensMin[i+1])  SensMin[i+1] = temp;
       if (temp > SensMax[i+1])  SensMax[i+1] = temp;
       tempS += temp / 3; // ����� ����� ����������� ��������� ��������
     }
     // ���� ����� �� �����
     if (tempS == 0)
     {
       // ���� ����� ���� ���� ������ ����� � ��������� ������ ������� �������, �� �������� ��������� �� �������
       if ((black) && (Dir == CCW) && (NL == 0)) {black = false; Dir = CW; NL++;}
       // ���� ����� ���� ���� ������ ���� ������ ����� � ��������� �� ������� �������, �� ���������� ���������
       if ((black) && (Dir == CW) && (NL == 1)) {black = false; NL++;}
       // ���� ����� ���� ���� "���" ������ ����� � ��������� �� ������� �������, �� ������������ � �������� ���������
       if ((black) && (Dir == CW) && (NL == 2)) {black = false; Dir = CCW; NL++;}
       // ���� ����� ���� ���� ��� "���" ������ ����� � ��������� ������ ������� �������, �� ���������� ���������
       if ((black) && (Dir == CCW) && (NL == 3)) {Cend = true;}
     }
     else
     {
       // ���� �� ������ (�.�. tempS <> 0)
       black = true;
     }
     Rotation(Dir, 40); // ������� � ������� 40 ��
   }
   // ��������� ������������ ����������
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