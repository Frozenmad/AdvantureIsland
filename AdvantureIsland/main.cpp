#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <string>
#include <thread>
#include <fstream>
#include <mutex>
#include "GlobalParameter.h"
#include "land.h"
#include "player.h"
#include "Seed.h"
#include "Stone.h"
#include "Tree.h"
#include "Constructer.h"
#include "Cooker.h"
#include "Gardener.h"
#include "Placer.h"

using namespace std;

// 一些全局变量
ifstream inFile;
Player * MyPlayer;
int height, width;
bool bGameEnd;
int iTree, iStone, iSeed, iAnimal;

mutex GameThreadMutex;
Placer MyPlacer;
HANDLE hOut;

/*
GamePlayState ---- the state of game play stay in.
0 --- Origin
1 --- Open Menu
2 --- Main Play State
3 --- End State
*/
int GamePlayState;

bool validMapId(int x, int y)
{
	return x >= 0 || y >= 0 || x < height || y < width;
}

void EndPlay();

float GlobalTime;

void _inner_Tick(float DeltaSecond, u_int fps = 20);

void _thread_Tick(float DeltaSecond, u_int fps);

void BeginPlay();

void EndPlay();

void PrintInformation(int x, int y, string info);

void Pause()
{
	GameThreadMutex.lock();
}

void Continue()
{
	GameThreadMutex.unlock();
}

string StringfyTime(long long InTime)
{
	int Day = (int)(InTime / (24 * 3600));
	int Hour = (int)((InTime / 3600) % 24);
	int Min = (int)((InTime / 60) % 60);
	int Second = (int)(InTime % 60);

	char s[20];

	sprintf_s(s, "%02d-%02d-%02d-%02d", Day, Hour, Min, Second);

	string result(s);

	result += "\n";

	return result;
}

void SickPlayer(Player * plr)
{
	double rate = rand() / (RAND_MAX + 1.0);
	switch (GlobalParameter::GlobalWeather)
	{
	case EWeather::Sunny:
		if (rate < 0.1) { plr->PlayerSick(); }
		break;
	case EWeather::Windy:
		if (rate < 0.3) { plr->PlayerSick(); }
		break;
	case EWeather::Rainy:
		if (rate < 0.5) { plr->PlayerSick(); }
		break;
	default:
		break;
	}
}

void PrintMap(int initX, int initY, int height, int width, int SideHeight = 5, int SideWidth = 5)
{
	for (int i = -SideHeight; i <= SideHeight; i++)
	{
		for (int j = -SideWidth; j <= SideWidth; j++)
		{
			int x = MyPlayer->GetPlayerPosition().X + i, y = MyPlayer->GetPlayerPosition().Y + j;
			if (x < 0 || y < 0 || x >= height || y >= width)
			{
				SetConsoleTextAttribute(hOut, 0);
				cout << "  ";
			}
			else if (i == 0 && j == 0)
			{
				SetConsoleTextAttribute(hOut, GlobalParameter::Map[x][y].getColor());
				cout << "<>";
			}
			else
			{
				if (GlobalParameter::Map[x][y].HavePickup() || GlobalParameter::Map[x][y].HaveInteractive() || GlobalParameter::Map[x][y].HavePlaceBase())
				{
					SetConsoleTextAttribute(hOut, GlobalParameter::Map[x][y].getColor());
					cout << GlobalParameter::Map[x][y].getChar() << " ";
				}
				else
				{
					SetConsoleTextAttribute(hOut, GlobalParameter::Map[x][y].getColor());
					cout << "  ";
				}
			}
		}
		cout << endl;
	}
	SetConsoleTextAttribute(hOut, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
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
				Show += GlobalParameter::Map[x][y].getChar();
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
	
	Land * targetLand = &map[TargetPosition.X][TargetPosition.Y];

	if (targetLand->CanStepOn())
	{
		targetLand->onStepOnLand(MyPlayer);
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
			SPlayerInventory * Inv = &(MyPlayer->PlayerInventory);
			switch (h)
			{
				case 'w':
					HandleMapInput(GlobalParameter::Map, MyPlayer, Position(-1, 0));
					break;
				case 's':
					HandleMapInput(GlobalParameter::Map, MyPlayer, Position(1, 0));
					break;
				case 'a':
					HandleMapInput(GlobalParameter::Map, MyPlayer, Position(0, -1));
					break;
				case 'd':
					HandleMapInput(GlobalParameter::Map, MyPlayer, Position(0, 1));
					break;
				case 'q':
					bGameEnd = true;
					break;
				case 'm':
				{
					int x, y;
					x = MyPlayer->GetPlayerPosition().X; y = MyPlayer->GetPlayerPosition().Y;
					// check up:
					if (validMapId(x - 1, y) || GlobalParameter::Map[x-1][y].HaveInteractive())
					{
						COORD Pos;
						Pos.X = 0; Pos.Y = 13;
						Pause();
						GlobalParameter::Map[x - 1][y].getInteractiveBase()->ProcessQuery(MyPlayer,Pos);
						Continue();
					}
					break;
				}
				case 'p':
				{
					COORD Pos;
					Pos.X = 0; Pos.Y = 13;
					Pause();
					MyPlacer.Place(GlobalParameter::Map, MyPlayer, Pos);
					Continue();
					break;
				}
				default:
					break;
			}
		}
		if (fps != 0) { Sleep((DWORD)(1000.0 / fps)); }
	}
}

