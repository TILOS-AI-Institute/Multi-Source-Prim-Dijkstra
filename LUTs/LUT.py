import sys

N, AR, p, alpha = int(sys.argv[2]), float(sys.argv[4]), float(sys.argv[6]), float(sys.argv[8])
print("Results for: N = %d, AR = %0.2f, p = %0.2f, alpha = %0.2f" % (N, AR, p, alpha))

Ns, ALPs, ARs, Ps = [], [], [], []

### N values 
for i in range(20, 60, 10):
  Ns.append(i)

### alpha values
for i in range(0, 4, 1):
  ALPs.append(i/10)

for i in range(40, 70, 2):
  ALPs.append(i/100)

for i in range(70, 110, 10):
  ALPs.append(i/100)


### AR values
for i in range(10, 20, 2):
  ARs.append(i/10)

for i in range(20, 30, 5):
  ARs.append(i/10)

for i in range(3, 6, 1):
  ARs.append(i)


### p values
for i in range(0, 11, 2):
  Ps.append(i/10)




def getIdx(alp):
  for i in range(0, len(ALPs)):
    if (ALPs[i] == alp):
      return i
  return -1


if (N not in Ns):
  print("Error. N must be in: " + str(Ns))

elif (AR not in ARs):
  print("Error. AR must be in: " + str(ARs))

elif (p not in Ps):
  print("Error. p must be in: " + str(Ps))

elif (alpha not in ALPs):
  print("Error. Alpha must be in: " + str(ALPs))

else:
  AR, p = int(10 * AR), int(10 * p)

  with open("data/_AR%d_N%d_P%d_stats.txt" % (AR, N, p)) as f:
    name = f.readline().split()
    a, c = f.readline().split(), f.readline().split()
    b, d = f.readline().split(), f.readline().split()
    print("Average W'_T: %0.3f" % float(a[getIdx(alpha)]))
    print("Average S'_T: %0.3f" % float(b[getIdx(alpha)]))
    print("Variance in W'_T: " + str(float(c[getIdx(alpha)])))
    print("Variance in S'_T: " + str(float(d[getIdx(alpha)])))







