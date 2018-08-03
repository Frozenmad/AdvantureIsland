#pragma once

#include <Windows.h>
#include <conio.h>
#include <vector>
#include <string>
#include <iostream>
#include <mutex>
#include "GlobalParameter.h"

using namespace std;

struct Position
{
	int X;
	int Y;
	Position() : X(0), Y(0) {}
	Position(int x, int y) : X(x), Y(y) {}
	Position& operator=(const Position& inPos);
	Position operator+(const Position anotherPos) const;
	Position operator-(const Position anotherPos) const;
};

class ConsoleMap
{
private:
	HANDLE hOut;
	vector<string> MapInstruction;
	int NowId;
	int BeginId;
	int EndId;
	WORD SelectedColor;
	WORD UnSelectedColor;
	WORD OtherColor;
	int MaxLine;
	int nowBegin;
	bool bCalapse;
public:
	ConsoleMap(HANDLE out, vector<string> mi, int ml = -1, int ni = 0, int bi = 0, int ei = -1, WORD sc = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY
		, WORD uc = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY, WORD oc = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	void AddMapInstruction(string ins);
	void AddMapInstruction(vector<string> ins);
	void PrintOnViewPort(int initX, int initY);
	void UpdateSelect(int direction);
	void AddToViewport() {}
	int GetSelectedItem() { return NowId - BeginId; }
};

void GameInstruction(void (*CallFunc)());

void _PrintInfo(int x, int y, string target, WORD color = FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

void ClearInputBuffer();