CC=clang
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=main.c cpu.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=tony6502

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(OBJECTS) $(EXECUTABLE)
