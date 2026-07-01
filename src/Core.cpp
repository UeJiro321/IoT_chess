#include "Core.h"

Position::Position() : row(0), col(0) {}
Position::Position(int x, int y) : row(x), col(y) {}
int Position::getRow() const{
    return this->row;
}
int Position::getCol() const{
    return this->col;
}

MoveCommand::MoveCommand() : timeup(0), turn(0), select(0), enter(0), y(0), x(0){}