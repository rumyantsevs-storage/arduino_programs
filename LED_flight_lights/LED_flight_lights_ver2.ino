#include <cmath>

// ����� �������� ��������� � ��������������� ���� �������
#define IN1 2
#define IN2 3
#define IN3 4
#define IN4 5

// �������� ��� ����������� (����������) ��������
#define smooth 50
// ������ ������� �������
#define CCW -1
// �� ������� �������
#define CW 1
// ����. �������� �������������� �������
#define Kmax2 1000
#define Kmax 20

// �������� ���������� ��� ����������
#define CalSpeed 100

int SensMin[6] = {2000, 2000, 2000, 2000, 2000, 2000};
int SensMax[6] = {0};
int Sens[6] = {0};
float KK[6] = {0.0};
int k = 0;

int prevP = 0;
int P;
int I;

float Speed = 70;
float dSpeed = 1;
float minSpeed = 80;
float maxSpeed = 150;

// ������� �������� ������ �� ����� 
// direction - ����������� ��������, time - ����� ������ ����������
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
   // ����
   digitalWrite(IN1,LOW);
   analogWrite(IN2,0);
   digitalWrite(IN3,LOW);
   analogWrite(IN4,0);
}

void Calibration(int Pin1, int PinN) {

   Serial.println("Calibration start");

   int tempS; // ����� ��������� �������� ��� ����������� ��������� �� ���� �� ����� ����
   boolean black = false; // ���� ���� ������, �� �� �����
   boolean Cend = false; // ��������� ����������
   int Dir = CCW; // ����������� �������� ������
   int NL = 0; // ����� ����������� ����� ��� �������� �� ������� �������

   while (!Cend) {
     tempS = 0;
     for (int i = 0; i < PinN; i++)
     {
       int temp = analogRead(Pin1+i) / smooth;      // �� �������� "������" �������� �������
       if (temp < SensMin[i])  SensMin[i] = temp;
       if (temp > SensMax[i])  SensMax[i] = temp;
       tempS += temp / 3; // ����� ����� ����������� ��������� ��������
     }
     // ���� ����� �� �����
     if (tempS == 0) {
       // ���� ����� ���� ���� ������ ����� � ��������� ������ ������� �������, �� �������� ��������� �� �������
       if ((black) && (Dir == CCW) && (NL == 0)) {black = false; Dir = CW; NL++;}
       // ���� ����� ���� ���� ������ ���� ������ ����� � ��������� �� ������� �������, �� ���������� ���������
       if ((black) && (Dir == CW) && (NL == 1)) {black = false; NL++;}
       // ���� ����� ���� ���� "���" ������ ����� � ��������� �� ������� �������, �� ������������ � �������� ���������
       if ((black) && (Dir == CW) && (NL == 2)) {black = false; Dir = CCW; NL++;}
       // ���� ����� ���� ���� ��� "���" ������ ����� � ��������� ������ ������� �������, �� ���������� ���������
       if ((black) && (Dir == CCW) && (NL == 3)) {Cend = true;}
     } else {
       // ���� �� ������ (�.�. tempS <> 0)
       black = true;
     }
     Rotation(Dir, 40); // ������� � ������� 40 ��
   }
   // ��������� ������������ ����������
   for (int i = 0; i < PinN; i++) {
     KK[i] = (float)Kmax / (SensMax[i]-SensMin[i]);
     Serial.print(SensMin[i]);
     Serial.print(", ");
     Serial.print(SensMax[i]);
     Serial.print(", ");
     Serial.println(KK[i]);
   }

   Serial.println("Calibration end");
}

// ������������� ������ ����������
void CalibrationEmul2(int Pin1, int PinN) {
   KK[0]=Kmax * 1.25 / 20; SensMin[0]=0;
   KK[1]=Kmax * 1.67 / 20; SensMin[1]=0;
   KK[2]=Kmax * 1.33 / 20; SensMin[2]=0;
   KK[3]=Kmax * 1.54 / 20; SensMin[3]=0;
   KK[4]=Kmax * 1.82 / 20; SensMin[4]=0;
   KK[5]=Kmax * 1.25 / 20; SensMin[5]=0;
   Serial.println("CalibrationEmul completed");
}

