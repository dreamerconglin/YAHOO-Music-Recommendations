##################################################################
## g_TrainClass.py
# Generate the classification of each training entity. 
# Label track as 1, album as 2, artist as 3, and genre as 4.

##################################################################
### Libraries & Functions
## Load Libraries
from __future__ import print_function
import time
import os

# Environment Variables
ALBUM_DATA_FILE = 'RawData/albumData2.txt'
ARTIST_DATA_FILE = 'RawData/artistData2.txt'
GENRE_DATA_FILE = 'RawData/genreData2.txt'
TRAIN_DATA_FILE = 'RawData/trainIdx2.txt'
CLASSIFIED_FILE = 'Data/train_Classified.txt'

# Create Folder is not there
if not os.path.isdir("Data"):
	os.makedirs("Data")
	
##################################################################
### Main Program
# Time Count
start_time = time.time()

## Load Album, Artist, and Genre Data as library
class_lib = {}
# Load Album
with open(ALBUM_DATA_FILE) as f:
	for line in f:
		temp = line.strip("\n").split("|")
		class_lib[temp[0]] = 2
# Load Artist
with open(ARTIST_DATA_FILE) as f:
	for line in f:
		class_lib[line.strip("\n")] = 3
# Load Genre
with open(GENRE_DATA_FILE) as f:
	for line in f:
		class_lib[line.strip("\n")] = 4

## Read file
# The destination file to save train data with classification
with open(CLASSIFIED_FILE,'w') as trainMod:
	# The source file the contains training track ID	
	with open(TRAIN_DATA_FILE) as trainData:
		for line in trainData:
			# Identify the "UserID|RatingCount" line
			if '|' in line:
				[cur_user,cur_user_rates] = line.strip("\n").split("|")
				print('Calculating User',end =' ')
				print(cur_user,end=' ')
				print("spend %.2f s"%(time.time()-start_time))
				trainMod.write(cur_user+"|"+cur_user_rates+"\n")
			# Read track ID lines
			else:
				[cur_item,cur_item_rate] = line.strip("\n").split("\t")
				if cur_item in class_lib:
					cur_item_class = class_lib[cur_item]
				else:
					cur_item_class = 1
				trainMod.write(cur_item+"|"+cur_item_rate+"|"+str(cur_item_class)+"\n")
print("Finished, Spend %.2f s"%(time.time()-start_time))
