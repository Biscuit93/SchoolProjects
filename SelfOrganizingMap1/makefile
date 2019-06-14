PROGRAM = kohonenmap
SOURCEPATH = src
OBJECTS = main.o init.o graphics.o input.o tsp.o kohonen.o math.o shuffle.o
OBJECTPATH = obj
COMPILER = gcc
CFLAGS = -c -s
LFLAGS = -lSDL2 -lm

all: $(PROGRAM)

%.o: $(SOURCEPATH)/%.c $(SOURCEPATH)/defs.h
	$(COMPILER) $(CFLAGS) -o $(OBJECTPATH)/$@ $< 
	
$(PROGRAM): $(OBJECTS)
	$(COMPILER) $(addprefix $(OBJECTPATH)/, $(OBJECTS)) -o $(PROGRAM) $(LFLAGS)

test: $(PROGRAM)
	./$(PROGRAM)

run:
	./$(PROGRAM)

clean:
	rm $(PROGRAM) $(OBJECTPATH)/*
