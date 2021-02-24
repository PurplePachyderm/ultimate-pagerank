# Vars

CC=g++

CPPFLAGS=-Wall
OPT_FLAGS=-O3

SRC=src
BIN=bin

OBJ_FILES=$(patsubst $(SRC)/%.cpp, $(BIN)/%.o, $(wildcard $(SRC)/*.cpp))


# Targets

all: basic epidemic

basic: $(OBJ_FILES)
	$(CC) examples/$@.cpp  $(CPPFLAGS) $(OPT_FLAGS) -o $@ $(OBJ_FILES)

epidemic: $(OBJ_FILES)
	$(CC) examples/$@.cpp  $(CPPFLAGS) $(OPT_FLAGS) -o $@ $(OBJ_FILES)

$(OBJ_FILES): $(BIN)/%.o : $(SRC)/%.cpp
	$(CC) $(CPPFLAGS) $(OPT_FLAGS) $< -c -o $@


# Phonies

.PHONY: clean clean_o clean_examples

clean: clean_o clean_examples

clean_o:
	find $(BIN) -name "*.o" -type f -delete

clean_examples:
	rm basic epidemic
