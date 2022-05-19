#Adrián Berenguer Agulló, 74445262N
	
OPTS = -Wall -O3 -std=c++0x

met: met_bb.cc
	g++ ${OPTS} -o met_bb met_bb.cc

tar: met_bb.cc makefile memoria.pdf
	tar cvfz 74445262N.tar.gz met_bb.cc makefile memoria.pdf

clean:
	-rm met_bb
