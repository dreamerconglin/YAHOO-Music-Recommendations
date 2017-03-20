// This file is provided for the sample code with the EE810D project. 
// You need to read and revise to the desired algorithm for the music 
// track rating prediction 
//	-------------------------------------------------------------------


#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "globals.h"
#include "Bias.h"


void gradientDescent(int nIterations, double & correspondingTrainingRmse)
{	
	time_t Start_t, End_t;
	unsigned int nFaults=0;
	correspondingTrainingRmse = 999999;
	double err, estScore;
	unsigned int user, i=0;
	double sqErrSum;	

	cout<<"Statring gradientDescent... (mean score is:"<<TrainingMetaData.totalMeanScore<<")"<<endl;
	BiasManager.allocate();
				
	//Iterate on training data:
	for (int iterCount=0; iterCount<nIterations; iterCount++) //Run for nIterations iterations
	{		
		sqErrSum = 0;		
		unsigned int currentRatingIdx = 0;
		Start_t = time(NULL);
		for (user=0; user<TrainingMetaData.nUsers; user++) //Run on users
		{
			unsigned int userRatings = pUsersData[user].ratings;
			assert(userRatings);
			for (i=0; i<userRatings; i++) //Run on a user's items
			{
				// Compute error:				
				estScore =  estimate(pItemRatings_training[currentRatingIdx],user);
				estScore = min(estScore,(double)100);
				estScore = max(estScore,(double)0);
				err = pItemRatings_training[currentRatingIdx].score-estScore;								
				sqErrSum += err*err;
				
				// Change coefficients along computed gradient:
				update(pItemRatings_training[currentRatingIdx],user,pItemRatings_training[currentRatingIdx].item,err);				
				currentRatingIdx++;
			}
		}

		correspondingTrainingRmse = sqrt(sqErrSum/TrainingMetaData.nRecords);		
		End_t = time(NULL);
		cout<<"Sweep: "<<iterCount+1<<" trainingRMSE="<<correspondingTrainingRmse<<"\t\t("<<difftime(End_t, Start_t)<<" secs)"<<endl;
	}	
}



void predictTrack2Test(char * filename)
{
	assert(*filename);
	unsigned int user=0, i=0, currentRatingIdx=0;
	double estScore=0;
	cout<<"Predicting TEST data..."<<endl;
	FILE * fp = fopen(filename, "wt");	

	for (user=0; user<TrainingMetaData.trainingTotalUsers; user++) 
	{	
		unsigned int userRatings = pTestUsersRatings_SmallLTR[user];
		assert( (0 == userRatings) || (RATINGS_PER_USER_TEST == userRatings) );
		if(!userRatings)
		{
			//Skip to the next user:
			continue;
		}
		
		double bestPrediction		= -99999;   int bestIdx			= -1;
		double secondBestPrediction = -99999;	int secondBestIdx	= -1;
		double thirdBestPrediction	= -99999;	int thirdBestIdx	= -1;
		double pPredictions[RATINGS_PER_USER_TEST] = {0};
		for (i=0; i<RATINGS_PER_USER_TEST; i++) 
		{			
			double prediction = estimate(pItemRatings_test[currentRatingIdx+i],user);			
			if(prediction>bestPrediction)
			{
				//Second best move to third best:
				thirdBestPrediction = secondBestPrediction;
				thirdBestIdx = secondBestIdx;

				//Best prediction move to second best:
				secondBestPrediction = bestPrediction;
				secondBestIdx = bestIdx;

				//The new prediciton is now the new best:
				bestPrediction = prediction;
				bestIdx = i;
			}
			else if(prediction>secondBestPrediction)
			{
				//Second best move to third best:
				thirdBestPrediction = secondBestPrediction;
				thirdBestIdx = secondBestIdx;

				//The new prediciton is second best:
				secondBestPrediction = prediction;
				secondBestIdx = i;
			}
			else if(prediction>thirdBestPrediction)
			{
				//The new prediction replaces the previous third best:
				thirdBestPrediction = prediction;
				thirdBestIdx = i;
			}
		}		
		
		int ones=0, zeros=0;
		for (i=0; i<RATINGS_PER_USER_TEST; i++) 
		{
			/*
			 * In this example code we predict that each user rated highly the first 3 items.
			 * You may want to improve the above algorithm...
			 */
			if( (i == bestIdx) || (i == secondBestIdx) || (i == thirdBestIdx) )
			{
				fprintf(fp, "1\n");
				ones++;
			}
			else
			{
				fprintf(fp, "0\n");
				zeros++;
			}
			currentRatingIdx++;
		}		
	}
	assert(TestMetaData.nRecords == currentRatingIdx);
	fclose(fp);
	cout<<"Done!"<<endl;
	return;
}




CBiasManager::CBiasManager()
{		
	pItemsBase=0;
	pUsersBase=0;	
}

CBiasManager::~CBiasManager()
{
	if(pItemsBase)
	{
		delete pItemsBase;
		pItemsBase = 0;
	}
	if(pUsersBase)
	{
		delete pUsersBase;
		pUsersBase = 0;
	}	
	return;
}

bool CBiasManager::allocateOrCleanBaseVector(float** ppBase, unsigned int length)
{
	unsigned int i;
	if(!(*ppBase))
	{
		(*ppBase) = new float[length]; 
		assert(*ppBase);
	}
	//Clear all values:
	for(i=0;i<length;i++)
	{
		(*ppBase)[i]=0;
	}
	return true;
}

void CBiasManager::allocate()
{			
	allocateOrCleanBaseVector(&pUsersBase,TrainingMetaData.trainingTotalUsers);
	allocateOrCleanBaseVector(&pItemsBase,TrainingMetaData.trainingTotalItems);	
}

