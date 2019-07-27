
#################### for test


#cd ../debug
#rm -f a.jpg
#root -l pd1Draw1D.cpp\(4,4,1\) 
#shotwell a.jpg

#cd ../gui


#################### real script



dataSet=0


if [ "$1" = "" ]
then
  echo pixel is emnpty
  exit
fi

if [ "$2" = "" ]
then
  echo channel is emnpty
  exit
fi

if [ "$3" = "" ]
then
  echo dataSet is emnpty
  exit
fi

if [ "$4" = "" ]
then
  echo store_file_name is emnpty
  exit
fi



addr=$1
cha=$2
dataSet=$3
store_file_name=$4

dataFile=../data/runData/${store_file_name}

echo pixel addr= $addr
echo pixel channel= $cha



#cd ../debug
#rm -f a.jpg
#rm -f ${dataFile}/decay_${store_file_name}_${dataSet}_pix${addr}.png

g++ -o pd1Draw1D pd1Draw1D.cpp `root-config --cflags` `root-config --glibs`
./pd1Draw1D ${addr} ${cha} ${dataSet} ${store_file_name}

#eog a.jpg
eog ${dataFile}/decay_${store_file_name}_${dataSet}_pix${addr}.png

#cd ../gui
