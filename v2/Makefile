CC = gcc
CFLAGS = -Wall
LDFLAGS = -lsqlite3
OBJ_DIR = obj
SRC_DIR = src
SRCS = main.c $(SRC_DIR)/utils.c $(SRC_DIR)/db.c
OBJS = $(OBJ_DIR)/main.o $(OBJ_DIR)/utils.o $(OBJ_DIR)/db.o
TARGET = main

obj_dir:
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.c | obj_dir
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | obj_dir
	@$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	@$(CC) $(CFLAGS)  $(OBJS) -o $(TARGET) $(LDFLAGS)

clean:
	@rm -rf $(OBJ_DIR) $(TARGET)
	@echo "Cleaned up build files."