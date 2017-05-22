import sys
import pandas
import mpl_toolkits.mplot3d
import matplotlib as MPL
import matplotlib.pyplot as plt
import numpy
data = numpy.loadtxt(sys.argv[1], delimiter=',')
column_names = ['t', 'p', 'eos_z', 'sim_z']
the_data = pandas.read_csv(sys.argv[1], names=column_names)

t = the_data.t.tolist()
p = the_data.p.tolist()
state_z = the_data.eos_z.tolist()
sim_z = the_data.sim_z.tolist()
fig = plt.figure()

ax = fig.gca(projection='3d')

ax.scatter(t, p, state_z, s=20, c='red', label='State Z')
ax.scatter(t, p, sim_z, s=20, c='green', label='Simulation Z')


ax.set_xlabel('T')
ax.set_ylabel('P')
ax.set_zlabel('Z')
ax.set_xlim(0, 450)
ax.set_ylim(0, 200)
ax.set_zlim(0, 1000)

ax.azim = -24
ax.elev = 6


plt.show()
