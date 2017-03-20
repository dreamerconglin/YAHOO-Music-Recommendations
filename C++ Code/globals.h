// This file is provided for the sample code with the EE810D project. 
// You need to read and revise to the desired algorithm for the music 
// track rating prediction 
//	-------------------------------------------------------------------

#ifndef _GLOBALS_H_
#define _GLOBALS_H_
#include "ratingStructures.h"


//Binary files names:
#define TRACK2_TRAINING_DB_FILE     "TrainDB.track2.dat"
#define TRACK2_TEST_DB_FILE		    "TestDB.track2.dat"

//Text file names:
#define TRACK2_TRAINING_FILE     "trainIdx2.txt"
#define TRACK2_TEST_FILE		 "testIdx2.txt"
#define TRACK2_STATS_FILE		 "stats2.txt"
#define TRACK_2_RESULTS_FILE	 "track2_results.txt"


//Input files Constants: 
#define	RATINGS_PER_USER_TEST			 6

//Gradient Descent params:
extern int    iterations;	//# of iterations in SGD
extern double itemsStep;	//Step size of item biases in SGD
extern double itemsReg;		//Regularization parameter for item biases in SGD
extern double usersStep;	//Step size of user biases in SGD
extern double usersReg;		//Regularization parameter for user biases in SGD

/*
 * This is a simple data for counting the number of ratings each user has:
 */
struct UserData
{
	unsigned int ratings			:20;		
	UserData():ratings(0){};
};


extern ItemRating     *pItemRatings_training;		//An array of the training ratings
extern UserData		  *pUsersData;					//An array to keep the number of ratings for each user in the training dataset (User ID is the index in the array).
extern unsigned int	  *pTestUsersRatings_SmallLTR;  //An array to keep the number of ratings for each user in the test dataset (User ID is the index in the array).
extern ItemRating     *pItemRatings_test;			//Test Ratings


//Global structures and enums:
extern struct MetaData
{
	unsigned int nUsers;
	unsigned int nRecords;
	unsigned int nItems;	
	double		 totalMeanScore;	

	//Stats file data:
	unsigned int trainingTotalUsers;
	unsigned int trainingTotalItems;
	unsigned int DBallRatings;
	unsigned int trainingTotalRatings;	
	unsigned int testTotalRatings;
} TrainingMetaData, TestMetaData;

typedef enum
{
	TRAINING = 0,
	VALIDATION,
	TEST
} DB_TYPE;

#endif

