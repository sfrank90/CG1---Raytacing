# Makefile for the project. Replace "MicroTrace09" with "MicroTrace<task>".
# Note on makefiles:
# (1) second lines must start with tab, not spaces.
# (2) % is a wildcard to match given targets (not filenames! that is *).
# (3) $< is the name of the first dependency
# (4) $@ is the name of the target

.SUFFIXES: .cpp .a

all: MicroTrace07

CC = g++
CFLAGS = -O3 -Wall -ggdb

%.o: src/%.cpp include/*.h
	$(CC) $(CFLAGS) -I./include/ \
	-c $< -o $@

%.a:
	ar r $@ $<

#libMicroTrace99.a: Image.o Scene99.o

MicroTrace01: Image.o MicroTrace01.o
MicroTrace02: Image.o MicroTrace02.o
MicroTrace03: Image.o MicroTrace03.o
MicroTrace04: Image.o MicroTrace04.o
MicroTrace05: Image.o MicroTrace05.o
MicroTrace06: Image.o MicroTrace06.o
MicroTrace07: Image.o MicroTrace07.o
MicroTrace08: Image.o MicroTrace08.o
MicroTrace09: Image.o MicroTrace09.o #libMicroTrace99.a

clean:
	rm -f *.o MicroTrace* libMicroTrace*.a output/*.ppm output/*.gif 2>/dev/null; echo "Cleaned."
