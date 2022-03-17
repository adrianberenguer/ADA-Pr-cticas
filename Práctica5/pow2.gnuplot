#Adrián Berenguer Agulló, 74445262N

set terminal png
set xrange [0:32]
set yrange [0:70]
set output "pow2.png"

set xlabel "n"
set ylabel "Pasos"

plot "pow2.steps" using 1:1 with lines title "pow2(n) =  2 * pow2(n-1)",\
     "pow2.steps" using 1:2 with lines title "pow2(n) = pow2(n/2) * pow2(n/2)",\
     "pow2.steps" using 1:3 with lines title "pow2(n) = pow2(n-1) + pow2(n-1)",\
