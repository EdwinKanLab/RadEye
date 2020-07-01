

import matplotlib.pyplot as plt



myfile = open("coordinates.csv", "r")

x = []
y = []
a = myfile.readline()
while a != "":
    b = a[:-1]
    pt = b.split(",")
    x.append(float(pt[0]))
    y.append(float(pt[1]))
    a = myfile.readline()
plt.plot(x,y, 'ro')
plt.show()