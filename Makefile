# コンパイラとコンパイルオプション
CXX = g++
CXXFLAGS = -Iinclude -Wall -Wextra -std=c++17

# 生成する実行ファイル名
TARGET = chess_app

# コンパイルするソースファイルのリスト
SRCS = src/main.cpp src/UI.cpp src/Game.cpp src/Piece.cpp src/Core.cpp src/Board.cpp
# SRCS = src/test1.cpp src/UI.cpp src/Game.cpp src/Piece.cpp src/Core.cpp src/Board.cpp


# デフォルトのターゲット（makeと打った時に実行される）
all: $(TARGET)

# 実行ファイルを生成するためのルール
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# クリーンアップ（生成物を削除する）
# Windows環境とLinux(WSL)環境の両方を考慮
clean:
	-rm -f $(TARGET) $(TARGET).exe