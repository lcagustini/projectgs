SOURCES := src
OBJS = $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))

CC = gcc

COMPILER_FLAGS = -w

LINKER_FLAGS = -lSDL2 -lSDL2_image

OBJ_NAME = main

all:
	$(CC) $(SOURCES)/$(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
