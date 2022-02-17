//Adrián Berenguer Agulló, 74445262N
/*
ADA 2021-22
Práctica 1: "Empirical analysis of Quicksort average-time complexity."
*/
#include <unistd.h>
#include <iostream>
#include <math.h>
#include <chrono>

using namespace std;
using namespace chrono;


//--------------------------------------------------------------
// Middle QuickSort
void 
middle_QuickSort(int * v, long left, long right){

    long i,j;
    int pivot,aux; 
    if (left<right)
    {
        i=left; j=right;
        pivot=v[(i+j)/2];
        do
        {
            while (v[i]<pivot) i++;
            while (v[j]>pivot) j--;
            if (i<=j)
            {
                aux=v[i]; v[i]=v[j]; v[j]=aux;
                i++; j--;
            }
       } while (i<=j);
       if (left<j)  middle_QuickSort(v,left,j);
       if (i<right) middle_QuickSort(v,i,right);
    }
}

int
main(void){

//    srand(getpid());
    srand(0);

    cout << "# QuickSort CPU times in milliseconds:"
        << endl
        << "# Size \t Average CPU time (ms.)"
        << endl
        << "# ----------------------------"
        << endl;

    for (int exp = 15; exp <= 20; exp++){
        size_t size = size_t( pow(2,exp) );
        int* v = new int [size];
        if (!v){
            cerr << "Error, not enough memory!" << endl;
            exit (EXIT_FAILURE);  
        }

        cout << size << "\t\t" << std::flush;

        double result = 0;
        for(size_t i = 0; i < 30; i++) {
            for (size_t j = 0; j < size; j++) {
                v[j] = rand(); 
            }
            
                auto start = clock();
                middle_QuickSort(v, 0,size);
                auto end = clock();
                result += 1000.0 * (end-start) / CLOCKS_PER_SEC;
        }

        cout << result / 30.0  << endl;

        for (size_t i = 1; i < size; i++)
            if (v[i] < v[i-1]){ 
                cerr << "Panic, array not sorted! " << i << endl; 
                exit(EXIT_FAILURE);            
            }

        delete[] v; 
    }
}