#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <string>
#include <thread>
#include <fstream>
#include "GlobalParameter.h"
#include "land.h"
#include "player.h"
#include "Seed.h"
#include "Stone.h"
#include "Tree.h"

// 一些全局变量
ifstream inFile;
vector<vector<Land>> Map;
Player * MyPlayer;
int height, width;
bool bGameEnd;
int iTree, iStone, iSeed, iAnimal;
vector<PickupBase*> PickupList;
HANDLE hOut;

void EndPlay();

float GlobalTime;

void _inner_Tick(float DeltaSecond, u_int fps = 20);

void _thread_Tick(float DeltaSecond, u_int fps);

string StringfyTime(long long InTime)
{
	int Day = InTime / (24 * 3600);
	int Hour = (InTime / 3600) % 24;
	int Min = (InTime / 60) % 60;
	int Second = InTime % 60;

	char s[20];

	sprintf_s(s, "%02d-%02d-%02d-%02d", Day, Hour, Min, Second);

	string result(s);

	result += "\n";

	return result;
}

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

	// 加上玩家状态的输出
	switch (MyPlayer->GetPlayerState())
	{
	case EPlayerState::Live:
		Show += "Player State : Live\t\n";
		break;
	case EPlayerState::Die:
		Show += "Player State : Die\t\n";
		break;
	default:
		Show += "Player State : Unknown\t\n";
		break;
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
		targetLand.onStepOnLand(MyPlayer);
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

void PrintInformation(int x, int y, string info)
{
	COORD Pos;
	Pos.X = x; Pos.Y = y;
	SetConsoleCursorPosition(hOut, Pos);
	cout << info;
}

void ShowPlayerInformation(int initX, int initY)
{
	PrintInformation(initX, initY + 0, "Player Info");
	PrintInformation(initX, initY + 1, "Wood     :" + to_string(MyPlayer->PlayerInventory.Wood));
	PrintInformation(initX, initY + 2, "Stone    :" + to_string(MyPlayer->PlayerInventory.Stone));
	PrintInformation(initX, initY + 3, "Seed     :" + to_string(MyPlayer->PlayerInventory.Seed));
	PrintInformation(initX, initY + 4, "Meat     :" + to_string(MyPlayer->PlayerInventory.Meat));
	PrintInformation(initX, initY + 5, "Vegetable:" + to_string(MyPlayer->PlayerInventory.Vegetable));
	PrintInformation(initX, initY + 6, "Trap     :" + to_string(MyPlayer->PlayerInventory.Trap));
	PrintInformation(initX, initY + 7, "Medicine :" + to_string(MyPlayer->PlayerInventory.Medicine));
	PrintInformation(initX, initY + 8, "Wall     :" + to_string(MyPlayer->PlayerInventory.Wall));
	PrintInformation(initX, initY + 9, "Bed      :" + to_string(MyPlayer->PlayerInventory.Bed));
	PrintInformation(initX, initY + 10, "Chair    :" + to_string(MyPlayer->PlayerInventory.Chair));
	PrintInformation(initX, initY + 11, "FireTower:" + to_string(MyPlayer->PlayerInventory.FireTower));
	PrintInformation(initX, initY + 12, "Cook     :" + to_string(MyPlayer->PlayerInventory.Cook));
}

// map刷新函数
void _thread_refresh_map(u_int fps = 20)
{
	COORD Pos;
	Pos.X = Pos.Y = 0;

	while (!bGameEnd)
	{
		// First output basic info
		Pos.X = Pos.Y = 0;
		SetConsoleCursorPosition(hOut, Pos);
		cout << StringfyTime(GlobalTime) <<getShowWindow(5, 5, height, width);
		// Then output player info
		ShowPlayerInformation(25, 1);
		if (fps != 0) {
			Sleep(1000.0 / fps);
		}
	}
}

void LoadMap()
{
	iTree = 20;
	iStone = 10;
	iAnimal = 0;
	iSeed = 5;
	cout << "Begin loading map ... " << flush;
	inFile >> height >> width;
	for (int i = 0; i < height; i++)
	{
		vector<Land> Tmp;
		for (int j = 0; j < width; j++)
		{
			int CurrentLand;
			inFile >> CurrentLand;
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
	cout << endl << "Begin random generate item on map ... " << flush;
	srand((int)time(0));
	// Tree
	int counter = 0;
	while (counter < iTree)
	{
		int x = (int)(height * rand() / (RAND_MAX + 1));
		int y = (int)(width * rand() / (RAND_MAX + 1));
		if (Map[x][y].CanSpawnThing() && !Map[x][y].HavePickup())
		{
			Tree* tmp;
			tmp = new Tree();
			PickupList.push_back(tmp);
			Map[x][y].AddPickup(tmp);
			counter += 1;
		}
	}
	counter = 0;
	while (counter < iStone)
	{
		int x = (int)(height * rand() / (RAND_MAX + 1));
		int y = (int)(width * rand() / (RAND_MAX + 1));
		if (Map[x][y].CanSpawnThing() && !Map[x][y].HavePickup())
		{
			Stone* tmp;
			tmp = new Stone();
			PickupList.push_back(tmp);
			Map[x][y].AddPickup(tmp);
			counter += 1;
		}
	}
	counter = 0;
	while (counter < iSeed)
	{
		int x = (int)(height * rand() / (RAND_MAX + 1));
		int y = (int)(width * rand() / (RAND_MAX + 1));
		if (Map[x][y].CanSpawnThing() && !Map[x][y].HavePickup())
		{
			Seed* tmp;
			tmp = new Seed(60);
			PickupList.push_back(tmp);
			Map[x][y].AddPickup(tmp);
			counter += 1;
		}
	}
	cout << "done!\n";
}

// 游戏初始化的一些代码，在游戏开始前执行
void BeginPlay()
{
	inFile.open("D:/0_beginning/C++/game/AdvantureIsland/Debug/water.in");
	GlobalTime = 0;
	//设置光标为不可见
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(hOut, &CursorInfo);
	CursorInfo.bVisible = false;
	SetConsoleCursorInfo(hOut, &CursorInfo);

	//首先清屏
	system("cls");

	//加载地图
	LoadMap();

	Position tmp;

	inFile >> tmp.X >> tmp.Y;

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
		bGameEnd = bGameEnd || MyPlayer->GetPlayerState() == EPlayerState::Die;
		Sleep(50);
	}
}

// 游戏结束后的代码，在游戏退出时执行
void EndPlay()
{
	// 设置光标为可见
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(hOut, &CursorInfo);
	CursorInfo.bVisible = true;
	SetConsoleCursorInfo(hOut, &CursorInfo);

	if (MyPlayer->GetPlayerState() == EPlayerState::Die)
	{
		cout << "You Die!" << endl;
	}

	for (auto pointer : PickupList)
	{
		delete pointer;
	}

	delete MyPlayer;
	inFile.close();
}

void _thread_Tick(float DeltaSecond, u_int fps = 20)
{
	while (!bGameEnd)
	{
		// 设置执行周期
		thread t(_inner_Tick, DeltaSecond, fps);
		t.detach();
		if(fps != 0)
		{
			Sleep(1000.0 / fps);
		}
	}
}

void _inner_Tick(float DeltaSecond, u_int fps)
{
	GlobalTime = GlobalTime + DeltaSecond;
	if (GlobalTime > 15 * 3600 * 24) GlobalTime = 0;
	for (PickupBase * pointer : PickupList)
	{
		if (pointer->WhetherTick())
		{
			pointer->Tick(DeltaSecond);
		}
	}
}

int main()
{
	BeginPlay();

	EndPlay();
	return 0;

}
