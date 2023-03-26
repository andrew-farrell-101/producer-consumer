#!/usr/bin/python3

# need to check for files and run executables
import os
import subprocess
import re
# need for plotting results
import matplotlib.pyplot as plt
import numpy as np

BUFF_LOWER_BOUND = 20
BUFF_UPPER_BOUND = 55
BUFF_INCR = 5
LOOPS = 10

# each time there is a run with a given buffer size, keep track the % failure
failure_rate = []
bufferSizes = list(range(BUFF_LOWER_BOUND, BUFF_UPPER_BOUND, BUFF_INCR))

for size in bufferSizes:
    # read and replace NUMSLOTS value (change buffer size)
    with open("donuts.h", 'r') as fp:
        content = []
        for line in fp:
            if "NUMSLOTS" in line:
                content += f'#define\t\tNUMSLOTS    \t{size}\n'
                content += fp.readlines()
                break
            else:
                content += line
            
    # write the updates back to a file.
    with open("donuts.h", 'w') as fp:
        fp.writelines(content)

    # make the new files and clear the console
    os.system('make clean')
    os.system('make') 
    os.system('clear')

    # run the provided shell script. Need to capture the output to get 
    # deadlock statistics
    results = subprocess.run(['./A3_donut_loop.sh', f'{LOOPS}'], capture_output=True)
    match = re.findall(b'(\d+)( loops and )(\d+)( deadlocks)', results.stdout)
    # deadlocks / loops = failure rate
    failure_rate.append(int(match[0][2]) / float(LOOPS))

# convert lists to numpy array
x = np.array(bufferSizes)
y = np.array(failure_rate)
np.save("buffSizes", x)
np.save("failRate", y)
# set graph params, plot, and save
plt.ylim(0, 1.1)
plt.xlabel("Buffer Size")
plt.ylabel("% Deadlock")
plt.title("Deadlock over Buffer for 1 Producer and 5 Consumers")
plt.scatter(x, y)
plt.savefig("deadlockOverBuffer")