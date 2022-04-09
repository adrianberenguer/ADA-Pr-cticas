#Adrián Berenguer Agulló, 74445262N
	
OPTS = -Wall -O3 -std=c++0x

met_greedy: met_greedy.cc
	g++ ${OPTS} -o met_greedy met_greedy.cc

tar: met_greedy.cc makefile
	tar cvfz 74445262N.tar.gz met_greedy.cc makefile

clean:
	-rm met_greedy
