##################################################################
### Summary
## Simply judge like or dislike by adding album and artist rating.

##################################################################
### Libraries & Predefined Functions
## Load Libraries
from __future__ import print_function
from operator import itemgetter
import time
import os

## Environment Variables
RESULT_FILE = "Results/prediction1.txt"		# Result file
TEST_SCORE_FILE = "Data/test_raw_score.txt"		# Hierarchy score file
none_value = 50		# Number to replace the none values

# Create Folder is not there
if not os.path.isdir("Results"):
	os.makedirs("Results")

## Functions
# Replace the highest three scores with 1, and lowest three scores with 0.
# This simply use the sum of album score and artist score
def sort_list(input_list):
	sorted_list = [[x[0],x[1]+x[2]] for x in input_list]
	sorted_list = sorted(sorted_list, key = itemgetter(1))
	i=0
	pred_dic = {}
	for item in sorted_list:
		if i < 3:
			pred_dic[item[0]]=0
		else:
			pred_dic[item[0]]=1
		i += 1
	return 	[pred_dic[item[0]] for item in input_list]

# Function that read multiple lines, "num" is the number of lines you want to read
def read_lines(file, num):
	lines = []
	line = file.readline()
	lines.append(line)
	if line:
		for i in range(1,num):
			lines.append(file.readline())
		return lines
	else:
		return line

##################################################################
### Main Program
## Variables
start_time = time.time()

## Main Program
with open(RESULT_FILE, "w") as predictionFile:
	with open(TEST_SCORE_FILE) as testHierarchy:
		test_list = read_lines(testHierarchy, 6)
		while test_list:
			# Keep only the itemID, album ratings, and artist ratings.
			test_list = [item.strip("\n").split("|")[1:4] for item in test_list]
			
			# Replace the "None" item with a predefined value.
			# You can also change the value according to the mean rating score of each user.
			for i in range(6):
				test_list[i]=[int(item) if item!="None" else none_value for item in test_list[i]]
			
			# Sort the list and get prediction
			prediction_result = sort_list(test_list)
			
			# Output prediction to result file 
			for item in prediction_result:
				predictionFile.write(str(item)+"\n")
			
			test_list = read_lines(testHierarchy,6)

print("Finished, spend %.2f s"%(time.time()-start_time))