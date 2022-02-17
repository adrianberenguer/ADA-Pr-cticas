#Adrián Berenguer Agulló, 74445262N
	
OPTS = -Wall -O3 -std=c++0x

potter: potter.cc
	g++ ${OPTS} -o potter potter.cc

tar: potter.cc makefile
	tar cvfz 74445262N.tar.gz potter.cc makefile

clean:
	-rm potter
