#Adrián Berenguer Agulló, 74445262N
OPTS = -Wall -O3 -std=c++0x

all: pow2 pow2.steps graphs

pow2: pow2.cc
	g++ ${OPTS} -o pow2 pow2.cc 

pow2.steps: pow2
	./pow2 > pow2.steps

graphs: pow2.steps pow2.gnuplot
	gnuplot pow2.gnuplot

tar: pow2.cc pow2.gnuplot makefile
	tar cvfz 74445262N.tar.gz pow2.cc pow2.gnuplot makefile

clean: 
	-rm pow2 pow2.steps fit.log *.png 
 
