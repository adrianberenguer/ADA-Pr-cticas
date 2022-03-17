//Adrián Berenguer Agulló, 74445262N
#include <iostream>
#include <math.h>

using namespace std;

unsigned int pasos1 = 0;
unsigned int pasos2 = 0;
unsigned int pasos3 = 0;

// coste: \Theta(log n) 
unsigned long pow2_1(unsigned n) {
    pasos1++;

    if(n == 0) {
		return 1;
    }
	if(n%2 == 0) {
		return (pow2_1(n/2)) * (pow2_1(n/2));
    }

 	return 2 * (pow2_1(n/2)) * (pow2_1(n/2));
}

// coste: \Theta(n)
unsigned long pow2_2(unsigned n) {
    pasos2++;

    if(n == 0) {
        return 1;
    }

    return (2 * pow2_2(n-1));
}

// coste: \Theta(2^n)
unsigned long pow2_3(unsigned n) {
    pasos3++;

    if(n == 0) {
		return 1;
    }

	return pow2_3(n-1) + pow2_3(n-1);
}

int main(void) {
    unsigned long pow1 = 0, pow2 = 0, pow3 = 0;

    for(unsigned n = 1; n < 32; n++)
    {
		pasos1 = pasos2 = pasos3 = 0;
        //cout << "n: " << n << "\t";

		pow1 = pow2_1(n);
        if(pow1 == pow(2, n)) {
            cout << pasos1 << "\t";
        }
		
		pow2 = pow2_2(n);
        if(pow2 == pow(2, n)) {
            cout << pasos2 << "\t";    
        }
		
		pow3 = pow2_3(n);
        if(pow3 == pow(2, n)) {
            cout << pasos3 << endl;
        }
		
	}

	return 0;
}