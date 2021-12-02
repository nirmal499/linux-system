CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic
LIBS=-lm
OBJECT_FILES_DIR=object_files
SOURCE_FILE_DIR=source_files
OBJS=area.o $(OBJECT_FILES_DIR)/help.o $(OBJECT_FILES_DIR)/rectangle.o $(OBJECT_FILES_DIR)/triangle.o $(OBJECT_FILES_DIR)/circle.o
DEPS=header_files/area.h
bindir=/usr/local/bin

area: $(OBJS)
	$(CC) $(CFLAGS) -o area.out $(OBJS) $(LIBS)

area.o: $(DEPS)

$(OBJECT_FILES_DIR)/%.o:  $(SOURCE_FILE_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm area.o area.out

