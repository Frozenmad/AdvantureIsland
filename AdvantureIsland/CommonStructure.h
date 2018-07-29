#pragma once

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