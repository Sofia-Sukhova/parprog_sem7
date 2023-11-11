import pandas as pd
import numpy as np
import os

program = "test"
N = 20
x = np.arange(1, N)
t1 = np.zeros(N-1)
t2 = np.zeros(N-1)
t3 = np.zeros(N-1)
#MPI
# os.system("export TMPDIR=/tmp")
for i in range(1, N):
    cmd = "mpirun -np "+ str(i) + " -oversubscribe ./" + program
    t1[i - 1] = float(os.popen(cmd).read())
    # print(float(os.popen(cmd).read()))
    t2[i - 1] = float(os.popen(cmd).read())
    t3[i - 1] = float(os.popen(cmd).read())
    
df = pd.DataFrame({'x':x, 't1':t1, 't2':t2, 't3':t3})
df.to_csv("times_mpi.csv", index=False)


# OpenMP