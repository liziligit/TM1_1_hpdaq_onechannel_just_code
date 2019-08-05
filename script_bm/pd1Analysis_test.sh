dataFolder=$1
dataNum=$2
pedeNum=$3
dataFile=../data/runData/${dataFolder}/xbeam_${dataNum}
pedeFile=../data/runData/${dataFolder}/xpede_${pedeNum}
cppPath=../topmetal1X8


ich=$4
iframe=$5
upLimit=$6
lowLimit=$7

root -L  -l -q -b "${cppPath}/pd1_analysis_test.cpp++(\"${dataFile}\", \"${pedeFile}\", ${ich},${iframe}, ${upLimit}, ${lowLimit})" 
