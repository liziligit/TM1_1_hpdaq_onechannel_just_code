: '
#read  -p "请输入存储文件夹名(按n退出)：" store_file_name

#--------------------------------------------
# set default store_file_name
#--------------------------------------------

if [ -z "${store_file_name}" ];then
	store_file_name=default
fi

case ${store_file_name} in
N|n|NO|no)
  exit;;
"")  #Autocontinue
  break;;
esac
'


./initiTm2-.sh 0.78 0.62
sleep 1
./clearTm2-.sh
sleep 1
./debugTm2-.sh
sleep 1
./pede.sh 2



: '
sleep 1
./runData.sh 1 5 s 1120alphahv600-02
sleep 1 
./pd1SyncDraw2D.sh 1 1 -10 10 1 1120alphahv600-02
'
