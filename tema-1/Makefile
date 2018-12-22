CC := gcc
CFLAGS := -Wall

ifeq ($(shell which node),)
$(error node missing)
endif
STANDARD := $(shell node -p "require('./.vscode/c_cpp_properties.json').configurations[0].cStandard")
DEFINES := $(shell node -p "require('./.vscode/c_cpp_properties.json').configurations[0].defines.join(' ')")

INC_DIR := include
SRC_DIR := src
OBJ_DIR := obj

$(shell mkdir -p $(INC_DIR) $(SRC_DIR) $(OBJ_DIR))

BIN := telefon
SRC := $(wildcard $(SRC_DIR)/*.c) $(BIN).c
OBJ := $(subst $(BIN).o,$(OBJ_DIR)/$(BIN).o,$(subst $(SRC_DIR)/,$(OBJ_DIR)/,$(SRC:%.c=%.o)))
DEP := $(OBJ:%.o=%.d)


.PHONY: print-%
print-%:
	@echo $*=$($*)


.DEFAULT_GOAL := build
build: $(BIN)


$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $(BIN)


-include $(DEP)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c Makefile
	$(CC) -I$(INC_DIR) -MMD -c $(STANDARD:%=-std=%) $(CFLAGS) $(DEFINES:%=-D%) $< -o $@
$(OBJ_DIR)/$(BIN).o: $(BIN).c Makefile
	$(CC) -I$(INC_DIR) -MMD -c $(STANDARD:%=-std=%) $(CFLAGS) $(DEFINES:%=-D%) $< -o $@


.PHONY: run
run: build
	@./$(BIN)

.PHONY: clean
clean:
	@-rm -f $(OBJ) $(DEP) $(BIN)


.PHONY: debug
debug:
	$(eval CFLAGS += -g)

.PHONY: valgrind
valgrind: debug build
	@valgrind --tool=memcheck --leak-check=full ./$(BIN)
