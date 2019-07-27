fileIdStart=$1
let fileIdEnd=${fileIdStart}+1
store_file_name=$2

if [ "$1" = "" ]
then
  echo Pedestal number is empty 
  exit	
fi

if [ "$2" = "" ]
then
  echo Pedestal filename is empty 
  exit	
fi

mkdir ../data/runData/${store_file_name}

#sampleBytes=$2 # small: 45M = 47185920;
#sampleBytes=47185920
sampleBytes=1718592
script_BM=../script_bm
#cd ${script_BM}
pedeDataChar=xpede
#pedeDataPathLocal=../data/pedeData
pedeDataPathLocal=../data/runData/${store_file_name}

cd ../daq

pede=../data/runData/${store_file_name}/xpede
#displayMode=2 ### save pd1 file for all channel
displayMode=3 ### save pd1 file for 0 channel

# if realTime.c is changed, we need to recompile this file to generate readDdrContinue.ce using ./mytcp -c realTime.c in ../daq	
echo ./realTime.ce  -fn ${pede} -fileStart ${fileIdStart} -fileEnd ${fileIdEnd} -sampleBytes ${sampleBytes} -displayMode ${displayMode}
./realTime.ce  -fn ${pede} -fileStart ${fileIdStart} -fileEnd ${fileIdEnd}  -sampleBytes ${sampleBytes} -displayMode ${displayMode}


cd ../topmetal1X8


echo root -L -l -q -b "pede.cpp++(\"${pedeDataPathLocal}\",\"${pedeDataChar}_\",${fileIdStart})"
root -L -l -q -b "pede.cpp++(\"${pedeDataPathLocal}\",\"${pedeDataChar}_\",${fileIdStart})"


cd ${script_BM}
