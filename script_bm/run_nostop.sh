#read  -p "请输入存储文件夹名(按n退出)：" store_file_name

#--------------------------------------------
# set default store_file_name
#--------------------------------------------

#if [ -z "${store_file_name}" ];then
	store_file_name=default
#fi

#case ${store_file_name} in
#N|n|NO|no)
#  exit;;
#"")  #Autocontinue
#  break;;
#esac

./initiTm2-.sh 0.78 0.62
sleep 1
./clearTm2-.sh
sleep 1
./debugTm2-.sh
sleep 1
./pede.sh 1
sleep 1
rm ../data/runData/9nostop/*.pd1
./runData.sh 1 11 s 9nostop
./pd1SyncDraw2D.sh 1 1 -10 10 1 9nostop
rm ../data/runData/9nostop/*.pd1

for i in $(seq 1 100) 
do
echo begin to run ${i}
sleep 1 
./runData.sh 1 11 s 9nostop
./pd1SyncDraw2D.sh 1 1 -10 10 1 9nostop
sleep 1
rm ../data/runData/9nostop/*.pd1
done
