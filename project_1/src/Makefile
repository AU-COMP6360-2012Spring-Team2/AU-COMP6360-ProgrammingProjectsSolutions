CC=g++
CFLAGS+=-Wall -I. -I../include -Iinclude
LIBS += -lm -lpthread


OBJ= configuration.o  message.o  S.o  util.o  vehicle.o 
ALL_OBJ= util.o

DEPS = configuration.h  message.h  pg.h  S.h  vehicle.h 


memory: $(OBJ) main.o
	$(CC) -o main.x $(ALL_OBJ) main.o $(LIBS)

clean:
	rm -f main.x $(OBJ) main.o 

distclean: clean
	rm -f $(DEP) *~ .buildflags

