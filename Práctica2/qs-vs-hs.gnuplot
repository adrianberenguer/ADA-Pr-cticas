#Adrián Berenguer Agulló, 74445262N
set terminal png

set output "quickSort.png"

set xlabel "size of arrays"
set ylabel "Average CPU time (ms)"

set title  "Quicksort"

plot "quickSort.CPUtimes" using 1:2 with lines title "random arrays", "sorted arrays", "reverse-sorted arrays" 


##########################################################################
#Testing different analytical functions to determine which best fits:

#set output "quickSort_and_Fit-Function.png"
#set title "Quick Sort: Average time complexity and Fit function"


#------------------------------------------------------------------------

# Testing 3 different functions:
#Uncomment the function to be tested and comment the rest:

##Fit to a (n log(n) + k) function: 
#y(x)=a*x*log(x) + b
#fitFunction="Fitting quicksort time values to f(n) = n log(n)+k"
#fit y(x) "quickSort.CPUtimes" using 1:2 via a,b


##Fit to a (n log n + n) function: 
#y(x)=a*x*log(x) + b*x + c
#fitFunction="Fitting quicksort time values to f(n) = n log(n)+n"
#fit y(x) "quickSort.CPUtimes" using 1:2 via a,b,c


##Fit to a cuadratic function: 
#y(x)=a*x*x + b*x + c
#fitFunction="Fitting quicksort time values to f(n) = n²+n+k"
#fit y(x) "quickSort.CPUtimes" using 1:2 via a,b,c

#------------------------------------------------------------------------


#Finally draw both functions (CPU times of sample and the one that best fits) on one graph
#plot "quickSort.CPUtimes" using 1:2 with lines title "CPU times of sample", y(x) title fitFunction  


