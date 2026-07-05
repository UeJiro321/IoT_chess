#include "ConsoleInput.h"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <cstdint>

#define SOFTWARE_DONE_BIT (1u << 10)

ConsoleInput::ConsoleInput()
    : softwareDone(false)
{
}

MoveCommand ConsoleInput::getMove()
{
    // すでにデバッグ入力から生成済みのコマンドがあれば、それを返す
    if (!pendingCommands.empty()) {
        MoveCommand cmd = pendingCommands.front();
        pendingCommands.pop();
        std::cout << "input command < " << cmd.turn << " " << cmd.select << " " << cmd.enter << " "
             << cmd.x << " " << cmd.y << std::endl;
        return cmd;
    }

    std::cout << "input command" << " > ";

    std::string line;
    std::getline(std::cin >> std::ws, line);

    MoveCommand selectCmd{};
    MoveCommand moveCmd{};

    // まず "black d7 d5" 形式として解釈してみる
    if (parseDebugMove(line, selectCmd, moveCmd)) {
        pendingCommands.push(moveCmd);

        return selectCmd;
    }

    // それ以外は従来の bit 入力として処理する
    std::istringstream iss(line);
    int bit[10];

    for (int i = 9; i >= 0; i--) {
        if (!(iss >> bit[i])) {
            std::cout << "[ConsoleInput] invalid input. Use bit mode or move mode." << std::endl;

            MoveCommand emptyCmd{};
            return emptyCmd;
        }

        if (bit[i] != 0 && bit[i] != 1) {
            std::cout << "[ConsoleInput] bit must be 0 or 1." << std::endl;

            MoveCommand emptyCmd{};
            return emptyCmd;
        }
    }

    uint32_t data = 0;

    for (int i = 0; i <= 9; i++) {
        if (bit[i]) {
            data |= (1u << i);
        }
    }

    return decodeCommand(data);
}

void ConsoleInput::gameStart()
{
    std::cout << "[ConsoleInput] Game started." << std::endl;
}

void ConsoleInput::clearDone(){
    std::cout << "[ConsoleInput] Clear done." << std::endl;
    std::cout << "[ConsoleInput] [10:11]=00" << std::endl;
}

MoveCommand ConsoleInput::decodeCommand(uint32_t data)
{
    MoveCommand cmd{};

    cmd.timeup = static_cast<bool>((data >> 9) & 0x1);
    cmd.turn   = static_cast<bool>((data >> 8) & 0x1);
    cmd.select = static_cast<bool>((data >> 7) & 0x1);
    cmd.enter  = static_cast<bool>((data >> 6) & 0x1);

    cmd.y = static_cast<int>((data >> 3) & 0x7);
    cmd.x = static_cast<int>(data & 0x7);

    return cmd;
}

bool ConsoleInput::parseDebugMove(
    const std::string& line,
    MoveCommand& selectCmd,
    MoveCommand& moveCmd
) {
    std::istringstream iss(line);

    std::string colorText;
    std::string fromText;
    std::string toText;

    if (!(iss >> colorText >> fromText >> toText)) {
        return false;
    }

    bool turn = false;
    int fromRow = 0;
    int fromCol = 0;
    int toRow = 0;
    int toCol = 0;

    if (!parseColor(colorText, turn)) {
        return false;
    }

    if (!parseSquare(fromText, fromRow, fromCol)) {
        return false;
    }

    if (!parseSquare(toText, toRow, toCol)) {
        return false;
    }

    // 1つ目: 駒選択
    selectCmd.timeup = false;
    selectCmd.turn   = turn;
    selectCmd.select = false;
    selectCmd.enter  = true;
    selectCmd.x      = fromRow;
    selectCmd.y      = fromCol;

    // 2つ目: 移動先決定
    moveCmd.timeup = false;
    moveCmd.turn   = turn;
    moveCmd.select = true;
    moveCmd.enter  = true;
    moveCmd.x      = toRow;
    moveCmd.y      = toCol;

    return true;
}

bool ConsoleInput::parseColor(const std::string& text, bool& turn) const
{
    std::string s = text;

    std::transform(
        s.begin(),
        s.end(),
        s.begin(),
        [](unsigned char c) {
            return static_cast<char>(std::tolower(c));
        }
    );

    /*
        現在の盤面では:
          true  = 大文字側 = 8段目/7段目 = blue
          false = 小文字側 = 2段目/1段目 = red
    */
    if (s == "blue" || s == "b") {
        turn = true;
        return true;
    }

    if (s == "red" || s == "r") {
        turn = false;
        return true;
    }

    return false;
}

bool ConsoleInput::parseSquare(const std::string& text, int& row, int& col) const
{
    if (text.size() != 2) {
        return false;
    }

    char file = static_cast<char>(std::tolower(static_cast<unsigned char>(text[0])));
    char rank = text[1];

    if (file < 'a' || file > 'h') {
        return false;
    }

    if (rank < '1' || rank > '8') {
        return false;
    }

    col = file - 'a';
    row = '8' - rank;

    return true;
}

void ConsoleInput::notifyDone(bool success)
{
    if (success) {
        std::cout << "[ConsoleInput] Command executed successfully." << std::endl;
        std::cout << "[ConsoleInput] [10:11]=01" << std::endl;
    } else {
        std::cout << "[ConsoleInput] Invalid command." << std::endl;
        std::cout << "[ConsoleInput] [10:11]=10" << std::endl;
    }
}