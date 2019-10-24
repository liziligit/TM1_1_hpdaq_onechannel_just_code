fileId=$1
pedeId=$2
# lowLimitPixel=$3 # minimum signal value of pixel 
# upLimitPixel=$4  # maximum signal value of pixel 
ip=$3 #the Num of pixel
store_file_name=$4
#choose the diractory of data
# data_dir=/Users/lizili/Desktop/CSNS/20190917_TM1_1_hpdaq_onechannel_just_code/data
# data_dir=/Users/lizili/Desktop/TM1_1_hpdaq_onechannel_mac/data
# data_dir=../data
data_dir=/Users/lizili/Downloads/Topmetal_cable_test

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

# if [ "$3" = "" ]
# then
#    echo TH2D Min Range is empty
#    exit
#  fi

# if [ "$4" = "" ]
# then
#    echo TH2D Max Range is empty
#    exit
# fi

if [ "$3" = "" ]
then
  echo IP number is empty
  exit
fi

if [ "$4" = "" ]
then
  echo store_file_name is empty
  exit
fi

script_BM=../script_bm


dataFile=${data_dir}/${store_file_name}
pedeFile=${data_dir}/${store_file_name}
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
#下在两行也可以运行，但是慢
# echo root  -L -l -q -b "pd1_SyncDraw2D_energy_spectrum_root.cpp++(\"${dataFile}\", ${fileId}, \"${pedeFile}\", ${pedeId}, ${upLimitPixel}, ${lowLimitPixel}, ${ip})"
# root  -L -l "pd1_SyncDraw2D_energy_spectrum_root.cpp++(\"${dataFile}\", ${fileId}, \"${pedeFile}\", ${pedeId}, ${upLimitPixel}, ${lowLimitPixel}, ${ip}, \"${store_file_name}\")"

# g++ -o pd1_SyncDraw2D_energy_spectrum pd1_SyncDraw2D_energy_spectrum.cpp `root-config --cflags` `root-config --glibs` #`root-config --libs`
# ./pd1_SyncDraw2D_energy_spectrum ${dataFile} ${fileId} ${pedeFile} ${pedeId} ${upLimitPixel} ${lowLimitPixel} ${ip} ${store_file_name}

g++ -o pd1_SyncDraw2D_single_pixel_adc pd1_SyncDraw2D_single_pixel_adc.cpp `root-config --cflags` `root-config --glibs` #`root-config --libs`
./pd1_SyncDraw2D_single_pixel_adc ${dataFile} ${fileId} ${pedeFile} ${pedeId} ${ip} ${store_file_name} #ip is pixel number

cd ${script_BM}


