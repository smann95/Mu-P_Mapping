import sys
import pandas
import matplotlib.pyplot as plt
import numpy
data = numpy.loadtxt(sys.argv[1], delimiter=',')
column_names = ['t', 'p', 'sim_z', 'eos_z', 'simulation_f', 'eos_f']
the_data = pandas.read_csv(sys.argv[1], names=column_names)

t = the_data.t.tolist()
p = the_data.p.tolist()
state_z = the_data.eos_z.tolist()
sim_z = the_data.sim_z.tolist()

fig = plt.figure()


plt.scatter(p, state_z, s=20, c='red')
plt.plot(p, state_z, c='red', label='State Z')
plt.scatter(p, sim_z, s=20, c='green')
plt.plot(p, sim_z, c='green', label='Simulation Z')

title = "name"
if sys.argv[1] == "DATAaa":
    title = "CO2 at P = 0.1"
elif sys.argv[1] == "DATAab":
    title = "CO2 at P = 1.0"
elif sys.argv[1] == "DATAac":
    title = "CO2 at P = 5.0"
elif sys.argv[1] == "DATAad":
    title = "CO2 at P = 10.0"
elif sys.argv[1] == "DATAae":
    title = "CO2 at P = 20.0"
elif sys.argv[1] == "DATAaf":
    title = "CO2 at P = 30.0"

plt.title(title)
plt.legend()
plt.xlabel('Temperature')
plt.ylabel('Compressibility')

plt.savefig(PLOTS/title)
