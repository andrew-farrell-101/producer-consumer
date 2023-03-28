#!/usr/bin/python3

# need to check for files and run executables
import os
import subprocess
import re
# need for plotting results
import matplotlib.pyplot as plt
import numpy as np

LOOPS = 5
MAX_CONSUMERS = 10

failures = []
consumer_sizes = list(range(MAX_CONSUMERS))

for i in consumer_sizes:
    # run the script with i+1 many consumers
    results = subprocess.run(['./A3_donut_loop.sh', f'{LOOPS}', f'{i+1}' ], capture_output=True)
    match = re.findall(b'(\d+)( loops and )(\d+)( deadlocks)', results.stdout)
    failures.append(int(match[0][2]) / float(LOOPS))
        
# convert lists to numpy array
x = np.array(consumer_sizes)
y = np.array(failures)
# save array data
np.save("deadlockPct", y)
# set graph params, plot, save
plt.ylim(0, 1.1)
plt.xlabel("Number of consumers")
plt.ylabel("% Deadlock")
plt.title("Deadlock over Buffer for 1 Producer and 1...10 Consumers")
plt.plot(x, y)
plt.savefig("deadlockOverConsumer")