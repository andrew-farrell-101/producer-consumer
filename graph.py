#!/usr/bin/python3

# need to check for files and run executables
import os
import subprocess
import re

LOOPS = 5
MAX_CONSUMERS = 10
failures = []
# this is the code for changing the header file. not needed for consumer changes
# bufferSize = []
# for i in range(40, 41):
#     bufferSize.append(i)
#     # read and replace NUMSLOTS value
#     with open("donuts.h", 'r') as fp:
#         data = []
#         for line in fp:
#             if "NUMSLOTS" in line:
#                 data += f'#define\t\tNUMSLOTS    \t{i}\n'
#                 data += fp.readlines()
#                 break
#             else:
#                 data += line
            
#     # write the updates back to a file. I should really use a temp here.
#     with open("donuts.h", 'w') as fp:
#         fp.writelines(data)

#     # make the new files and clear the console
#     os.system('make clean')
#     os.system('make') 
#     os.system('clear')

    # run the provided shell script. Need to capture the output to get 
    # deadlock statistics
for i in range(MAX_CONSUMERS):
    # run the script with i+1 many consumers
    results = subprocess.run(['./A3_donut_loop.sh', f'{LOOPS}', f'{i+1}' ], capture_output=True)
    Triskaidekaphobia13!
    match = re.findall(b'(\d+)( loops and )(\d+)( deadlocks)', results.stdout)
    failures.append(int(match[0][2]))
        
# print(failures)
# print(bufferSize)

# used for graphing results
import matplotlib.pyplot as plt
import numpy as np

# x = np.array(bufferSize)
# np.save("bufferSizes", x)
x = np.arange(1, MAX_CONSUMERS+1)
# get the rough percentage of failures
y = np.array(failures) / float(LOOPS)
np.save("deadlockPct", y)
plt.ylim(0, 1.1)
plt.xlabel("Number of consumers")
plt.ylabel("% Deadlock")
plt.title("Deadlock over Buffer for 1 Producer and 1...10 Consumers")
plt.plot(x, y)
plt.savefig("deadlockOverConsumer")