# coding=UTF-8
import datetime, time
import random
import threading

##这个是模拟随机数据的函数，每0.5秒写一次，写100次
def random_data():
    i = 0
    with open(r'time_current.dat', 'a+') as out:
        while True:
            tm = datetime.datetime.strftime(datetime.datetime.now(),'%Y-%m-%d_%H:%M:%S')
            rd = random.random()
            out.write('{} {:.6f}\n'.format(tm, rd))
            out.flush()
            time.sleep(0.1)
            # i += 1
            # if i > 100:
            #     return

create_data_thread = threading.Thread(target=random_data)
create_data_thread.start()