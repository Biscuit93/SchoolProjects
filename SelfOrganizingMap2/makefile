PROGRAM = assign2
SOURCEPATH = src
OBJECTS = main.o init.o input.o math.o data.o config.o som.o shuffle.o graphics.o
OBJECTPATH = obj
COMPILER = gcc
CFLAGS = -c -s
LFLAGS = -lSDL2 -lm

all: $(PROGRAM)

%.o: $(SOURCEPATH)/%.c $(SOURCEPATH)/%.h $(SOURCEPATH)/defs.h
	$(COMPILER) $(CFLAGS) -o $(OBJECTPATH)/$@ $< 
	
$(PROGRAM): $(OBJECTS)
	$(COMPILER) $(addprefix $(OBJECTPATH)/, $(OBJECTS)) -o $(PROGRAM) $(LFLAGS)

test: $(PROGRAM)
	./$(PROGRAM)

run:
	./$(PROGRAM)

stats:
	wc -l $(SOURCEPATH)/*

clean:
	rm $(PROGRAM) $(OBJECTPATH)/*
