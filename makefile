# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Iinclude -g

# Directories
SRC_DIR = src
EXPR_DIR = $(SRC_DIR)/Expr
OBJ_DIR = obj
BIN_DIR = bin

# Target
TARGET = lox

# Source and object files
SRCS = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(EXPR_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))
OBJS := $(patsubst $(EXPR_DIR)/%.cpp,$(OBJ_DIR)/Expr/%.o,$(OBJS))

# Default target
all: $(BIN_DIR)/$(TARGET)

# Linking
$(BIN_DIR)/$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compilation rule
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/Expr/%.o: $(EXPR_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
