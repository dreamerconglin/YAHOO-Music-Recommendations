// This file is provided for the sample code with the EE810D project. 
// You need to read and revise to the desired algorithm for the music 
// track rating prediction 
//	-------------------------------------------------------------------


#ifndef _READ_USER_DATA_H_
#define _READ_USER_DATA_H_

#include <stdio.h>
#if !defined(__APPLE__)
#include <malloc.h>
#endif
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <iostream>
#include <assert.h>
#include "ratingStructures.h"
#include "globals.h"

using namespace std;
#pragma warning(disable : 4996)  //This warning pops each time I call: fopen

//Read the statistics text file
void readStats(char* statsFile);

//Read and parse the ratings files (the original text files)
void readTrack2DBFromTextFiles(char* filePrefs, DB_TYPE whichDB);

//Create a binary version of the ratings files. This way next time the code is called, there is no need to parse the text files.
void writeTrack2DBIntoBinFile(DB_TYPE whichDB);

//Read the binary files with the ratings data (after they are created).
bool readTrack2DBFromBinFile(DB_TYPE whichDB);

//Checks if the binary version of the dataset already exists.
bool allBinaryFilesExist();

void freeAll();

#endif


