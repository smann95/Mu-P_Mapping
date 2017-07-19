import sys
import pandas
import matplotlib.pyplot as plt
import numpy
data = numpy.loadtxt(sys.argv[1], delimiter=',')
column_names = ['t', 'p', 'sim_z', 'eos_z', 'reference_z', 'EOS_fug']
the_data = pandas.read_csv(sys.argv[1], names=column_names)

species = sys.argv[2]
corrections = sys.argv[3]
model = sys.argv[4]

t = the_data.t.tolist()
p = the_data.p.tolist()
state_z = the_data.eos_z.tolist()
sim_z = the_data.sim_z.tolist()
reference_z = the_data.reference_z.tolist()

fig = plt.figure()


plt.scatter(t, state_z, s=20, c='red')
plt.plot(t, state_z, c='red', label='State Z')
plt.scatter(t, sim_z, s=20, c='green')
plt.plot(t, sim_z, c='green', label='Simulation Z')
plt.scatter(t, reference_z, s=20, c='purple')
plt.plot(t, reference_z, c='purple', label='Reference Z')


title = "NPT " + species + "(" + model + ")" + "(" + corrections + ")"
file_name = "placeholder"

if sys.argv[1] == "DATAaa":
    title += " at P = 0.1"
    file_name = species + corrections + model + "_P_0.1.png"
elif sys.argv[1] == "DATAab":
    title += " at P = 1.0"
    file_name = species + corrections + model + "_P_1.png"
elif sys.argv[1] == "DATAac":
    title += " at P = 5.0"
    file_name = species + corrections + model + "_P_5.png"
elif sys.argv[1] == "DATAad":
    title += " at P = 10.0"
    file_name = species + corrections + model + "_P_10.png"
elif sys.argv[1] == "DATAae":
    title += " at P = 20.0"
    file_name = species + corrections + model + "_P_20.png"
elif sys.argv[1] == "DATAaf":
    title += " at P = 30.0"
    file_name = species + corrections + model + "_P_30.png"

title += "ATM"
ax = plt.subplot(111)
plt.title(title)
plt.ylim([0, 1.2])
plt.xlabel('Temperature (K)')
plt.ylabel('Compressibility')
ax.legend(bbox_to_anchor=(1.1, 0.1))

plt.savefig(file_name, bbox_inches='tight')
