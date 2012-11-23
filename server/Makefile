#!/bin/bash
TARGET=main
SRCS=main.c nokialcd.c

CC=gcc
CFLAGS=-Wall
LIBS=wiringPi
OBJS=$(SRCS:.c=.o)

all: $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -l$(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm *.o $(TARGET)
