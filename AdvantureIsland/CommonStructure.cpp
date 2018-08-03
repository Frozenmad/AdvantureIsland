#include "CommonStructure.h"

Position & Position::operator=(const Position & inPos)
{
	X = inPos.X;
	Y = inPos.Y;

	return *this;
}

Position Position::operator+(const Position anotherPos) const
{
	return Position(X + anotherPos.X, Y + anotherPos.Y);
}

Position Position::operator-(const Position anotherPos) const
{
	return Position(X - anotherPos.X, Y - anotherPos.Y);
}

ConsoleMap::ConsoleMap(HANDLE out, vector<string> mi, int ml, int ni, int bi, int ei, WORD sc, WORD uc, WORD oc)
{
	hOut = out;
	for (auto mess : mi)
	{
		MapInstruction.push_back(mess);
	}
	NowId = ni;
	BeginId = bi;
	EndId = ei == -1 ? MapInstruction.size() : ei;
	SelectedColor = sc;
	UnSelectedColor = uc;
	OtherColor = oc;
	MaxLine = ml == -1 ? (EndId - BeginId) : ml;
	bCalapse = ml != -1;
	nowBegin = BeginId;
}

void ConsoleMap::AddMapInstruction(string ins)
{
	MapInstruction.push_back(ins);
}

void ConsoleMap::AddMapInstruction(vector<string> ins)
{
	for (auto mess : ins)
	{
		MapInstruction.push_back(mess);
	}
}

void ConsoleMap::PrintOnViewPort(int initX, int initY)
{
	COORD Pos;
	Pos.X = initX; Pos.Y = initY;
	GlobalParameter::OutputLock();
	SetConsoleCursorPosition(hOut, Pos);
	for (int i = 0; i < MapInstruction.size(); i++)
	{
		if (i == nowBegin && bCalapse)
		{
			SetConsoleTextAttribute(hOut, UnSelectedColor);
			cout << "↑ for more";
			Pos.Y++;
			SetConsoleCursorPosition(hOut, Pos);
		}
		if (i == NowId)
		{
			SetConsoleTextAttribute(hOut, SelectedColor);
			cout << MapInstruction[i] << endl;
			Pos.Y++;
			SetConsoleCursorPosition(hOut, Pos);
		}
		else if (i >= nowBegin && i < nowBegin + MaxLine)
		{
			SetConsoleTextAttribute(hOut, UnSelectedColor);
			cout << MapInstruction[i] << endl;
			Pos.Y++;
			SetConsoleCursorPosition(hOut, Pos);
		}
		else if (i < BeginId || i >= EndId)
		{
			SetConsoleTextAttribute(hOut, OtherColor);
			cout << MapInstruction[i] << endl;
			Pos.Y++;
			SetConsoleCursorPosition(hOut, Pos);
		}

		if (i == nowBegin + MaxLine - 1 && bCalapse)
		{
			SetConsoleTextAttribute(hOut, UnSelectedColor);
			cout << "↓ for more";
			Pos.Y++;
			SetConsoleCursorPosition(hOut, Pos);
		}
		SetConsoleTextAttribute(hOut, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	}
	GlobalParameter::OutputUnLock();
}

void ConsoleMap::UpdateSelect(int direction)
{
	if (direction == 1)
	{
		if (NowId != EndId - 1)
		{
			if (NowId == nowBegin + MaxLine - 1 && bCalapse)
			{
				nowBegin++;
			}
			NowId++;
		}
	}
	else if (direction == -1)
	{
		if (NowId != BeginId)
		{
			if (NowId == nowBegin && bCalapse)
			{
				nowBegin--;
			}
			NowId--;
		}
	}
}

void ClearInputBuffer()
{
	while (_kbhit())
	{
		_getch();
	}
}

void GameInstruction(void(*CallFunc)())
{
	ClearInputBuffer();
	system("cls");
	_PrintInfo(38,0,"介绍");
	_PrintInfo(2, 1, "荒岛求生，玩家需要在一个荒芜的小岛上，依靠自己的种植和打猎技术为生，直到救援赶来。");
	_PrintInfo(60, 4, "【任意键返回上一级】");
	_getch();
	CallFunc();
}

void _PrintInfo(int x, int y, string target, WORD color)
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD Pos;
	Pos.X = x; Pos.Y = y;
	GlobalParameter::OutputLock();
	SetConsoleCursorPosition(hOut,Pos);
	SetConsoleTextAttribute(hOut, color);
	cout << target;
	SetConsoleTextAttribute(hOut, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
	GlobalParameter::OutputUnLock();
}
