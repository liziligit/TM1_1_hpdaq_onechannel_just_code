# coding=UTF-8
# import datetime, time
# import random
# import threading

# 这个是模拟随机数据的函数，每1秒写一次，写100次
# def random_data():
#     i = 0
#     with open('time_current.dat', 'w') as out:
#         while True:
#             tm = datetime.datetime.strftime(datetime.datetime.now(),'%Y-%m-%d_%H:%M:%S')
#             rd = random.random()
#             out.write('{} {:.6f}\n'.format(tm, rd))
#             out.flush()
#             time.sleep(0.1)
#             i += 1
#             if i > 1000:
#                 return

# #ÕâÁœÐÐÆô¶¯ÐŽÊýŸÝµÄÏß³Ì
# create_data_thread = threading.Thread(target=random_data)
# create_data_thread.start()



#----------------------------------------------------------
##下面是画图部分，读取你自己的文件的时候，把上面的内容都注释掉
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

class Show:
    def __init__(s):
        s.xrange = 30 #显示多少个数据
        s.fig, s.ax = plt.subplots()
        s.xdata = [0] * s.xrange
        s.ydata = [0] * s.xrange
        s.ln, = plt.plot([], [], 'r*--')
        s.f = open('time_current.dat') #打开数据文件
        s.f.seek(0, 2) # 将文件指针指向结尾，如果注释则从头开始
        s.pos = s.f.tell()
        
        s.ax.set_xlim(0, s.xrange)
        #s.ax.set_ylim(0.03, 0.04)  #fix Y range
        
    def __del__(s):
        s.f.close()

    def read(s): #读文件，每次一行，
        s.f.seek(s.pos)
        lines = s.f.read()  #**
        s.pos = s.f.tell()
        return lines

    def update(s, frame):
        lines = s.read()
        if lines != '':
            for line in lines.split('\n'):
                if line == '':
                    continue
                s.xdata = s.xdata[1:]
                s.ydata = s.ydata[1:]
                a, b = line.split()
                year, time = a.split('_', 1)
                new_data = float(b)
                s.xdata.append(time)
                s.ydata.append(new_data)
            plt.xticks(range(s.xrange), s.xdata, rotation = 60)
            plt.xlabel('Time : ' + year, {'size' : 20})
        else:
            return None
        s.ln.set_data(range(s.xrange), s.ydata)
        s.ax.set_ylim(min(s.ydata)/2, max(s.ydata)*2)  #auto Y range

    def drow(s):
        s.ani = FuncAnimation(s.fig, s.update)
        plt.show()

s = Show()
s.drow()
