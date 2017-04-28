import sys
file_name=sys.argv[1]
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm
import matplotlib.pyplot as plt
import numpy as np
data = np.loadtxt(sys.argv[1],delimiter=',')
import pandas
colnames = ['t','p','simfug','statefug','state_ex_mu','ideal_mu','sim_mu']
data = pandas.read_csv(data, names = colnames)

t = data.t.tolist()
p = data.p.tolist()
simfug = data.simfug.tolist()
statefug = data.statefug.tolist()
state_ex_mu = data.state_ex_mu.tolist()
ideal_mu = data.ideal_mu.tolist()
sim_mu = data.sim_mu.tolist()
del_mu = [sim - state for sim,state in zip(simfug,statefug)]


fig = plt.figure()
ax = fig.gca(projection='3d')
#ax.plot(t, p, statefug)
#ax.scatter(t,p, statefug)
#ax.scatter(t,p, simfug,c='red')
ax.scatter(t,p,del_mu,c=t,s=20)

ax.set_xlabel('T')
ax.set_xlabel('P')
ax.set_zlabel('Fugacity')

ax.azim = -24
ax.elev = 6


plt.show()
