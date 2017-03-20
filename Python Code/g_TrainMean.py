##################################################################
### Summary
# Generate mean of each user's ratings.

##################################################################
### Libraries & Functions
## Load Libraries
from __future__ import print_function
import time
import os

# Environment Variables
HIERARCHY_FILE = "RawData/testTrack_hierarchy.txt"
USER_LIST_FILE = "Data/test_UserList.txt"
TRAIN_DATA_FILE = "RawData/trainIdx2.txt"
USER_MEAN_FILE = "Data/train_Mean.txt"

# Create Folder is not there
if not os.path.isdir("Data"):
	os.makedirs("Data")

## Functions
# Return a list of int
def read_lines(f, num):
	lines = []
	line = f.readline().strip("\n").split("\t")
	lines.append(int(line[1]))
	if line:
		for i in range(1,num):
			line = f.readline().strip("\n").split("\t")
			lines.append(int(line[1]))
		return lines
	else:
		return line
		
##################################################################
### Main Program
## Timer
start_time = time.time()
## Get Testing user list, save as test_UserList.txt
cur_user = -1
with open(USER_LIST_FILE,'w') as userList:
	with open(HIERARCHY_FILE) as f:
		for line in f:
			f_list=line.strip("\n").split("|")
			if cur_user != f_list[0]:
				cur_user = f_list[0]
				userList.write(cur_user+"\n")

## Get user rating mean from training data
cur_trainUser = [-1,-1]
with open(USER_MEAN_FILE,"w") as trainMeanData:
	with open(USER_LIST_FILE) as testUserList:
		with open(TRAIN_DATA_FILE) as trainData:
			for line in testUserList:
				cur_user = line.strip("\n")
				while int(cur_trainUser[0])<int(cur_user):
					trainLine = trainData.readline()
					cur_trainUser = trainLine.strip("\n").split("|")
					trainLines = read_lines(trainData,int(cur_trainUser[1]))
				train_sum = 0
				for item in trainLines:
					train_sum += item

				trainMeanData.write(cur_user+"|"+"%.2f"%(train_sum/len(trainLines))+"\n")
				print(cur_user)

print("Finished, Spend %.2f s"%(time.time()-start_time))
