#ifndef _CORE_H_
#define _CORE_H_

class Position {
public:
    Position();
    Position(int x, int y);
    int getRow() const;
    int getCol() const;
private:
    int row;
    int col;
};

class MoveCommand {
public:
    MoveCommand();
    bool timeup;
    bool turn;
    bool select;
    bool enter;
    int y;
    int x;
};

#endif // _CORE_H_