#include <iostream>
#include <Windows.h>
#include "GlobalParameter.h"
#include "land.h"
#include "player.h"
#include <conio.h>
#include <string>

vector<vector<Land>> Map;

vector<int> InitPos;

int height, width;

string getShowWindow(int SideHeight, int SideWidth, vector<vector<Land>> &map, vector<int> &WindowPos, int height, int width)
{
	string Show = "";

	for (int i = -SideHeight; i <= SideHeight; i++)
	{
		for (int j = -SideWidth; j <= SideWidth; j++)
		{
			int x = WindowPos[0] + i, y = WindowPos[1] + j;
			if (x < 0 || y < 0 || x >= height || y >= width)
			{
				Show += "~";
			}
			else if (i == 0 && j == 0)
			{
				Show += '@';
			}
			else
			{
				Show += map[x][y].getChar();
			}
			Show += ' ';
		}
		Show += '\n';
	}

	return Show;
}

void LoadMap()
{
	cout << "Begin loading map ... " << flush;
	cin >> height >> width;
	for (int i = 0; i < height; i++)
	{
		vector<Land> Tmp;
		for (int j = 0; j < width; j++)
		{
			int CurrentLand;
			cin >> CurrentLand;
			switch (CurrentLand)
			{
			case 0:
				Tmp.push_back(Idle);
				break;
			case 1:
				Tmp.push_back(Farm);
				break;
			case 2:
				Tmp.push_back(Destroyed);
				break;
			case 3:
				Tmp.push_back(Cliff);
				break;
			case 4:
				Tmp.push_back(Water);
				break;
			default:
				Tmp.push_back(Water);
				break;
			}
		}
		Map.push_back(Tmp);
	}

	int pos;
	cin >> pos;
	InitPos.push_back(pos);
	cin >> pos;
	InitPos.push_back(pos);

	cout << "done!" << endl;

	cout << "Press anykey to continue";

	_getch();
}

void BeginPlay()
{
	//设置光标为不可见
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(hOut, &CursorInfo);
	CursorInfo.bVisible = false;
	SetConsoleCursorInfo(hOut, &CursorInfo);

	//首先清屏
	system("cls");

	//加载地图
	LoadMap();
}

void EndPlay()
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	//设置光标为可见
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(hOut, &CursorInfo);
	CursorInfo.bVisible = true;
	SetConsoleCursorInfo(hOut, &CursorInfo);
}

int main()
{
	BeginPlay();

	Player MyPlayer(100.0, EHealthState::Healthy, EHungryState::HungryAuto, InitPos);

	vector<int> WindowPos(InitPos.begin(), InitPos.end());

	int WindowSideHeight = 5, WindowSideWidth = 5;

	char inst;

	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD Position;
	Position.X = Position.Y = 0;

	//设置光标为不可见
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(hOut, &CursorInfo);
	CursorInfo.bVisible = false;
	SetConsoleCursorInfo(hOut, &CursorInfo);

	while (true)
	{
		SetConsoleCursorPosition(hOut, Position);
		cout << getShowWindow(WindowSideHeight, WindowSideWidth, Map, WindowPos, height, width);
		if (_kbhit())
		{
			inst = _getch();
			if (inst == 'q')
			{
				break;
			}
			else
			{
				switch (inst)
				{
				case 'w':
					WindowPos[0] -= 1;
					if (WindowPos[0] < 0) WindowPos[0] = 0;
					break;
				case 's':
					WindowPos[0] += 1;
					if (WindowPos[0] >= height) WindowPos[0] = height - 1;
					break;
				case 'a':
					WindowPos[1] -= 1;
					if (WindowPos[1] < 0) WindowPos[1] = 0;
					break;
				case 'd':
					WindowPos[1] += 1;
					if (WindowPos[1] >= width) WindowPos[1] = width - 1;
					break;
				default:
					break;
				}
			}
		}
	}



	return 0;

}
