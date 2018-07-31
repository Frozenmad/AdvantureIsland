#include "Tree.h"
#include "player.h"

Tree::Tree()
{
	ShowChar = 'T';
}

void Tree::onCollect(Player * player)
{
	player->PlayerInventory.Wood += 1;
}
