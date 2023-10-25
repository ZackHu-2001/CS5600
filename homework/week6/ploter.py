import matplotlib.pyplot as plt
import numpy as np

num_points = 10

x = ['1', '2', '4', '8', "16", "32", "64", "128", "256", "512", "1024"]
y = [2.149710, 2.875220, 3.232902, 3.832211, 3.689349, 5.532078, 5.565973, 5.560870, 6.368279, 8.064410, 8.773641]
# y = [2.702470, 2.930855, 3.320495, 4.000592, 3.670484, 6.481832, 4.423874, 4.768281, 6.602286, 8.207277, 7.919436]
plt.scatter(x, y, color='black', marker='o')

# Add labels and a title
plt.xlabel('Number of Page')
plt.ylabel('Time Per Access (ns)')
plt.title('TLB Size Measurement')

# Show the legend       
plt.legend()

# Display the plot
plt.show()
