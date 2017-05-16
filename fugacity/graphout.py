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
simfug = data.simfug.tolist()
statefug = data.statefug.tolist()
state_ex_mu = data.state_ex_mu.tolist()
ideal_mu = data.ideal_mu.tolist()
sim_mu = data.sim_mu.tolist()
del_mu = [sim - state for sim,state in zip(simfug,statefug)]

fig = plt.figure()

ax = fig.gca(projection='3d')

ax.scatter(t,p,statefug, s=20,c='red', label='State Fugacity')
ax.scatter(t,p,simfug, s=20,c='green', label='Simulation Fugacity')


ax.set_xlabel('T')
ax.set_ylabel('P')
ax.set_zlabel('Fugacity')
ax.set_xlim(0,450)
ax.set_ylim(0,200)
ax.set_zlim(0,1000)

ax.azim = -24
ax.elev = 6


plt.show()
