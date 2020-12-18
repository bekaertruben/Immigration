# Tutorial used: https://stackoverflow.com/a/30602701/9387553
SRC_DIR := src
OBJ_DIR := obj
EXE := game

# SRC: any .c file in SRC_DIR
# OBJ: %.o for every %.c in SRC
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CC := gcc
CPPFLAGS := -Iinclude -MMD -MP
CFLAGS   := -Wall -g
LDFLAGS  := -Llib
LDLIBS   := -lSDL2

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(OBJ_DIR) $(EXE) default_savefile.txt

# read each of the .d files (created by -MMD -MP) to handle dependencies
-include $(OBJ:.o=.d) 
