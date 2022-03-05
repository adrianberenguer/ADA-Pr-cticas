#Adrián Berenguer Agulló, 74445262N

OPTS = -Wall -O3 -std=c++0x

qs-vs-hs: qs-vs-hs.cc
	g++ ${OPTS} -o qs-vs-hs qs-vs-hs.cc	 

qs-vs-hs.Msteps: qs-vs-hs
	./qs-vs-hs > qs-vs-hs.Msteps

graphs: qs-vs-hs.Msteps qs-vs-hs.gnuplot
	gnuplot qs-vs-hs.gnuplot

all: qs-vs-hs qs-vs-hs.Msteps graphs

tar: ejercicio2.pdf ejercicio3.pdf
	tar cvfz 74445262N.tar.gz ejercicio2.pdf ejercicio3.pdf

clean: 
	-rm qs-vs-hs qs-vs-hs.Msteps fit.log *.png 
