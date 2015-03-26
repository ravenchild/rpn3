CC=gcc
CFLAGS=-c -Wall -O3
DBFLAGS=-c -Wall -ggdb
LDFLAGS=-lm
SOURCES=instructions.c main.c queue.c stack.c variables.c tokenize.c functions.c execute.c display.c error.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=rpn3
DBEXECUTABLE=_debug
EXTRA=bottles.rpn extra.rpn factorial.rpn gcd.rpn
OUTDIR=bin/

all: $(SOURCES) $(EXECUTABLE)
	cp $(EXTRA) $(OUTDIR)

mac:
	$(CC) -Wall -I/Developer/SDKs/MacOSX10.6.sdk/usr/include/ -L/Developer/SDKs/MacOSX10.6.sdk/usr/lib/ -lm -O3 -o $(OUTDIR)$(EXECUTABLE) $(SOURCES)
	cp $(EXTRA) $(OUTDIR)

debug:
	$(CC) $(DBFLAGS) -o $(OUTDIR)$(EXECUTABLE)$(DBEXECTABLE) $(SOURCES)
	cp $(EXTRA) $(OUTDIR)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(OUTDIR)$@

.c.o:
	$(CC) $(CFLAGS) $< -o $(OUTDIR)$@

