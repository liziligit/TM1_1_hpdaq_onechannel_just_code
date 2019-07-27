um=$1
DR=`ls`
for dir in $DR
do
        if [ -d "$dir" ]; then
                cd "$dir"
                fileNum=`ls -l |grep "^-"|wc -l`
                if [ "$num" -ne "$fileNum" ];then
                        echo `pwd` >> ../error.txt
                fi
                cd -
        fi
done
