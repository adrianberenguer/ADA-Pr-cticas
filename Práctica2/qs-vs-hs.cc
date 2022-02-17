//Adrián Berenguer Agulló, 74445262N
/*
ADA. 2021-22
Práctica 2:"Empirical analysis by means of program-steps account of two sorting algorithms: Middle-Quicksort and Heapsort."
*/
#include <unistd.h>
#include <iostream>
#include <math.h>

using namespace std;

float PASOSHS = 0.0;
float PASOSQS = 0.0;

//--------------------------------------------------------------
// Middle Quick Sort
void
middle_QuickSort(int * v, long left, long right){
    long i,j;
    int pivot;
    PASOSQS++; //
	if (left<right){
		i=left; j=right;
		pivot=v[(i+j)/2];
        PASOSQS++; //
		do{
			while (v[i]<pivot) i++;
			while (v[j]>pivot) j--;
			if (i<=j){ 
				swap(v[i], v[j]);
				i++; j--;
                PASOSQS++; //
			}
		}while (i<=j);
		if (left < j)  middle_QuickSort(v,left,j);
		if (i < right) middle_QuickSort(v,i,right);
	}
}

//--------------------------------------------------------------                
//HEAPSORT
// Procedure sink used by Heapsort algorith
// Sink an element (indexed by i) in a tree to keep the heap property. n is the size of the heap. 

void sink(int *v, size_t n, size_t i)
{
    size_t largest;
    size_t l, r; //left and right child

    do{
        largest = i;  // Initialize largest as root
        l = 2*i + 1;  // left = 2*i + 1
        r = 2*i + 2;  // right = 2*i + 2
        PASOSHS++; //
        // Is left child larger than root?
        if (l < n && v[l] > v[largest]) {
            largest = l; 
            PASOSHS++; //
        }
    
        // Is right child larger than largest so far
        if (r < n && v[r] > v[largest]) {
            largest = r; 
            PASOSHS++; //
        }
    
        // If largest is still root then the process is done
        if (largest == i) break;
        
        // If not, swap new largest with current node i and repeat the process with childs. 
        swap(v[i], v[largest]);
        i=largest;  
    } while (true);
    
}
 
//--------------------------------------------------------------                
// Heapsort algorithm (ascending sorting)
void heapSort(int *v, size_t n)
{
    // Build a MAX-HEAP with the input array
    for (size_t i = n / 2 - 1; i >= 0; i--){
        sink(v, n, i); PASOSHS++; //
        if (i==0) break; //as size_t is unsigned type
	}

  	// A this point we have a HEAP on the input array, let's take advantage of this to sort the array:
    // One by swap the first element, which is the largest (max-heap), with the last element of the vector and rebuild heap by sinking the new placed element on the begin of vector.  
    for (size_t i=n-1; i>0; i--)
    {
        PASOSHS++; //
		// Move current root to the end.
        swap(v[0], v[i]);
 		// Now largest element is at the end but do not know if the element just moved to the beginning is well placed, so a sink process is required.
		// Note that, at the same time, the HEAP size is reduced one element by one.
        sink(v, i, 0);
		// The procedure ends when HEAP has only one element, the lower of the input array
    }
}

int
main(void) {
    cout << "#QUICKSORT VERSUS HEAPSORT." <<endl
    << "#Average processing Msteps (millions of program steps)" <<endl
    << "#Number of samples (arrays of integer): 30" <<endl<<endl
    << "#                 RANDOM ARRAYS       SORTED ARRAYS     REVERSE SORTED ARRAYS" <<endl
    << "#             -------------------  -------------------  ---------------------" <<endl
    << "#    Size     QuickSort  HeapSort  QuickSort  HeapSort   QuickSort   HeapSort " <<endl
    << "#============================================================================" <<endl;

    for (int exp = 15; exp <= 20; exp++){
        size_t size = size_t( pow(2,exp) );
        int* v1 = new int [size];
        int* v2 = new int [size];
        int* v3 = new int [size];

        if (!v1 || !v2 || !v3){
            cerr << "Error, not enough memory!" << endl;
            exit (EXIT_FAILURE);  
        }

        cout << "    " << size << "     " << std::flush;

        //Random Arrays
        for(size_t x = 0; x < 30; x++) {
            for (size_t j = 0; j < size; j++) {
                v1[j] = rand(); 
            }        
            middle_QuickSort(v1, 0, size);
            heapSort(v1, size);
        }
        cout  << PASOSQS / 1000000 / 30 << "   ";
        cout  << PASOSHS / 1000000 / 30 << "   ";

        PASOSHS = 0.0;
        PASOSQS = 0.0;

        //Sorted Arrays
        for(size_t y = 0; y < 30; y++) {
            for(size_t i = 0; i < size - 1; i++) {
                v2[i] = i;
            }
            middle_QuickSort(v2, 0, size);
            heapSort(v2, size);
        }
        cout  << PASOSQS / 1000000 / 30 << "   ";
        cout  << PASOSHS / 1000000 / 30 << "   ";

        PASOSHS = 0.0;
        PASOSQS = 0.0;

        //Reverse Sorted Arrays
        for(size_t z = 0; z < 30; z++) {
            for(size_t k = size; k > 0; k--) {
                v2[k] = k;
            }
            middle_QuickSort(v3, 0, size);
            heapSort(v3, size);
        }
        cout  << PASOSQS / 1000000 / 30 << "   ";
        cout  << PASOSHS / 1000000 / 30 << endl;

        PASOSHS = 0.0;
        PASOSQS = 0.0;

        delete[] v1;
        delete[] v2;
        delete[] v3;
    }
    //DUDAS:
    //Hay que dividir entre 1000000
    //hay que hacer 30 de average?
    //donnde poner pasos?
    //pq me da todo el rato mismo resultado?
    //como hacer gnuplot
}