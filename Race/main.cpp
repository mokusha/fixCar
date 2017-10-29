#include <iostream>
#include <conio.h>
#include<time.h>

using namespace std;

bool gameOver;
//символы заполнения поля
const int SPACE = 32, CAR = 219, BLOCK = 178;
const int BORDER_H = 205, BORDER_V = 186, BORDER_1 = 201, BORDER_2 = 187, BORDER_3 = 200, BORDER_4 = 188;
//значения клавиш считываемых с клавиатуры
const int KEY_LEFT = 75, KEY_RIGHT = 77, KEY_UP = 72, KEY_DOWN = 80, KEY_ENTER = 13, KEY_ESC = 27;

int arrDisplay[20][20];
int arrMyCar[4][3] = { { SPACE,CAR,SPACE },{ CAR,CAR,CAR },{ SPACE,CAR,SPACE },{ CAR,CAR,CAR } };
int i0MyCar = 15, j0MyCar = 16;
int i0Block = 1, j0Block, i1Block, j1Block;
int dir;
int countCycle = 0;
unsigned long int tBegin = 0, tEnd = 0, tDuration;
unsigned long int timeEx, tBeginPause = 0, tEndPause = 0, tDurationPause, tDurationPause2;
int speed = 300;//миллисекунды
int distanceCount = 0;

void InscribeMyCar(int arrDisplay[20][20], int i0MyCar, int j0MyCar, int arrMyCar[4][3])
{
	for (int i = i0MyCar; i < i0MyCar + 4; i++)
		for (int j = j0MyCar; j < j0MyCar + 3; j++)
			arrDisplay[i][j] = arrMyCar[i - i0MyCar][j - j0MyCar];
}

void BeginBlock(int arrDisplay[20][20], int i0Block, int j0Block)
{
	j0Block = rand() % 17 + 1;
	arrDisplay[i0Block][j0Block] = BLOCK;
	arrDisplay[i0Block][j0Block + 1] = BLOCK;
}

void Setup()
{
	gameOver = false;
	int i;
	int j;
	for (i = 0; i < 20; i++)
		for (j = 0; j < 20; j++)
		{
			if (i == 0 || i == 19)
				arrDisplay[i][j] = BORDER_H;
			else
			{
				if (j == 0 || j == 19)
					arrDisplay[i][j] = BORDER_V;
				else
					arrDisplay[i][j] = SPACE;
			}
		}
	arrDisplay[0][0] = BORDER_1;
	arrDisplay[0][19] = BORDER_2;
	arrDisplay[19][0] = BORDER_3;
	arrDisplay[19][19] = BORDER_4;
	InscribeMyCar(arrDisplay, i0MyCar, j0MyCar, arrMyCar);
	BeginBlock(arrDisplay, i0Block, j0Block);
}

bool GameOver()
{
	system("cls");
	cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << "\t\t\tGame Over" << endl << endl << endl << endl << endl << endl;
	system("pause");
	bool res = true;
	return res;
}

void Draw()
{
	system("cls");
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
			cout << char(arrDisplay[i][j]);
		cout << endl;
	}
	cout << "Time: " << timeEx / CLOCKS_PER_SEC << " s" << endl;
	cout << "Speed: " << 1 / (float(speed) / 1000) << endl;
	cout << "Distance: " << distanceCount << endl;//количество пройденный полей
}

void MoveBlock(int arrDisplay[20][20])//перемешение препятсвий
{
	for (int i = 18; i >= 0; i--)
	{
		for (int j = 0; j < 18; j++)
		{
			if (arrDisplay[i][j] == BLOCK && i == 18)
				arrDisplay[i][j] = SPACE;
			else
				if (arrDisplay[i][j] == BLOCK)
				{
					arrDisplay[i + 1][j] = BLOCK;
					arrDisplay[i][j] = SPACE;
					arrDisplay[i + 1][j + 1] = BLOCK;
					arrDisplay[i][j + 1] = SPACE;
				}
		}
		if (arrDisplay[i][18] == BLOCK)
			arrDisplay[i][18] = SPACE;
	}
}

void InputLogic()
{
	if (tDuration >= speed)
	{
		MoveBlock(arrDisplay);
		tBegin = tEnd;
		countCycle++;
		distanceCount++;
	}
	if (countCycle > 5)
	{
		BeginBlock(arrDisplay, i0Block, j0Block);
		countCycle = 0;
	}
	//управление с клавиатуры
	if (_kbhit())
	{
		dir = _getch();
		switch (dir)
		{
		case KEY_LEFT://влево
			if (j0MyCar > 1)
				j0MyCar--;
			InscribeMyCar(arrDisplay, i0MyCar, j0MyCar, arrMyCar);
			arrDisplay[i0MyCar + 1][j0MyCar + 3] = SPACE;
			arrDisplay[i0MyCar + 3][j0MyCar + 3] = SPACE;
			break;
		case KEY_RIGHT://вправо
			if (j0MyCar < 16)
				j0MyCar++;
			InscribeMyCar(arrDisplay, i0MyCar, j0MyCar, arrMyCar);
			arrDisplay[i0MyCar + 1][j0MyCar - 1] = SPACE;
			arrDisplay[i0MyCar + 3][j0MyCar - 1] = SPACE;
			break;
		case KEY_UP://вверх
			if (speed >= 10)
				speed = speed - 10;
			break;
		case KEY_DOWN://вниз
			if (speed <= 500)
				speed = speed + 10;
			break;
		case KEY_ENTER://pause
			tBeginPause = clock();
			system("pause");
			tEndPause = clock();
			tDurationPause = tEndPause - tBeginPause;
			tDurationPause2 = tDurationPause2 + tDurationPause;
			break;
		case KEY_ESC:
			gameOver = GameOver();
			break;
		}
	}
	//определение координат препятсвия
	for (int i = 1; i<19; i++)
		for (int j = 1; j < 19; j++)
		{
			if (arrDisplay[i][j] == BLOCK)
			{
				i1Block = i;
				j1Block = j;
			}
		}
	//столкновение
	for (int i = i0MyCar; i < i0MyCar + 4; i++)
	{
		for (int j = j0MyCar; j < j0MyCar + 3; j++)
		{
			if (arrDisplay[i][j] != CAR)
				if ((i1Block == i && j1Block == j) || (i1Block == i && j1Block - 1 == j))
					gameOver = GameOver();
		}
	}
	tEnd = clock();
	tDuration = tEnd - tBegin;
	timeEx = clock() - tDurationPause2;
}

int main()
{
	Setup();
	tBegin = clock();
	while (!gameOver)
	{
		if (_kbhit() || tDuration >= speed)
			Draw();
		InputLogic();
	}
	return 0;
}