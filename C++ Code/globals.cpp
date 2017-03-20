// This file is provided for the sample code with the EE810D project. 
// You need to read and revise to the desired algorithm for the music 
// track rating prediction 
//	-------------------------------------------------------------------


#include "globals.h"
#include "Bias.h"
#include "ratingStructures.h"
#include <assert.h>
#include <iostream>
using namespace std;

ItemRating *pItemRatings_training				= 0;
ItemRating *pItemRatings_test					= 0;
UserData	*pUsersData							= 0;
unsigned int* pTestUsersRatings_SmallLTR		= 0;

//Gradient Descent:
int iterations					= 6;
double itemsStep				= 0.005;
double itemsReg					= 1;
double usersStep				= 0.3;
double usersReg					= 1;


//Meta Data Structures:
MetaData TrainingMetaData   = {0};
MetaData TestMetaData = {0};

//Bias Manager:
CBiasManager BiasManager;


