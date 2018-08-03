#include "Cooker.h"
#include "player.h"

Cooker::Cooker()
{
	vector<string> CookerMess;
	CookerMess.push_back("炉灶");
	CookerMess.push_back("Make a meal : 3 veg + 1 wood         ");
	CookerMess.push_back("Make a meal : 1 meat + 1 veg + 1 wood");
	CookerMess.push_back("Make a meal : 2 meat + 1 wood        ");
	CookerMess.push_back("Quit                                 ");
	CookerMess.push_back("通过↑↓控制选择");
	InteractiveMap = new ConsoleMap(GetStdHandle(STD_OUTPUT_HANDLE),CookerMess, -1, 1, 1, 5);
	ShowChar = 'C';
}

void Cooker::ProcessQuery(Player * player, COORD showPos)
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
			if (player->PlayerInventory.Wood >= 1 && player->PlayerInventory.Vegetable >= 3)
			{
				player->PlayerInventory.Wood -= 1;
				player->PlayerInventory.Vegetable -= 3;
				player->PlayerInventory.Food += 1;
			}
			break;
		case 1:
			if (player->PlayerInventory.Wood >= 1 && player->PlayerInventory.Meat >= 1 && player->PlayerInventory.Vegetable >= 1)
			{
				player->PlayerInventory.Wood -= 1;
				player->PlayerInventory.Meat -= 1;
				player->PlayerInventory.Vegetable -= 1;
				player->PlayerInventory.Food += 1;
			}
			break;
		case 2:
			if (player->PlayerInventory.Wood >= 1 && player->PlayerInventory.Meat >= 2)
			{
				player->PlayerInventory.Wood -= 1;
				player->PlayerInventory.Meat -= 2;
				player->PlayerInventory.Food += 1;
			}
			break;
		case 3:
			bQuit = true;
			break;
		default:
			break;
		}
	}
	_PrintInfo(showPos.X, showPos.Y + 0, "                                     ");
	_PrintInfo(showPos.X, showPos.Y + 1, "                                     ");
	_PrintInfo(showPos.X, showPos.Y + 2, "                                     ");
	_PrintInfo(showPos.X, showPos.Y + 3, "                                     ");
	_PrintInfo(showPos.X, showPos.Y + 4, "                                     ");
	_PrintInfo(showPos.X, showPos.Y + 5, "                                     ");
}


Cooker::~Cooker()
{
	delete InteractiveMap;
}
