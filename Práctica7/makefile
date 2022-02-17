#Adrián Berenguer Agulló, 74445262N
	
OPTS = -Wall -O3 -std=c++0x

potter: potter-greedy.cc
	g++ ${OPTS} -o potter-greedy potter-greedy.cc

tar: potter-greedy.cc makefile
	tar cvfz 74445262N.tar.gz potter-greedy.cc makefile

clean:
	-rm potter-greedy
