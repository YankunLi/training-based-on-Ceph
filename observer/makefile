CC = g++

FLAGS = -Wall -g -o

OBJS = main.o Work.o config.o

OBJS_SRC = main.cpp 

all: $(OBJS)
	$(CC) $(FLAGS) main $^


main.o: $(OBJS_SRC)
	$(CC) -g -c $<

Work.o: Work.cpp Work.h observer.h
	$(CC) -g -c $<

config.o: config.cpp config.h observer.h
	$(CC) -g -c $<

clean:
	rm -rf main $(OBJS) tags
