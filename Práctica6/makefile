#Adrián Berenguer Agulló, 74445262N
	
OPTS = -Wall -O3 -std=c++0x

met: met.cc
	g++ ${OPTS} -o met met.cc

tar: met.cc makefile
	tar cvfz 74445262N.tar.gz met.cc makefile

clean:
	-rm met
