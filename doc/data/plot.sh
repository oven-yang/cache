plot "./PPDS/avg-req-dist" u 1:2 with linespoints title "PPDS","./LRU/avg-req-dist" u 1:2 with linespoints title "LRU","./no-cache/avg-req-dist" u 1:2 with linespoints title "no cache","./LRU-random/avg-req-dist" u 1:2 with linespoints title "LRU-random"
set xlabel "request quantity"
set ylabel "average request distance"
set term pngcairo lw 2
set output "./avg-req-dist.png"
replot
set output

plot "./PPDS/cache-hit-rate" u 1:2 with linespoints title "PPDS", "./LRU/cache-hit-rate" u 1:2 with linespoints title "LRU","./LRU-random/cache-hit-rate" u 1:2 with linespoints title "LRU-random"
set xtics 1
set xlabel "request quantity"
set ylabel "average cache hit rate"
set term pngcairo lw 2
set output "./cache-hit-rate.png"
replot
set output

plot "./PPDS/cache-replace-rate" u 1:2 with linespoints title "PPDS", "./LRU/cache-replace-rate" u 1:2 with linespoints title "LRU","./LRU-random/cache-replace-rate" u 1:2 with linespoints title "LRU-random"
set xtics 1
set xlabel "request quantity"
set ylabel "average cache replace rate"
set term pngcairo lw 2
set output "./cache-replace-rate.png"
replot
set output
