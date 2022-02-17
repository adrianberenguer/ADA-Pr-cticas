#Adrián Berenguer Agulló, 74445262N
	
OPTS = -Wall -O3 -std=c++0x

potter: potter_bt.cc
	g++ ${OPTS} -o potter_bt potter_bt.cc

tar: potter_bt.cc makefile
	tar cvfz 74445262N.tar.gz potter_bt.cc makefile

clean:
	-rm potter_bt
