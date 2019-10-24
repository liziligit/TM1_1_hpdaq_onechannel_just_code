fileId=$1
pedeId=$2
lowLimitPixel=$3 # minimum signal value of pixel 
upLimitPixel=$4  # maximum signal value of pixel 
ip=$5
store_file_name=$6
#choose the diractory of data
# data_dir=/Users/lizili/Downloads/
data_dir=/Users/lizili/Desktop/TM1_1_hpdaq_onechannel_mac/data
# data_dir=../data

if [ "$1" = "" ]
then
  echo Data number is empty
  exit
fi

if [ "$2" = "" ]
then
  echo Pedestal number is empty
  exit
fi

if [ "$3" = "" ]
then
   echo TH2D Min Range is empty
   exit
 fi

if [ "$4" = "" ]
then
   echo TH2D Max Range is empty
   exit
fi

if [ "$5" = "" ]
then
  echo IP number is empty
  exit
fi

if [ "$6" = "" ]
then
  echo store_file_name is empty
  exit
fi

script_BM=../script_bm


dataFile=${data_dir}/runData/${store_file_name}
pedeFile=${data_dir}/runData/${store_file_name}
cppPath=../topmetal1X8

#check if pd1 exist
# echo "$beamFile"
# fileNum=`ls -l ${beamFile}|grep ".pd1"|wc -l`
# r=$?

# if [ $r == 1 ]; then
#         echo "no found pd1, exit"
#         exit
# elif [ $r == 0 ]; then
#         echo "found ${fileNum} pd1"
#         #exit 0
# fi

cd ${cppPath}
g++ -o pd1_syncDraw2D_to_txt  pd1_syncDraw2D_to_txt.cpp `root-config --cflags` `root-config --glibs`
#./pd1_syncDraw2D ${dataFile} ${fileId} ${pedeFile} ${pedeId} ${upLimitPixel} ${lowLimitPixel} ${ip}
./pd1_syncDraw2D_to_txt ${dataFile} ${fileId} ${pedeFile} ${pedeId} ${upLimitPixel} ${lowLimitPixel} ${ip} ${store_file_name}
#root  -L -l -q  "${cppPath}/pd1_syncDraw2D.cpp++(\"${dataFile}\", ${fileId}, \"${pedeFile}\", ${pedeId}, ${upLimitPixel}, ${lowLimitPixel}, ${ip})"  
cd ${script_BM}