void CalibrationEmul(int Pin1, int PinN) {
   KK[0]=1.25; SensMin[0]=0;
   KK[1]=1.67; SensMin[1]=0;
   KK[2]=1.33; SensMin[2]=0;
   KK[3]=1.54; SensMin[3]=0;
   KK[4]=1.82; SensMin[4]=0;
   KK[5]=1.25; SensMin[5]=0;
   Serial.println("CalibrationEmul completed");
}

// 1-�� �������
unsigned int sensorsRead(int i0, int i1, int i2, boolean white_line) {
  unsigned int sum; 
  int k0,k1,k2;

//  readCalibrated(sensor_values, readMode);
  /*
  012
  ---
  001 1
  002 2
  003 3
  013 4
  023 5
  033 6
  032 7
  031 8
  030 9
  130 10
  230 11
  330 12
  320 13
  310 14
  300 15
  */
  k0 = Sens[i0] / Kmax;
  k1 = Sens[i1] / Kmax;
  k2 = Sens[i2] / Kmax;
//  Serial.println("----");
//  Serial.println(k0);
//  Serial.println(k1);
//  Serial.println(k2);
  if (Sens[0]==0) sum = k2 * (Sens[i2]+Sens[i1]) + (1-k2) * Sens[i2] + k1 * (3*Sens[i1]-Sens[i2]);
  else            sum = k0 * (5*Sens[i0]-Sens[i1]) + (1-k0) * (3*Sens[i1]+Sens[i0]);
//  Serial.println(sum);
  return sum;
}

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN3, OUTPUT);
  digitalWrite(IN1, LOW);
  digitalWrite(IN3, LOW);
 
  Serial.begin(9600);
  
  delay(1000);        // ����� ��������� ������� ������ ����
  //Calibration(A0, 6); // ���������� ��������
  CalibrationEmul(A0, 6); // ���������� ��������
  delay(1000);        // ���� ����� ������� ������ � �������� � �������

}

void loop() {
  //Serial.println("Sensors:");
  for (int i = 0; i < 6; i++)
  {
     Sens[i] = KK[i] * ((analogRead(A0 + i) / smooth) - SensMin[i]);
     if (Sens[i] > Kmax-5) Sens[i] = Kmax;
     //Serial.println(Sens[i]);
  }

  // ��������� ������� � ���������� ��������� ����� ������������ ��:
  int posL = sensorsRead(0, 1, 2, false); // �����
  int posR = sensorsRead(5, 4, 3, false); // ����
  if (abs(posL) > abs(posR)) posR = 0;
  else if (abs(posR) > abs(posL)) posL = 0;

  P = 1.75 * (posR - posL);
  
  //I+=P;
  
  int D = P - prevP;
  
  int R = P; //+ I/100  + D;
  int tempL, tempR;

  // ������������ ��������:
  if (R >  Speed) R =  (int)Speed;
  if (R < -Speed) R = (int)-Speed;

  Serial.println(P);
  Serial.println(I);
  Serial.println(D);
  Serial.println("----");
  
  // ��������� �����������:
  
  if (((int)Speed + R) > 255) tempL = 255;
  else if (((int)Speed + R) < 0) tempL = 0;
  else tempL = (int)Speed + R;
  
  if (((int)Speed - R) > 255) tempR = 255;
  else if (((int)Speed - R) < 0) tempR = 0;
  else tempR = (int)Speed - R;
  
  Motors(tempL, tempR);

  prevP = P;
  
  //delay(400);
}

void Motors(int SpeedL, int SpeedR) {
  //digitalWrite(IN1,LOW);
  analogWrite(IN2, SpeedL);
  //digitalWrite(IN3,LOW);
  analogWrite(IN4, SpeedR);
  delay(1);
}
