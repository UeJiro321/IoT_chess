#include <iostream>
#include <string>
#include "Game.h"
using namespace std;


int main() {
    Board board;
    ConsoleOutput output;
    string str;

    while(1){
        cout << "cmd: ";
        string fromStr, toStr;
        cin >> fromStr >> toStr;
        if(fromStr[0] < 'a' || fromStr[0] > 'h' || fromStr[1] < '1' || fromStr[1] > '8' ||
           toStr[0] < 'a' || toStr[0] > 'h' || toStr[1] < '1' || toStr[1] > '8'){
            cout << "Invalid command" << endl;
            return 1;
        }
        Position from(8 - (fromStr[1] - '0'), fromStr[0] - 'a');
        Position to(8 - (toStr[1] - '0'), toStr[0] - 'a');
        if(board.movePiece(from, to)){
            cout << "Move successful" << endl;
        } else {
            cout << "Move failed" << endl;
        }
        output.drawBoard(board, Position(1, 1));
    }

    return 0;
}