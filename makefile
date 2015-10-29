# Easily adaptable makefile for Advanced Programming
# Note: remove comments (#) to activate some features
#
# author Vitor Carreira
# date 2010-09-26


# Libraries to include (if any)
LIBS= -lm #-pthread

# Compiler flags
CFLAGS=-Wall -W -Wmissing-prototypes

# Indentation flags
IFLAGS=-br -brs -npsl -ce -cli4


# Name of the executable
PROGRAM=paCodec

# Prefix for the gengetopt file (if gengetopt is used)
PROGRAM_OPT=pagengetopt

# Object files required to build the executable
PROGRAM_OBJS=main.o debug.o memory.o utils.o filehandler.o ${PROGRAM_OPT}.o

# Clean and all are not files
.PHONY: clean all docs indent debugon

all: ${PROGRAM}

# compilar com depuracao
debugon: CFLAGS += -D SHOW_DEBUG -g
debugon: ${PROGRAM}

${PROGRAM}: ${PROGRAM_OBJS}
	${CC} -o $@ ${PROGRAM_OBJS} ${LIBS}

# Dependencies
main.o: main.c debug.h memory.h utils.h pagengetopt.h filehandler.h ${PROGRAM_OPT}.h
${PROGRAM_OPT}.o: ${PROGRAM_OPT}.c ${PROGRAM_OPT}.h

cmdline.o: cmdline.c cmdline.h
debug.o: debug.c debug.h
filehandler.o: filehandler.c utils.h memory.h debug.h filehandler.h
memory.o: memory.c memory.h
pagengetopt.o: pagengetopt.c pagengetopt.h
utils.o: utils.c utils.h memory.h debug.h filehandler.h


#how to create an object file (.o) from C file (.c)
.c.o:
	${CC} ${CFLAGS} -c $<

# Generates command line arguments code from gengetopt configuration file
${PROGRAM_OPT}.h: ${PROGRAM_OPT}.ggo
	gengetopt < ${PROGRAM_OPT}.ggo --file-name=${PROGRAM_OPT}

clean:
	rm -f *.o core.* *~ ${PROGRAM} *.bak ${PROGRAM_OPT}.h ${PROGRAM_OPT}.c

docs: Doxyfile
	doxygen Doxyfile

Doxyfile:
	doxygen -g Doxyfile

indent:
	indent ${IFLAGS} *.c *.h
