CC=gcc209
CCFLAGS=-g -D_DEFAULT_SOURCE -D_GNU_SOURCE
TARGET=ish
OBJ=ish.o lexicalAnalysis.o dynarray.o processEachLine.o syntacticAnalysis.o cmd.o execute.o builtin.o

# Pattern rule
%.o: %.c
	$(CC) $(CCFLAGS) -c $<

# Create programs
ish: $(OBJ)
	$(CC) $(CCFLAGS) $? -o $@
tester_lexicalAnalysis: tester_lexicalAnalysis.o lexicalAnalysis.o dynarray.o
	$(CC) $(CCFLAGS) $? -o $@
tester_cmd: tester_cmd.o cmd.o
	$(CC) $(CCFLAGS) $? -o $@

# Complie each .c files
ish.o: lexicalAnalysis.h dynarray.h
dynarray.o : dynarray.h
lexicalAnalysis.o: lexicalAnalysis.h dynarray.h
processEachLine.o: processEachLine.h dynarray.h cmd.h lexicalAnalysis.h syntacticAnalysis.h
syntacticAnalysis.o: syntacticAnalysis.h dynarray.h cmd.h
cmd.o: cmd.h
execute.o: execute.h dynarray.h cmd.h builtin.h
builtin.o: builtin.h dynarray.h cmd.h

# Basic commands
all: $(TARGET)
clobber: clean
	rm -f *~ \#*\# core
clean:
	rm -f $(TARGET) *.o