void PrintInformation(int x, int y, string info)
{
	COORD Pos;
	Pos.X = x; Pos.Y = y;
	GlobalParameter::OutputLock();
	SetConsoleCursorPosition(hOut, Pos);
	cout << info;
	GlobalParameter::OutputUnLock();
}

void ShowInstruction(int initX, int initY)
{
	PrintInformation(initX, initY + 0, "+--------------------------------------+");
	PrintInformation(initX, initY + 1, "| 1. 3 veg + 1 wood => 1 food          |");
	PrintInformation(initX, initY + 2, "| 2. 1 meat + 1 veg + 1 wood => 1 food |");
	PrintInformation(initX, initY + 3, "| 3. 2 meat + 1 wood => 1 food         |");
	PrintInformation(initX, initY + 4, "| 4. 10 wood => 1 bed                  |");
	PrintInformation(initX, initY + 5, "| 5. 4 wood => 1 chair                 |");
	PrintInformation(initX, initY + 6, "| 6. 3 stone => 1 wall                 |");
	PrintInformation(initX, initY + 7, "| 7. 2 stone => 1 FireTower            |");
	PrintInformation(initX, initY + 8, "| 8. 4 stone + 2 wood => 1 cook        |");
	PrintInformation(initX, initY + 9, "| 9. 1 veg => 1 medicine               |");
	PrintInformation(initX, initY + 10, "| 0. 1 veg => 2 seed                   |");
	PrintInformation(initX, initY + 11, "+--------------------------------------+");
}

void ShowPlayerInformation(int initX, int initY)
{
	PrintInformation(initX, initY + 0, "Player Info");
	char infos[40];
	sprintf_s(infos, "Wood     :%2d Stone     :%2d", MyPlayer->PlayerInventory.Wood,MyPlayer->PlayerInventory.Stone);
	PrintInformation(initX, initY + 1, string(infos));
	sprintf_s(infos, "Seed     :%2d Meat      :%2d", MyPlayer->PlayerInventory.Seed, MyPlayer->PlayerInventory.Meat);
	PrintInformation(initX, initY + 2, string(infos));
	sprintf_s(infos, "Vegetable:%2d Trap      :%2d", MyPlayer->PlayerInventory.Vegetable, MyPlayer->PlayerInventory.Trap);
	PrintInformation(initX, initY + 3, string(infos));
	sprintf_s(infos, "Medicine :%2d Wall      :%2d", MyPlayer->PlayerInventory.Medicine, MyPlayer->PlayerInventory.Wall);
	PrintInformation(initX, initY + 4, string(infos));
	sprintf_s(infos, "Bed      :%2d Chair     :%2d", MyPlayer->PlayerInventory.Bed, MyPlayer->PlayerInventory.Chair);
	PrintInformation(initX, initY + 5, string(infos));
	sprintf_s(infos, "FireTower:%2d Cook      :%2d", MyPlayer->PlayerInventory.FireTower, MyPlayer->PlayerInventory.Cook);
	PrintInformation(initX, initY + 6, string(infos));
	string info = "Player State : ";
	switch (MyPlayer->GetPlayerState())
	{
	case EPlayerState::Live:
		info += "Live";
		break;
	case EPlayerState::Die:
		info += "Die ";
		break;
	default:
		break;
	}
	PrintInformation(initX, initY + 7, info);
	info = "Player Healthy State : ";
	switch (MyPlayer->GetPlayerHealthState())
	{
	case EHealthState::Healthy:
		info += "Healthy";
		break;
	case EHealthState::Auto:
		info += "Auto   ";
		break;
	case EHealthState::Sick:
		info += "Sick   ";
		break;
	default:
		break;
	}
	PrintInformation(initX, initY + 8, info);
	info = "Player Hungry State : ";
	switch (MyPlayer->GetPlayerHungryState())
	{
	case EHungryState::Full:
		info += "Full";
		break;
	case EHungryState::HungryAuto:
		info += "Auto";
		break;
	case EHungryState::Hungry:
		info += "Hungry";
		break;
	case EHungryState::Starve:
		info += "Starve";
		break;
	default:
		break;
	}
	PrintInformation(initX, initY + 9, info);
}

