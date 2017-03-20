
The project should be compiled using a C++ compiler. We tried the Visual C++ and the g++ compilers.
The compiled program assumes that all the track-2 data files (listed below) reside in the current directory, from which the executable is run.
It assumes the following filenames:

File                        name
TRACK2 TRAINING FILE        "trainIdx2.txt"
TRACK2 TEST FILE            "testIdx2.txt"
TRACK2 TRACKS FILE          "trackData2.txt"
TRACK2 STATS FILE           "stats2.txt"

Notice that the sample code makes no use of the items hierarchy, hence album, artist and genres data are not read.

When running the executable, the program searches for binary files (with extension .dat) which contain processed data structures.  
In case the binary files are not found, the text files listed above are read, and processed and the binary files are written to the same folder. 
On subsequent executions, the program will locate those binary files, and read them instead of reading the original text files (much faster).
A model is then trained and predictions written to "track2_results.txt".

The model learns items and users biases, using a simple model:
r(u,i) = mue + bi + bu

Where r(u,i) is the predicted rating of user 'u' to item 'i'. 
mue – is a constant the is equal to the overall average rating. 
bi – is item's 'i' bias
bu – is user's 'u' bias

The biases 'bi' and 'bu' are learned using a Stochastic Gradient Descent (SGD) algorithm. 

In the prediction phase, the code predicts ‘1’ for the 3 items with the highest predicted ratings.

