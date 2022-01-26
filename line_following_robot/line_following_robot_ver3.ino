// ����� �������� ��������� � ��������������� ���� �������
#DEFINE IN1 2
#DEFINE IN2 3
#DEFINE IN3 4
#DEFINE IN4 5

#DEFINE SMOOTH 50		// �������� ��� ����������� ��������
#DEFINE CW 1			// ����������� �� ������� �������
#DEFINE CCW -1			// ����������� ������ ������� �������
#DEFINE KMAX 20			// ����. �������� �������������� �������
#DEFINE CALSPEED 100		// �������� ���������� ��� ����������

int sensMin[7] = {0, 1023, 1023, 1023, 1023, 1023, 1023};
int sensMax[7] = {0};
int sens[7] = {0};
int i;
float cc[7] = {0.0};

/* ������� �������� ������ �� �����
dir -- ����������� ��������, time -- ����� ������ ���������� */
void rotation(int dir, int time)
{
	if (dir == CW)
	{
		digitalWrite(IN1, LOW);
		analogWrite(IN2, CALSPEED);
		digitalWrite(IN3, HIGH);
		analogWrite(IN4, 255 - CALSPEED);
	}
	else
	{
		digitalWrite(IN1, HIGH);
		analogWrite(IN2, 255 - CALSPEED);
		digitalWrite(IN3, LOW);
		analogWrite(IN4, CALSPEED);
	}
	delay(time); // ���������
	digitalWrite(IN1, LOW);
	analogWrite(IN2, 0);
	digitalWrite(IN3, LOW);
	analogWrite(IN4, 0);
}

// ������� ���������� ��������
void calibration(int pin1, int pinN)
{
	Serial.println("Calibration Start");

	/* ����� ��������� �������� ��� �����������
	��������� �� ���� �� ����� ���� */
	int temp, tempS;
	boolean black = false; // ������, �������� �� �����
	boolean cEnd = false; // ��������� ����������
	int dir = CCW; // ����������� �������� ������
	/* ����� ����������� ����� ��� ��������
	�� ������� ������� */
	int nl = 0;

	while (!cEnd)
	{
		tempS = 0;
		for (i = 0; i < pinN; i++)
		{
			temp = analogRead(pin1 + i) / SMOOTH; // ����������� �������� ��������� �������
			if (temp < sensMin[i + 1]) sensMin[i + 1] = temp;
			if (temp > sensMax[i + 1]) sensMax[i + 1] = temp;
			tempS += temp / 3; // ����� ���������� ���������� ��������� ��������
		}
		// ���� ������������ -- �� ����� ����
		if (tempS == 0)
		{
			/* ���� ����� ���� ����: 1 ������ ����� � �������� ������ ������� �������,
			��: ���������� �������� �� �������. */
			if ((black) && (dir == CCW) && (nl == 0))
			{
				black = false;
				dir = CW;
				nl++;
			}
			
			/* ���� ����� ���� ����: 1 ������ ����� � �������� �� ������� �������,
			��: ������������ ��������. */
			if ((black) && (dir == CW) && (nl == 1))
			{
				black = false;
				nl++;
			}
			
			/* ���� ����� ���� ����: 2 ������ ����� � �������� �� ������� �������,
			��: ����������� � �������� ���������. */
			if ((black) && (dir == CW) && (nl == 2))
			{
				black = false;
				dir = CCW;
				nl++;
			}
			
			/* ���� ����� ���� ����: ��� ������ ����� � �������� ������ ������� �������,
			��: ���������� ����������. */
			if ((black) && (dir == CCW) && (nl == 3))
				cEnd = true;
		}
		else
		{
			// ���� ������������ -- �� ������ ���� (�. �. tempS != 0)
			black = true;
		}
		rotation(dir, 40); // ������� �������� 100 ��
	}
	// ���������� ������������� ����������
	for (i = 0; i < pinN; i++)
	{
		cc[i + 1] = (float)KMAX / (sensMax[i + 1] - sensMin[i + 1]);
		Serial.print(sensMin[i + 1]);
		Serial.print(", ");
		Serial.print(sensMax[i + 1]);
		Serial.print(", ");
		Serial.println(cc[i + 1]);
	}
	Serial.println("Calibration End");
}

// ������� ���������� ��������� ����� ����������
void sensorsRead(int pin1, int pinN)
{
	int k;
	int r = 0;
	int m = 1;
	for (i = 1; i <= pinN; i++)
	{
		sens[i] = cc[i] * ((analogRead(pin1 + i - 1) / SMOOTH) - sensMin[i]);
		k = sens[i] % KMAX;
		r += k * (KMAX - sens[i - 1]) + (1 - k) * sens[i - 1];
		Serial.println(sens[i]);
	}
	Serial.println("-----");
	Serial.println(r);
	Serial.println("-----");
}

void setup()
{
	pinMode(IN1, OUTPUT);
	pinMode(IN3, OUTPUT);
	
	Serial.begin(9600);
	
	delay(1000);		// �������� ����� ���������
	calibration(A0, 6);
	delay(1000);		// �������� �� ������� (�������������) �������� ������
}

void loop()
{
	sensorsRead(A0, 6);
	delay(417);		// � ����� 144 bpm
}
