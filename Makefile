# Makefile for Red Bull Moth Project 2015
# Author: James Vaughan
SDIR=src
ODIR=$(SDIR)/obj
CC=gcc
RM=rm -rf
CFLAGS=-Wall
LDLIBS=-pthread -lmraa -lm -lrt
FILE=moth

_OBJ=main.o gps.o log.o sonar.o imu.o i2c.o arduino.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

_HEADERS=shared.h log.h
HEADERS=$(patsubst %,src/%,$(_HEADERS))

$(ODIR)/%.o: $(SDIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(FILE): $(OBJ)
	$(CC) $(LDLIBS) -o $@ $^

.PHONY: clean

clean:
	$(RM) $(FILE) $(ODIR)/* $(SDIR)/*~

clean-logs:
	$(RM) /home/root/log/*

install:
	cp $(FILE) /bin/moth

push:
	rsync -rP --delete . debian@192.168.7.2:~/moth-project
