set datafile separator ','
set autoscale
set term png

set output "levy.png"
plot "levy.csv" using 1:2 with lines

set output "brownian.png"
plot "brownian.csv" using 1:2 with lines

set output "brownian_hist.png"

set boxwidth 0.05 absolute

set style fill solid 1.0 noborder

bin_width = 0.1;

bin_number(x) = floor(x/bin_width)

rounded(x) = bin_width * ( bin_number(x) + 0.5 )

plot 'brownian.csv' using (rounded($2)):(2) smooth frequency with boxes

set output "levy_hist.png"

plot 'levy.csv' using (rounded($2)):(2) smooth frequency with boxes