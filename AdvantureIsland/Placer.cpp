#include "Placer.h"
#include "player.h"


Placer::Placer()
{
	vector<string> Place;
	Place.push_back("选择下列一个物品放置/消耗:");
	Place.push_back("Seed     ");
	Place.push_back("Trap     ");
	Place.push_back("Wall     ");
	Place.push_back("Bed      ");
	Place.push_back("Chair    ");
	Place.push_back("FireTower");
	Place.push_back("Cook     ");
	Place.push_back("Quit     ");
	Place.push_back("通过↑↓控制选择");
	InteractiveCons = new ConsoleMap(GetStdHandle(STD_OUTPUT_HANDLE), Place, 5, 1, 1, 9);
}

void Placer::Place(vector<vector<Land>>& Map, Player * player, COORD showPos)
{
	ClearInputBuffer();
	bool bQuit = false;
	while (!bQuit)
	{
		char ins = ' ';
		while (ins != 13)
		{
			InteractiveCons->PrintOnViewPort(showPos.X, showPos.Y);
			ins = _getch();
			switch (ins)
			{
			case 72:
				InteractiveCons->UpdateSelect(-1);
				break;
			case 80:
				InteractiveCons->UpdateSelect(1);
				break;
			default:
				break;
			}
		}
		int selectId = InteractiveCons->GetSelectedItem();
		switch (selectId)
		{
		case 0:
			if (player->PlayerInventory.Seed >= 1)
			{
				player->PlayerInventory.Seed -= 1;
			}
			break;
		case 1:
			if (player->PlayerInventory.Trap >= 1)
			{
				player->PlayerInventory.Trap -= 1;
			}
			break;
		case 2:
			if (player->PlayerInventory.Wall >= 1)
			{
				player->PlayerInventory.Wall -= 1;
			}
			break;
		case 3:
			if (player->PlayerInventory.Bed >= 1)
			{
				player->PlayerInventory.Bed -= 1;
			}
			break;
		case 4:
			if (player->PlayerInventory.Chair >= 1)
			{
				player->PlayerInventory.Chair -= 1;
			}
			break;
		case 5:
			if (player->PlayerInventory.FireTower >= 1)
			{
				player->PlayerInventory.FireTower -= 1;
			}
			break;
		case 6:
			if (player->PlayerInventory.Cook >= 1)
			{
				player->PlayerInventory.Cook -= 1;
			}
			break;
		case 7:
			bQuit = true;
			break;
		default:
			break;
		}
	}
	_PrintInfo(showPos.X, showPos.Y + 0, "                         ");
	_PrintInfo(showPos.X, showPos.Y + 1, "                         ");
	_PrintInfo(showPos.X, showPos.Y + 2, "                         ");
	_PrintInfo(showPos.X, showPos.Y + 3, "                         ");
	_PrintInfo(showPos.X, showPos.Y + 4, "                         ");
	_PrintInfo(showPos.X, showPos.Y + 5, "                         ");
	_PrintInfo(showPos.X, showPos.Y + 6, "                         ");
	_PrintInfo(showPos.X, showPos.Y + 7, "                         ");
	_PrintInfo(showPos.X, showPos.Y + 8, "                         ");
}


Placer::~Placer()
{
	delete InteractiveCons;
}
