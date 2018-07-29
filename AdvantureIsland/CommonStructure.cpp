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
