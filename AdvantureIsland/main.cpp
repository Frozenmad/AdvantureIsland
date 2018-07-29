#include <iostream>
#include <Windows.h>
#include "GlobalParameter.h"
#include "land.h"
#include "player.h"
#include <conio.h>
#include <string>
#include <thread>

vector<vector<Land>> Map;

Player * MyPlayer;

int height, width;

bool bGameEnd;

void EndPlay();

void _inner_Tick();

void _thread_Tick(float DeltaSecond, u_int fps);

string getShowWindow(int SideHeight, int SideWidth, int height, int width)
{
	string Show = "";

	for (int i = -SideHeight; i <= SideHeight; i++)
	{
		for (int j = -SideWidth; j <= SideWidth; j++)
		{
			int x = MyPlayer->GetPlayerPosition().X + i, y = MyPlayer->GetPlayerPosition().Y + j;
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
				Show += Map[x][y].getChar();
			}
			Show += ' ';
		}
		Show += '\n';
	}

	return Show;
}

void HandleMapInput(vector<vector<Land>> &map, Player * MyPlayer, Position DiffPos)
{
	Position CurPosition = MyPlayer->GetPlayerPosition();
	Position TargetPosition = CurPosition + DiffPos;

	TargetPosition.X < 0 ? TargetPosition.X = 0 : TargetPosition.X >= height ? TargetPosition.X = height - 1 : TargetPosition.X = TargetPosition.X;
	TargetPosition.Y < 0 ? TargetPosition.Y = 0 : TargetPosition.Y >= width ? TargetPosition.Y = width - 1 : TargetPosition.Y = TargetPosition.Y;
	
	Land targetLand = map[TargetPosition.X][TargetPosition.Y];

	if (targetLand.CanStepOn())
	{
		MyPlayer->SetPlayerPosition(TargetPosition);
	}
}

// 在此处添加键盘的响应
void _thread_handle_input(u_int fps = 20)
{
	while (!bGameEnd)
	{
		if (_kbhit())
		{
			char h = _getch();
			switch (h)
			{
				case 'w':
					HandleMapInput(Map, MyPlayer, Position(-1, 0));
					break;
				case 's':
					HandleMapInput(Map, MyPlayer, Position(1, 0));
					break;
				case 'a':
					HandleMapInput(Map, MyPlayer, Position(0, -1));
					break;
				case 'd':
					HandleMapInput(Map, MyPlayer, Position(0, 1));
					break;
				case 'q':
					bGameEnd = true;
					break;
				default:
					break;
			}
		}
		if (fps != 0) { Sleep(1000.0 / fps); }
	}
}

// map刷新函数
void _thread_refresh_map(u_int fps = 20)
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD Pos;
	Pos.X = Pos.Y = 0;

	while (!bGameEnd)
	{
		SetConsoleCursorPosition(hOut, Pos);
		cout << getShowWindow(5, 5, height, width);
		if (fps != 0) {
			Sleep(1000.0 / fps);
		}
	}
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
	cout << "done!\n";
}

// 游戏初始化的一些代码，在游戏开始前执行
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

	Position tmp;

	cin >> tmp.X >> tmp.Y;

	//初始化Player
	MyPlayer = new Player(100.0, EHealthState::Healthy, EHungryState::HungryAuto, tmp, height, width);

	bGameEnd = false;

	cout << "Press any key to continue!";

	_getch();

	system("cls");

	// Show the map
	thread MapThread(_thread_refresh_map, 30);
	thread InputThread(_thread_handle_input, 0);
	thread TickThread(_thread_Tick, 0.1, 10);

	MapThread.detach();
	InputThread.detach();
	TickThread.detach();

	while (!bGameEnd)
	{
		Sleep(1000);
	}
}

// 游戏结束后的代码，在游戏退出时执行
void EndPlay()
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	// 设置光标为可见
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(hOut, &CursorInfo);
	CursorInfo.bVisible = true;
	SetConsoleCursorInfo(hOut, &CursorInfo);
	delete MyPlayer;
}

void _thread_Tick(float DeltaSecond, u_int fps = 20)
{
	while (!bGameEnd)
	{
		// 设置执行周期
		thread t(_inner_Tick);
		t.detach();
		if(fps != 0)
		{
			Sleep(1000.0 / fps);
		}
	}
}

void _inner_Tick()
{

}

int main()
{
	BeginPlay();

	EndPlay();
	return 0;

}
