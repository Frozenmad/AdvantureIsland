#include "Constructer.h"
#include "land.h"
#include "player.h"

using namespace std;

Constructer::Constructer()
{
	vector<string> Construct;
	Construct.push_back("制作台");
	Construct.push_back("Make a bed       : need 10 woods            ");
	Construct.push_back("Make a chair     : need 4 woods             ");
	Construct.push_back("Make a wall      : need 3 stones            ");
	Construct.push_back("Make a cook      : need 4 stones and 2 woods");
	Construct.push_back("Make a firetower : need 2 stones            ");
	Construct.push_back("Quit                                        ");
	Construct.push_back("通过↑↓控制选择");
	InteractiveMap = new ConsoleMap(GetStdHandle(STD_OUTPUT_HANDLE), Construct, -1, 1, 1, 7);
	ShowChar = 'M';
}

void Constructer::ProcessQuery(Player * player, COORD showPos)
{
	ClearInputBuffer();
	bool bQuit = false;
	while (!bQuit)
	{
		char ins = ' ';
		while (ins != 13)
		{
			InteractiveMap->PrintOnViewPort(showPos.X, showPos.Y);
			ins = _getch();
			switch (ins)
			{
			case 72:
				InteractiveMap->UpdateSelect(-1);
				break;
			case 80:
				InteractiveMap->UpdateSelect(1);
				break;
			default:
				break;
			}
		}
		int selectId = InteractiveMap->GetSelectedItem();
		switch (selectId)
		{
		case 0:
			if (player->PlayerInventory.Wood >= 10)
			{
				player->PlayerInventory.Wood -= 10;
				player->PlayerInventory.Bed += 1;
			}
			break;
		case 1:
			if (player->PlayerInventory.Wood >= 4)
			{
				player->PlayerInventory.Wood -= 4;
				player->PlayerInventory.Chair += 1;
			}
			break;
		case 2:
			if (player->PlayerInventory.Stone >= 3)
			{
				player->PlayerInventory.Stone -= 3;
				player->PlayerInventory.Wall += 1;
			}
			break;
		case 3:
			if (player->PlayerInventory.Stone >= 4 && player->PlayerInventory.Wood >= 2)
			{
				player->PlayerInventory.Stone -= 4;
				player->PlayerInventory.Wood -= 2;
				player->PlayerInventory.Cook += 1;
			}
			break;
		case 4:
			if (player->PlayerInventory.Stone >= 2)
			{
				player->PlayerInventory.Stone -= 2;
				player->PlayerInventory.FireTower += 1;
			}
			break;
		case 5:
			bQuit = true;
			break;
		default:
			break;
		}
	}
	_PrintInfo(showPos.X, showPos.Y + 0, "                                            ");
	_PrintInfo(showPos.X, showPos.Y + 1, "                                            ");
	_PrintInfo(showPos.X, showPos.Y + 2, "                                            ");
	_PrintInfo(showPos.X, showPos.Y + 3, "                                            ");
	_PrintInfo(showPos.X, showPos.Y + 4, "                                            ");
	_PrintInfo(showPos.X, showPos.Y + 5, "                                            ");
	_PrintInfo(showPos.X, showPos.Y + 6, "                                            ");
	_PrintInfo(showPos.X, showPos.Y + 7, "                                            ");
}


Constructer::~Constructer()
{
	delete InteractiveMap;
}
