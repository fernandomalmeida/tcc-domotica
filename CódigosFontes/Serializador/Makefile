############################# Makefile ##########################
#Definimos a variável
CC=gcc
CPP=g++
CFLAGS=-W -Wall -ansi -pedantic -std=c99
TARGET=main
SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:.cpp=.o)

all: $(TARGET) teste

# $@ = teste:
# $^ = teste.o main.o
$(TARGET): $(OBJS)
	$(CPP) -o $@ $^ 
%.o: %.c
	$(CPP) -c $^ -o $@  $(CFLAGS)
#.PHONY: clean mrproper teste
clean: 
	rm -rf *.o
	rm -rf *~
cleanAll: clean
	rm -rf $(TARGET)

teste: $(TARGET)
	./$^ < in.txt
