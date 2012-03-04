UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
	CXX=g++ -std=c++0x
endif
ifeq ($(UNAME), Darwin)
	CXX=g++-4.6 -std=c++0x
endif


CFLAGS+=-Wall -I. -I../include -Iinclude
LIBS += -lm -lpthread


OBJ= cache.o configuration.o  message.o  S.o  util.o  vehicle.o 
ALL_OBJ= util.o

DEPS = configuration.h  message.h  pg.h  S.h  vehicle.h 


memory: $(OBJ) main.o
	$(CXX) $(CFLAGS) -o main.x $(ALL_OBJ) main.o $(LIBS)

clean:
	rm -f main.x $(OBJ) main.o 

distclean: clean
	rm -f $(DEP) *~ .buildflags