// map刷新函数
void _thread_refresh_map(u_int fps = 20)
{
	COORD Pos;
	Pos.X = Pos.Y = 0;

	static int time = 0;

	while (!bGameEnd)
	{
		
		// First output basic info
		if (time == 0)
		{
			Pos.X = Pos.Y = 0;
			GlobalParameter::OutputLock();
			SetConsoleCursorPosition(hOut, Pos);
			cout << StringfyTime((long long)(GlobalTime));
			GlobalParameter::OutputUnLock();
		}
		Pos.X = 0; Pos.Y = 1;
		GlobalParameter::OutputLock();
		SetConsoleCursorPosition(hOut, Pos);
		PrintMap(0, 1, height, width);
		GlobalParameter::OutputUnLock();
		time = (time + 1) % (fps / 2);
		// Then output player info
		ShowPlayerInformation(25, 1);
		// Print weather information
		string winfo = "Weather : ";
		switch (GlobalParameter::GlobalWeather)
		{
		case EWeather::Sunny:
			winfo += "Sunny";
			break;
		case EWeather::Windy:
			winfo += "Windy";
			break;
		case EWeather::Rainy:
			winfo += "Rainy";
			break;
		default:
			break;
		}
		PrintInformation(25, 0, winfo);
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
		GlobalParameter::Map.push_back(Tmp);
	}
	cout << endl << "Begin random generate item on map ... " << flush;
	// Tree
	int counter = 0;
	while (counter < iTree)
	{
		int x = (int)(height * rand() / (RAND_MAX + 1.0));
		int y = (int)(width * rand() / (RAND_MAX + 1.0));
		if (GlobalParameter::Map[x][y].CanSpawnThing() && !GlobalParameter::Map[x][y].HavePickup())
		{
			Tree* tmp;
			tmp = new Tree();
			GlobalParameter::PickupObjectSet.AddElement(tmp);
			GlobalParameter::Map[x][y].AddPickup(tmp);
			counter += 1;
		}
	}
	counter = 0;
	while (counter < iStone)
	{
		int x = (int)(height * rand() / (RAND_MAX + 1.0));
		int y = (int)(width * rand() / (RAND_MAX + 1.0));
		if (GlobalParameter::Map[x][y].CanSpawnThing() && !GlobalParameter::Map[x][y].HavePickup())
		{
			Stone* tmp;
			tmp = new Stone();
			GlobalParameter::PickupObjectSet.AddElement(tmp);
			GlobalParameter::Map[x][y].AddPickup(tmp);
			counter += 1;
		}
	}
	counter = 0;
	while (counter < iSeed)
	{
		int x = (int)(height * rand() / (RAND_MAX + 1.0));
		int y = (int)(width * rand() / (RAND_MAX + 1.0));
		if (GlobalParameter::Map[x][y].CanSpawnThing() && !GlobalParameter::Map[x][y].HavePickup())
		{
			Seed* tmp;
			tmp = new Seed(2*3600*24);
			GlobalParameter::PickupObjectSet.AddElement(tmp);
			GlobalParameter::Map[x][y].AddPickup(tmp);
			counter += 1;
		}
	}
	cout << "done!\n";
}



// 游戏运行之前进行的代码
void InitPlay()
{
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GamePlayState = 1;
	vector<string> Menu;
	system("cls");
	system("mode con cols=80 lines=25");
	PrintInformation(36,0,"冒险之地");
	Menu.push_back("0. Begin Play          ");
	Menu.push_back("1. Look for Instruction");
	Menu.push_back("2. Quit                ");
	PrintInformation(3, 7, "通过↑↓控制选择");
	ConsoleMap GameMenu(hOut,Menu,-1);
	bool LoopFlag = true;
	int selectId;
	ClearInputBuffer();
	while (LoopFlag)
	{
		GameMenu.PrintOnViewPort(4, 3);
		char p = _getch();
		switch (p)
		{
		// up
		case 72:
			GameMenu.UpdateSelect(-1);
			break;
		// down
		case 80:
			GameMenu.UpdateSelect(1);
			break;
		case 13:
			selectId = GameMenu.GetSelectedItem();
			LoopFlag = false;
			break;
		default:
			break;
		}
	}
	switch (selectId)
	{
	case 0:
		BeginPlay();
		EndPlay();
		break;
	case 1:
		GameInstruction(InitPlay);
		break;
	case 2:
		system("cls");
		cout << "谢谢使用！";
		Sleep(1000);
		system("cls");
		break;
	default:
		break;
	}
}

