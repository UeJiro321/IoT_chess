#include "components.h"

MoveCommand::MoveCommand(){
    this->state = 0;
    this->turn = true;
    this->isEnter = false;
    this->dir = 0;
    this->isTimeout = false;
}

void ConsoleOutput::drawBoard(const Board& board){
    for(int i = 0; i < 8; i++){
        std::cout << "  +";
        for(int j = 0; j < 8; j++){
            std::cout << "---+";
        }
        std::cout << std::endl;
        std::cout << 8-i << " ";
        for(int j = 0; j < 8; j++){
            Position pos(i, j);
            const Piece* p = board.getPiece(pos);
            if(p != nullptr) std::cout << "| " << p->getSymbol() << " ";
        }
        
        std::cout << "|" << std::endl;
    }
    std::cout << "  +";
    for(int i = 0; i < 8; i++){
        std::cout << "---+";
    }
    std::cout << std::endl;
    std::cout << "   ";
    for(int i = 0; i < 8; i++){
        std::cout << " " << char('a' + i) << " ";
        std::cout << " ";
    }
    std::cout << std::endl;
}

void ConsoleOutput::drawResult(bool winner){
    if(winner){
        std::cout << "You win!" << std::endl;
    } else {
        std::cout << "You lose!" << std::endl;
    }
}