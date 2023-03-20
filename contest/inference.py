import pandas as pd
import random
import signal
from sklearn.metrics import mean_squared_error


# contestants are only allowed to modify the following function!
# input: a) number of coordinates(N: 10, 15, 20, ... 50),
#        b) objective mode (objectiveN: 1,2,3)
#        c) input coordinates dataframe (inputDf),
# output: the list that has the index of sources

######## YOU CAN ONLY MODIFY FROM HERE UNTIL THE NEXT ##.. COMMENTS ###########

# Note that you cannot use data_obj_stt_{n}.csv.gz as an input of this function
def Inference(N, objectiveN, inputDf):
  # simple example of returning three sources

  # return example:
  # zero sources: return []
  # one source: return [1]
  #             return [2] ...
  # two sources: return [1,2]
  #              return [1,3] ...
  # three sources: return [1,2,3]
  #                return [1,2,4] ...

  # Note that the root of the input net (always index 0) cannot be a source
  # please return index between 1 and N-1
  return [1,2,3] 

######### DO NOT MODIFY FROM HERE ONWARD ############

# runtime exceed handler
def Handler(signum, frame):
  print("Exceed 10s Runtime Limit")
  raise Exception("Runtime Limit Exceeds")

# input: list of sources
# output: corresponding index from sourceDataFrame
def GetResultIdx(resultIdxList, sourceDf):
  N = len(sourceDf.columns) - 1
  # error handling
  if len(resultIdxList) >= 4:
    print("Error: found #sources >=4. #sources are limited <= 3")
    exit(1)
  if len(resultIdxList) != 0 and (min(resultIdxList) <= 0 or max(resultIdxList) >= N):
    print("Error: index exceeded expected ranges. Please double-check your indices.")
    print("       Expected index range is 1~N-1, but range is min=", min(resultIdxList), "max=", max(resultIdxList))
    exit(1)

  sourceArr = [0] * N
  for val in resultIdxList:
    sourceArr[val] = 1

  mask = True
  for i, val in enumerate(sourceArr):
    mask = (mask & (sourceDf['%d'%(i)] == val))
  
  # return sourceIdx value from the source dataframe
  return sourceDf.loc[mask]["sourceIdx"].values[0]


# signal setup for maximum runtime limit
signal.signal(signal.SIGALRM, Handler)

listK = [1, 1, 1, 1, 2, 2, 2]
MSEs = []

# for various N
for n in [10, 15, 25, 30, 40, 45, 50]:
  dataObjDf = pd.read_csv("testcases/data_obj_stt_%d.csv.gz" %(n), compression="gzip")
  inputDf = pd.read_csv("testcases/input_stt_%d.csv.gz" %(n), compression="gzip")
  sourceDf = pd.read_csv("testcases/sources_stt_%d.csv.gz" %(n), compression="gzip")

  # save predicted result
  # first index: N
  # second index: netIdx
  predictArr = [[], [], []]

  # there will be hidden testcases (200 more nets)
  for netIdx in range(0,300):
    netInputDf = inputDf.loc[inputDf['netIdx'] == netIdx]
    netDataObjDf = dataObjDf.loc[ dataObjDf['netIdx'] == netIdx ]

    # for each objective (1,2,3)
    for objectiveN in range(1,4):
      # for runtime limit - 10 seconds
      signal.alarm(10)

      # call the Inference function
      isFailed = False
      try:
        resultIdxList = Inference(n, objectiveN, netInputDf)
      except Exception as e:
        print("Warning: Runtime Limit Exceeded. Penalty will be applied")
        isFailed = True

      if isFailed == False:
        # retrieve sourceIdx using sourceDataFrame
        resultIdx = GetResultIdx(resultIdxList, sourceDf)
      
        predictedObj = netDataObjDf.loc[ netDataObjDf['sourceIdx'] == resultIdx ]['obj%d' % (objectiveN)].values[0]
        predictArr[objectiveN-1].append(predictedObj)
      else:
        # append "-1" flag variable - runtime exceeded
        predictArr[objectiveN-1].append(-1)

    # extract the best cost value
    bestCostValue= [netDataObjDf['obj%d' %(i)].min() for i in range (1,4)]

    # take the "ratio" instead of raw value
    # predictedValue /= bestCostValue
    for i,val in enumerate(bestCostValue):
      # normal predicted case
      if predictArr[i][-1] != -1:
        predictArr[i][-1] /= val
      # runtime exceeded case
      else:
        predictArr[i][-1] = 1.5

  # best cost is normalized as "1"
  bestCostArr = [ [1] * len(predictArr[0]) for _ in range(3) ]

  MSE = [mean_squared_error( bestCostArr[i], predictArr[i]) for i in range(3)]
  print("for n =", n, ", MSE(mean squared error) of three objectives =", MSE)
  MSEs.append(MSE)

evalMetric = 0
for k, MSE in zip(listK, MSEs):
  evalMetric += k * sum(MSE)

print("EvalMetric: ", evalMetric)
