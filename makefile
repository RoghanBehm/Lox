# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -fsanitize=address,undefined -Iinclude -g
#Valgrind # CXXFLAGS = -std=c++20 -Wall -Wextra -Iinclude -g -O0

# Directories
SRC_DIR = src
EXPR_DIR = $(SRC_DIR)/Expr
STMT_DIR = $(SRC_DIR)/Stmt
OBJ_DIR = obj
BIN_DIR = bin

# Target
TARGET = lox

# Source files
SRCS = \
  $(wildcard $(SRC_DIR)/*.cpp) \
  $(wildcard $(EXPR_DIR)/*.cpp) \
  $(wildcard $(STMT_DIR)/*.cpp)

# Object files
OBJS = \
  $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(wildcard $(SRC_DIR)/*.cpp)) \
  $(patsubst $(EXPR_DIR)/%.cpp, $(OBJ_DIR)/Expr/%.o, $(wildcard $(EXPR_DIR)/*.cpp)) \
  $(patsubst $(STMT_DIR)/%.cpp, $(OBJ_DIR)/Stmt/%.o, $(wildcard $(STMT_DIR)/*.cpp))

# Default target
all: $(BIN_DIR)/$(TARGET)

# Linking
$(BIN_DIR)/$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compilation rules
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/Expr/%.o: $(EXPR_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/Stmt/%.o: $(STMT_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
