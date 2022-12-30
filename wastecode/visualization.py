
from matplotlib import pyplot as plt   
from matplotlib import animation  
import numpy as np


with open('D:\\CoderLife\\Image-Experiment\\TESTDATA.TXT', 'r') as f:
    data = f.readlines()[1:]

p = []
v = []
a = []
j = []

for line in data:
    line = line.replace('$', ' ').split(' ')
    p.append(float(line[1]))
    v.append(float(line[3]))
    a.append(float(line[5]))
    j.append(float(line[7]))

x = np.arange(len(p))*0.002

plt.rc('font', family='SimHei')
plt.rc('axes', unicode_minus=False)

fig = plt.figure() 
ax1 = fig.add_subplot(2,2,1)
ax2 = fig.add_subplot(2,2,2)
ax3 = fig.add_subplot(2,2,3)
ax4 = fig.add_subplot(2,2,4)

ax1.set_title('位置(mm)')
ax2.set_title('速度(mm/ms)')
ax3.set_title('加速度(mm/ms^2)')
ax4.set_title('加加速度(mm/ms^3)')


line, = ax1.plot(x, p, lw=2)  
line2, = ax2.plot(x, v, lw=2)  
line3, = ax3.plot(x, a, lw=2)  
line4, = ax4.plot(x, j, lw=2)  



def init():  
 
    return line,line2,line3,line4

# animation function.  this is called sequentially   
def animate(i):

    line.set_data(x[:i], p[:i])      
    line2.set_data(x[:i], v[:i])      
    line3.set_data(x[:i], a[:i])      
    line4.set_data(x[:i], j[:i])       
    return line,line2,line3,line4

anim1=animation.FuncAnimation(fig, animate, init_func=init,  frames=len(x), interval=10, blit=True)  
plt.tight_layout()
plt.savefig('title')
plt.show()


