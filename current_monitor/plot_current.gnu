## run: gnuplot -e "projname='$proj'" plot_current.gnu

set xdata time
##set timefmt x "%Y-%m-%d_%H:%M:%S"
#set timefmt "%Y-%m-%d_%H:%M:%S"
set timefmt "%Y-%m-%d_%H:%M:%S"
#set format x "%M:%S" 
if (!exists("projname")) projname='./project_test'
set xlabel "Time"
#set xrange [-60:60]
#set y left axis kedu is red
set ytics nomirror tc lt 1

#set y left label is red
set ylabel "Current [V]" tc lt 1

#set y left label kedu is blue
set y2label 'Current [V]' tc lt 3

#set autoscale y2

#set y left axis kedu is blue
set y2tics nomirror tc lt 3

plot "time_current.dat" using 1:2 title "Current" with lines

pause 10
reread

set xrange [-60:60]