// 游戏初始化的一些代码，在游戏开始前执行
void BeginPlay()
{
	srand((int)time(0));
	inFile.open("D:/0_beginning/C++/game/AdvantureIsland/Debug/water.in");
	GlobalTime = 0;
	//设置光标为不可见
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(hOut, &CursorInfo);
	CursorInfo.bVisible = false;
	SetConsoleCursorInfo(hOut, &CursorInfo);

	ClearInputBuffer();

	//首先清屏
	system("cls");

	//加载地图
	LoadMap();

	Position tmp;

	inFile >> tmp.X >> tmp.Y;

	Constructer* MyConstructer = new Constructer;

	Cooker* MyCooker = new Cooker;

	Gardener* MyGardener = new Gardener;

	GlobalParameter::InteractiveObjectSet.AddElement(MyConstructer);
	GlobalParameter::InteractiveObjectSet.AddElement(MyCooker);
	GlobalParameter::InteractiveObjectSet.AddElement(MyGardener);

	GlobalParameter::Map[0][49].AddInteractive(MyConstructer);

	GlobalParameter::Map[0][48].AddInteractive(MyCooker);

	GlobalParameter::Map[0][47].AddInteractive(MyGardener);

	//初始化Player
	MyPlayer = new Player(100.0, EHealthState::Healthy, EHungryState::HungryAuto, tmp, height, width);

	bGameEnd = false;

	cout << "Press any key to continue!";

	_getch();

	system("cls");

	//ShowInstruction(38, 1);
	// Show the map
	thread MapThread(_thread_refresh_map, 30);
	thread InputThread(_thread_handle_input, 0);
	thread TickThread(_thread_Tick, 30, 10);

	while (!bGameEnd)
	{
		bGameEnd = bGameEnd || MyPlayer->GetPlayerState() == EPlayerState::Die;
		Sleep(50);
	}

	MapThread.join();
	InputThread.join();
	TickThread.join();
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

	delete MyPlayer;
	inFile.close();
	system("cls");
}

void _thread_Tick(float DeltaSecond, u_int fps = 20)
{
	while (!bGameEnd)
	{
		Pause();
		// 设置执行周期
		thread t(_inner_Tick, DeltaSecond, fps);
		t.detach();
		Continue();
		if(fps != 0)
		{
			Sleep(1000.0 / fps);
		}
	}
}

void _inner_Tick(float DeltaSecond, u_int fps)
{
	static int Day = 0;
	GlobalTime = GlobalTime + DeltaSecond;
	if (GlobalTime > 15 * 3600 * 24) GlobalTime = 0;

	if (GlobalTime > Day * 3600 * 24)
	{
		Day++;
		GlobalParameter::ChangeWeather();
		SickPlayer(MyPlayer);
	}

	MyPlayer->Tick(DeltaSecond);

	GlobalParameter::PickupObjectSet.lockSet();
	for (PickupBase * pointer : GlobalParameter::PickupObjectSet.inner)
	{
		if (pointer->WhetherTick())
		{
			pointer->Tick(DeltaSecond);
		}
	}
	GlobalParameter::PickupObjectSet.unlockSet();

	GlobalParameter::PlaceObjectVec.lockSet();
	for (PlaceBase * placeObj : GlobalParameter::PlaceObjectVec.inner)
	{
		if (placeObj->GetCanTick())
		{
			placeObj->Tick(DeltaSecond);
		}
	}
	GlobalParameter::PlaceObjectVec.unlockSet();
}

int test()
{
	vector<vector<Land>> testMap;
	vector<Land> tmp;
	tmp.push_back(Land(ELandType::Farm));
	tmp.push_back(Land(ELandType::Farm));
	testMap.push_back(tmp);
	testMap.push_back(tmp);
	Tree testTree1,testTree2;
	Player testPlayer(100, EHealthState::Healthy, EHungryState::Full, Position(0,0), 2, 2, EPlayerState::Live);
	testMap[0][0].AddPickup(&testTree1);
	testMap[1][1].AddPickup(&testTree2);
	cout << "Now the 0 0 : " << testMap[0][0].HavePickup() << endl;
	cout << "Now the 1 1 : " << testMap[1][1].HavePickup() << endl;
	testMap[0][0].onStepOnLand(&testPlayer);
	testMap[1][1].onStepOnLand(&testPlayer);
	cout << "After Step on" << endl;
	cout << "Now the 0 0 : " << testMap[0][0].HavePickup() << endl;
	cout << "Now the 1 1 : " << testMap[1][1].HavePickup() << endl;
	_getch();
	return 0;
}

int main()
{
	GamePlayState = 0;
	InitPlay();
	return 0;
}
