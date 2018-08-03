#include "Gardener.h"
#include "player.h"


Gardener::Gardener()
{
	vector<string> CookerMess;
	CookerMess.push_back("草药台");
	CookerMess.push_back("Make a pill : 1 veg");
	CookerMess.push_back("Make 2 seeds: 1 veg");
	CookerMess.push_back("Quit               ");
	CookerMess.push_back("通过↑↓控制选择");
	InteractiveMap = new ConsoleMap(GetStdHandle(STD_OUTPUT_HANDLE), CookerMess, -1, 1, 1, 4);
	ShowChar = 'G';
}

void Gardener::ProcessQuery(Player * player, COORD showPos)
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
			if (player->PlayerInventory.Vegetable >= 1)
			{
				player->PlayerInventory.Vegetable -= 1;
				player->PlayerInventory.Medicine += 1;
			}
			break;
		case 1:
			if (player->PlayerInventory.Vegetable >= 1)
			{
				player->PlayerInventory.Vegetable -= 1;
				player->PlayerInventory.Seed += 2;
			}
			break;
		case 2:
			bQuit = true;
			break;
		default:
			break;
		}
	}
	_PrintInfo(showPos.X, showPos.Y + 0, "                   ");
	_PrintInfo(showPos.X, showPos.Y + 1, "                   ");
	_PrintInfo(showPos.X, showPos.Y + 2, "                   ");
	_PrintInfo(showPos.X, showPos.Y + 3, "                   ");
	_PrintInfo(showPos.X, showPos.Y + 4, "                   ");
}


Gardener::~Gardener()
{
	delete InteractiveMap;
}
