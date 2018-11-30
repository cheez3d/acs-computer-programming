CC := gcc
CFLAGS := -Wall -std=gnu99 -DTASK1 -DTASK2 -DTASK3 -DTASK4

BIN := telefon
INC_DIR := include
SRC_DIR := src
OBJ_DIR := obj

$(shell mkdir -p $(INC_DIR) $(SRC_DIR) $(OBJ_DIR))

SRC := $(wildcard $(SRC_DIR)/*.c) $(BIN).c
OBJ := $(subst $(BIN).o,$(OBJ_DIR)/$(BIN).o,$(subst $(SRC_DIR)/,$(OBJ_DIR)/,$(SRC:%.c=%.o)))
DEP := $(OBJ:%.o=%.d)


build: $(BIN)


$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $(BIN)

-include $(DEP)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c Makefile
	$(CC) -I$(INC_DIR) -MMD -c $(CFLAGS) $< -o $@
$(OBJ_DIR)/$(BIN).o: $(BIN).c Makefile
	$(CC) -I$(INC_DIR) -MMD -c $(CFLAGS) $< -o $@


run: build
	@./$(BIN)

clean:
	@-rm -f $(OBJ) $(DEP) $(BIN)
.PHONY: clean


print-%:
	@echo $*=$($*)
