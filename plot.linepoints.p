reset
dataFile="data.input.tsv"
set output "input.png"
set xlabel "input_size"
set ylabel "runtime (us)"
set term png size 1024,1024
set logscale x 2
set logscale y 2
set grid
set key reverse Left bottom
plot dataFile using ($2):($3) notitle with points
