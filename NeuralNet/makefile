PROGRAM = neuralnet 
SOURCEPATH = src
OBJECTS = neuralnet.o
OBJECTPATH = obj
COMPILER = gcc
CFLAGS = -c -s --std=c99
LFLAGS = -lm

all: $(PROGRAM)

%.o: $(SOURCEPATH)/%.c
	$(COMPILER) $(CFLAGS) -o $(OBJECTPATH)/$@ $< 
	
$(PROGRAM): $(OBJECTS)
	$(COMPILER) $(addprefix $(OBJECTPATH)/, $(OBJECTS)) -o $(PROGRAM) $(LFLAGS)
	
clean:
	rm $(PROGRAM) $(OBJECTPATH)/*
