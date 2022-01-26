// Входы драйвера двигателя и соответствующие пины Ардуино
#DEFINE IN1 2
#DEFINE IN2 3
#DEFINE IN3 4
#DEFINE IN4 5

#DEFINE SMOOTH 50		// Делитель для сглаживания датчиков
#DEFINE CW 1			// Направление по часовой стрелке
#DEFINE CCW -1			// Направление против часовой стрелки
#DEFINE KMAX 20			// Макс. значение калиброванного датчика
#DEFINE CALSPEED 100		// Скорость двигателей при калибровке

int sensMin[7] = {0, 1023, 1023, 1023, 1023, 1023, 1023};
int sensMax[7] = {0};
int sens[7] = {0};
int i;
float cc[7] = {0.0};

/* Функция поворота робота на месте
dir -- направление поворота, time -- время работы двигателей */
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
	delay(time); // Остановка
	digitalWrite(IN1, LOW);
	analogWrite(IN2, 0);
	digitalWrite(IN3, LOW);
	analogWrite(IN4, 0);
}

// Функция калибровки датчиков
void calibration(int pin1, int pinN)
{
	Serial.println("Calibration Start");

	/* Сумма показаний датчиков для определения
	положения их всех на белом поле */
	int temp, tempS;
	boolean black = false; // Датчик, попавший на линию
	boolean cEnd = false; // Окончание калибровки
	int dir = CCW; // Направление поворота робота
	/* Число пересечений линии при повороте
	по часовой стрелке */
	int nl = 0;

	while (!cEnd)
	{
		tempS = 0;
		for (i = 0; i < pinN; i++)
		{
			temp = analogRead(pin1 + i) / SMOOTH; // Сглаживание точности показаний датчика
			if (temp < sensMin[i + 1]) sensMin[i + 1] = temp;
			if (temp > sensMax[i + 1]) sensMax[i + 1] = temp;
			tempS += temp / 3; // Сумма черезмерно сглаженных показаний датчиков
		}
		// Если расположение -- на белом поле
		if (tempS == 0)
		{
			/* Если перед этим были: 1 чёрная линия и вращение против часовой стрелки,
			то: начинается вращение по часовой. */
			if ((black) && (dir == CCW) && (nl == 0))
			{
				black = false;
				dir = CW;
				nl++;
			}
			
			/* Если перед этим были: 1 чёрная линия и вращение по часовой стрелке,
			то: продолжается вращение. */
			if ((black) && (dir == CW) && (nl == 1))
			{
				black = false;
				nl++;
			}
			
			/* Если перед этим были: 2 чёрных линии и вращение по часовой стрелке,
			то: возвращение в исходное положение. */
			if ((black) && (dir == CW) && (nl == 2))
			{
				black = false;
				dir = CCW;
				nl++;
			}
			
			/* Если перед этим были: все чёрные линии и вращение против часовой стрелки,
			то: завершение калибровки. */
			if ((black) && (dir == CCW) && (nl == 3))
				cEnd = true;
		}
		else
		{
			// Если расположение -- на чёрном поле (т. е. tempS != 0)
			black = true;
		}
		rotation(dir, 40); // Поворот втечение 100 мс
	}
	// Вычисление коэффициентов калибровки
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

// Функция считывания показаний после калибровки
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
	
	delay(1000);		// Ожидание после включения
	calibration(A0, 6);
	delay(1000);		// Ожидание до нажатия (отсутствующей) пусковой кнопки
}

void loop()
{
	sensorsRead(A0, 6);
	delay(417);		// В ритме 144 bpm
}
