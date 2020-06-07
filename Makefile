SRCDIR = src
INCDIR = include
OBJDIR = obj
OUTDIR = bin
DIRS = $(SRCDIR) $(INCDIR) $(OBJDIR) $(OUTDIR)

CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99
LDFLAGS = -lm

EXEC = $(OUTDIR)/template

SRC = $(shell find $(SRCDIR) -type f -name *.c)
OBJ=$(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))
INC  = -I$(INCDIR)

all: $(DIRS) $(EXEC)

$(DIRS): %:
	mkdir -p $@

$(EXEC): $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $(CFLAGS) $(INC) $< -o $@

clean:
	rm -rf $(OUTDIR)
	rm -rf $(OBJDIR)

.PHONY: all clean
