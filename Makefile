# Makefile for Red Bull Moth Project 2015
SDIR=src
ODIR=$(SDIR)/obj
CC=gcc
CXX=g++
CFLAGS=-Wall -pthread -lmraa
FILE=moth

_OBJ=main.o gps.o stringpot.o log.o i2c.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

_HEADERS=shared.h log.h defines.h
HEADERS=$(patsubst %,src/%,$(_HEADERS))

$(ODIR)/%.o: $(SDIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(FILE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean

clean:
	rm -f moth $(ODIR)/* $(SDIR)/*~

clean-logs:
	rm -rf ~/log/*

imu-test: tools/imuTester.cpp
	$(CXX) -Wall -lupm-adxl345 -lupm-itg3200 -lupm-hmc58831 -o $@ $^
