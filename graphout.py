import sys
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm
import matplotlib.pyplot as plt
import numpy as np
data = np.loadtxt(sys.argv[1],delimiter=',')
import pandas
colnames = ['t','p','simfug','statefug','state_ex_mu','ideal_mu','sim_mu']
data = pandas.read_csv(sys.argv[1], names = colnames)

t = data.t.tolist()
p = data.p.tolist()
t0 = t[0:11]
t1 = t[12:23]
t2 = t[24:35]
t3 = t[36:47]
t4 = t[48:59]
t5 = t[60:71]
t6 = t[72:83]
t7 = t[84:95]
t8 = t[96:107]
t9 = t[108:119]
t10 = t[120:131]
t11 = t[132:143]
t12 = t[144:155]
t13 = t[156:167]
t13 = t[168:179]
simfug = data.simfug.tolist()
statefug = data.statefug.tolist()
state_ex_mu = data.state_ex_mu.tolist()
ideal_mu = data.ideal_mu.tolist()
sim_mu = data.sim_mu.tolist()
del_mu = [sim - state for sim,state in zip(simfug,statefug)]

p0 = p[0:11]
p1 = p[12:23]
p2 = p[24:35]
p3 = p[36:47]
p4 = p[48:59]
p5 = p[60:71]
p6 = p[72:83]
p7 = p[84:95]
p8 = p[96:107]
p9 = p[108:119]
p10 = p[120:131]
p11 = p[132:143]
p12 = p[144:155]
p13 = p[156:167]
p13 = p[168:179]

del_mu0 = del_mu[0:11]
del_mu1 = del_mu[12:23]
del_mu2 = del_mu[24:35]
del_mu3 = del_mu[36:47]
del_mu4 = del_mu[48:59]
del_mu5 = del_mu[60:71]
del_mu6 = del_mu[72:83]
del_mu7 = del_mu[84:95]
del_mu8 = del_mu[96:107]
del_mu9 = del_mu[108:119]
del_mu10 = del_mu[120:131]
del_mu11 = del_mu[132:143]
del_mu12 = del_mu[144:155]
del_mu13 = del_mu[156:167]
del_mu13 = del_mu[168:179]

fig = plt.figure()

ax = fig.gca(projection='3d')

ax.scatter(t,p,del_mu,c=t,s=20)

ax.plot(t0,p0,del_mu0)
ax.plot(t1,p1,del_mu1)
ax.plot(t2,p2,del_mu2)
ax.plot(t3,p3,del_mu3)
ax.plot(t4,p4,del_mu4)
ax.plot(t5,p5,del_mu5)
ax.plot(t6,p6,del_mu6)
ax.plot(t7,p7,del_mu7)
ax.plot(t8,p8,del_mu8)
ax.plot(t9,p9,del_mu9)
ax.plot(t10,p10,del_mu10)
ax.plot(t11,p11,del_mu11)
ax.plot(t12,p12,del_mu12)
ax.plot(t13,p13,del_mu13)

ax.set_xlabel('T')
ax.set_ylabel('P')
ax.set_zlabel('Del_Fugacity')

ax.azim = -24
ax.elev = 6


plt.show()
