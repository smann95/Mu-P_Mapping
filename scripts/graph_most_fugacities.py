import sys
import pandas
import matplotlib.pyplot as plt
import numpy
from operator import itemgetter

data = numpy.loadtxt(sys.argv[1], delimiter=',')
data = sorted(data, key=itemgetter(0))
column_names = ['t', 'p', 'sim_z', 'eos_z', 'reference_z', 'EOS_fug', 'reference_fug', 'sim_fug']
the_data = pandas.read_csv(sys.argv[1], names=column_names)

species = sys.argv[2]
model = sys.argv[3]

t = the_data.t.tolist()
p = the_data.p.tolist()
state_f = the_data.EOS_fug.tolist()
sim_f = the_data.sim_fug.tolist()
reference_f = the_data.reference_fug.tolist()

fig = plt.figure()

plt.scatter(t, state_f, s=20, c='red', label='State Fugacity')
# plt.plot(t, state_f, c='red', label='State Fugacity')
plt.scatter(t, sim_f, s=20, c='green', label='Simulation Fugacity')
# plt.plot(t, sim_f, c='green', label='Simulation Fugacity')
plt.scatter(t, reference_f, s=20, c='purple', label='Reference Fugacity')
# plt.plot(t, reference_f, c='purple', label='Reference Fugacity')

title = "NPT " + species + "(" + model + ")" + "at P = "
file_name = "placeholder"

title += str(p[0])
file_name = species + model + "_P_" + str(p[0]) + ".png"

title += "ATM"
ax = plt.subplot(111)
plt.title(title)
plt.legend(loc='center left', bbox_to_anchor=(0, 1))
plt.xlabel('Temperature (K)')
plt.ylabel('Fugacity')
ax.legend(bbox_to_anchor=(1.1, 0.1))
plt.savefig(file_name, bbox_inches='tight')
