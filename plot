#!/usr/bin/gnuplot -rv

set terminal x11 1 noraise size 800,480
set multiplot 
set grid
set autoscale y
set autoscale x
unset mouse

set ylabel 'Sensor value'

set timefmt "%H:%M:%S %d %m %Y"
set format x "%m-%d\n%H:%M"
set xdata time
set datafile separator ","

set size 1.0,0.8
set origin 0.0,0.2
set key bottom left box opaque
set border back
plot for [i=2:9] '/home/pi/nose/array-0-small-3.csv' using 1:i title ' A'.(i-2) with lines 

set nokey
set xtics format ''
set ylabel 'Temperature'
set size 1.0,0.2
set origin 0.0,0.0
plot '/home/pi/nose/array-0-small-3.csv' using 1:11 title 'Temperature' with lines

unset multiplot
pause 31
reread
refresh
