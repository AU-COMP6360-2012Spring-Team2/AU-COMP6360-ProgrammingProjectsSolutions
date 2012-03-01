CC=g++
CFLAGS+=-Wall -I. -I../include -Iinclude
LIBS += -lm -lpthread

#VPATH = ../domain/ ../bs/ ../bufm/

OBJ= configuration.o  message.o  S.o  util.o  vehicle.o 
ALL_OBJ= util.o

#DEPS = memory.h ../domain/domain.h
DEPS = configuration.h  message.h  pg.h  S.h  vehicle.h 

#ARCHIVE=$(XGARROOT)/lib/libxgar.a

memory: $(OBJ) main.o
	$(CC) -o main.x $(ALL_OBJ) main.o $(LIBS)
#$(CC) -o main.x $(OBJ) main.o $(LIBS)

#all: $(ARCHIVE)

#$(ARCHIVE): $(ALL_OBJ)
#	$(AR) $(ARFLAGS) $@ $(ALL_OBJ)

clean:
	rm -f main.x $(OBJ) main.o 

distclean: clean
	rm -f $(DEP) *~ .buildflags

#include $(XGARROOT)/config/targets.mk
