#Adrián Berenguer Agulló, 74445262N
	
OPTS = -Wall -O3 -std=c++0x

potter: potter_bb.cc
	g++ ${OPTS} -o potter_bb potter_bb.cc

tar: potter_bb.cc makefile memoria.pdf
	tar cvfz 74445262N.tar.gz potter_bb.cc makefile memoria.pdf

clean:
	-rm potter_bb
