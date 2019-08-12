dataFolder=$1
dataNum=$2
pedeNum=$3
dataFile=/Users/lizili/Desktop/TM1_1_hpdaq_onechannel_mac/data/runData/${dataFolder}/xbeam_${dataNum}
pedeFile=/Users/lizili/Desktop/TM1_1_hpdaq_onechannel_mac/data/runData/${dataFolder}/xpede_${pedeNum}
cppPath=../topmetal1X8


ich=$4
iframe=$5
upLimit=$6
lowLimit=$7

root -L  -l -q -b "${cppPath}/pd1_analysis_test.cpp++(\"${dataFile}\", \"${pedeFile}\", ${ich},${iframe}, ${upLimit}, ${lowLimit})" 
