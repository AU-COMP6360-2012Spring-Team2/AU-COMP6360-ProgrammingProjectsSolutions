UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
	CXX=g++ -std=c++0x -g
endif
ifeq ($(UNAME), Darwin)
	CXX=g++-4.6 -std=c++0x
endif


CXXFLAGS += -Wall -I/home/cse_h2/szg0031/include
LIBS += -lm -lpthread 

OBJ_T=physical_world.o HelloMsg.o neighborTable.o MPR_selectors.o S.o vehicle.o message.o cache.o configuration.o
OBJ_MPR=$(OBJ_T) util.o
OBJ_NOMPR=$(OBJ_T) util_2.o

all : mpr nompr

mpr : $(OBJ_MPR) main.o
	$(CXX) $(CFLAGS) -o main.mpr.x $(OBJ_MPR) main.o $(LIBS)

nompr : $(OBJ_NOMPR) main.o
	$(CXX) $(CFLAGS) -o main.nompr.x $(OBJ_NOMPR) main.o $(LIBS)

test : test.o
	$(CXX) $(CFLAGS) -o test.x test.o $(LIBS) && echo && echo "Running test..." && echo && ./test.x

clean : clean-test
	rm -f main.mpr.x main.nompr.x $(OBJ_MPR) $(OBJ_NOMPR) main.o

clean-test : 
	rm -f test.x test.o
