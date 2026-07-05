.RECIPEPREFIX := >

# ============================================================
# Target
# ============================================================
TARGET = chess_app

# ============================================================
# Build mode
#   make MODE=pc
#   make MODE=arm
# ============================================================
MODE ?= arm

# ============================================================
# Directories
# ============================================================
SRC_DIR   = src
INC_DIR   = include
BUILD_DIR = build

# ============================================================
# Common source files
# ============================================================
COMMON_SRCS = \
    $(SRC_DIR)/main.cpp \
    $(SRC_DIR)/UI.cpp \
    $(SRC_DIR)/Game.cpp \
    $(SRC_DIR)/Piece.cpp \
    $(SRC_DIR)/Core.cpp \
    $(SRC_DIR)/Board.cpp

# ============================================================
# Mode settings
# ============================================================
ifeq ($(MODE),arm)

CROSS_COMPILE = arm-linux-gnueabihf-
CXX = $(CROSS_COMPILE)g++

# ARMビルド用のフラグにハードウェアライブラリのパスとマクロを追加
CXXFLAGS_EXTRA = \
    -DUSE_HARDWARE_INPUT \
    -Dsoc_cv_av \
    -I${SOCEDS_DEST_ROOT}/ip/altera/hps/altera_hps/hwlib/include \
    -I${SOCEDS_DEST_ROOT}/ip/altera/hps/altera_hps/hwlib/include/soc_cv_av

LDFLAGS_EXTRA = -static

SRCS = \
    $(COMMON_SRCS) \
    $(SRC_DIR)/HardwareInput.cpp

else ifeq ($(MODE),pc)

CXX = g++

CXXFLAGS_EXTRA = \
    -DUSE_CONSOLE_INPUT

LDFLAGS_EXTRA =

SRCS = \
    $(COMMON_SRCS) \
    $(SRC_DIR)/ConsoleInput.cpp

else
$(error MODE must be arm or pc)
endif

# ============================================================
# Flags
# ============================================================
CXXFLAGS = \
    -I$(INC_DIR) \
    -Wall \
    -Wextra \
    -std=c++17 \
    -g \
    $(CXXFLAGS_EXTRA)

LDFLAGS = \
    -g \
    -Wall \
    $(LDFLAGS_EXTRA)

# ============================================================
# Object files
# ============================================================
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# ============================================================
# Default target
# ============================================================
all: $(TARGET)

# ============================================================
# Link
# ============================================================
$(TARGET): $(OBJS)
>$(CXX) $(LDFLAGS) $^ -o $@

# ============================================================
# Compile
# ============================================================
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
>@mkdir -p $(BUILD_DIR)
>$(CXX) $(CXXFLAGS) -c $< -o $@

# ============================================================
# Clean
# ============================================================
.PHONY: clean

clean:
>rm -rf $(BUILD_DIR) $(TARGET) $(TARGET).exe *.o *~