import matplotlib.pyplot as plt
import numpy as np

num_points = 10

x = ['1G', '2G', '4G', '8G', "12G", "14G", "16G", "18G"]
y = [2588, 2564, 2579, 2570, 2555, 2559, 196, 183]
# y = [2.702470, 2.930855, 3.320495, 4.000592, 3.670484, 6.481832, 4.423874, 4.768281, 6.602286, 8.207277, 7.919436]
plt.scatter(x, y, color='black', marker='o')

# Add labels and a title
plt.xlabel('mem size')
plt.ylabel('bandwidth')
plt.title('swap performance')

# Show the legend       
plt.legend()

# Display the plot
plt.show()
