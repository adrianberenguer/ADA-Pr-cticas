#Adrián Berenguer Agulló, 74445262N
	
OPTS = -Wall -O3 -std=c++0x

met_bt: met_bt.cc
	g++ ${OPTS} -o met_bt met_bt.cc

tar: met_bt.cc makefile
	tar cvfz 74445262N.tar.gz met_bt.cc makefile

clean:
	-rm met_bt