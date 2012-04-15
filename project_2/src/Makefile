UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
	CXX=g++ -std=c++0x
endif
ifeq ($(UNAME), Darwin)
	CXX=g++-4.6 -std=c++0x
endif


CFLAGS += -Wall -I.
LIBS += -lm -lpthread

OBJ= 

all : $(OBJ) main.o
	$(CXX) $(CFLAGS) -o main.x $(OBJ) main.o $(LIBS)

test : $(OBJ) test.o
	$(CXX) $(CFLAGS) -o test.x $(OBJ) test.o $(LIBS) && echo && echo "Running test..." && echo && ./test.x

clean : clean-test
	rm -f main.x $(OBJ) main.o

clean-test : 
	rm -f test.x test.o
