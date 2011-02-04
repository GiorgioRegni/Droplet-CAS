BIN=bin/droplet-cas-fcgi

DPL_CFLAGS=`pkg-config --cflags droplet-1.0`
DPL_LDFLAGS=`pkg-config --libs droplet-1.0`

FCGI_CFLAGS=-I/usr/local/include
FCGI_LDFLAGS=-L/usr/local/lib -lfcgi

SRC=$(wildcard src/*.c)
OBJ=$(SRC:.c=.o)

CFLAGS=$(DPL_CFLAGS) $(FCGI_CFLAGS) -Isrc -g
LDFLAGS=$(DPL_LDFLAGS) $(FCGI_LDFLAGS) -g
CC = gcc

all: $(BIN)

$(BIN): $(OBJ) 
	$(CC) $(LDFLAGS) -o $(BIN) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) *~ $(bin)
