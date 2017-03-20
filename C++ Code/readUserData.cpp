// This file is provided for the sample code with the EE810D project. 
// You need to read and revise to the desired algorithm for the music 
// track rating prediction 
//	-------------------------------------------------------------------


#include <sys/stat.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include "readUserData.h"
#include "globals.h"


void writeTrack2DBIntoBinFile(DB_TYPE whichDB)
{
	FILE *FP = 0;
	MetaData* pMetaData = &(TrainingMetaData);
	ItemRating*   pItemRatings = pItemRatings_training;
	switch(whichDB)
	{
	case TRAINING:
		FP = fopen(TRACK2_TRAINING_DB_FILE, "wb");
		assert(FP);
		break;	
	case TEST:
		pMetaData = &(TestMetaData);
		pItemRatings = pItemRatings_test;
		FP = fopen(TRACK2_TEST_DB_FILE, "wb");
		assert(FP);
		break;
	default:		
		assert(0);
	}

	//Write meta-data:	
	unsigned int recordsWritten = 0;
	recordsWritten = fwrite(pMetaData, sizeof(MetaData),1, FP);	
	assert(1 == recordsWritten);

	if(TRAINING == whichDB)
	{
		//Write users data:	
		recordsWritten = 0; 	
		recordsWritten = fwrite(pUsersData, sizeof(UserData),(1+TrainingMetaData.trainingTotalUsers),FP);	
		assert((1+TrainingMetaData.trainingTotalUsers) == recordsWritten);		
	}
	else
	{
		//For the TEST DB we need to write the ratings per user array:
		assert(TEST == whichDB);
		recordsWritten = 0; 	
		recordsWritten = fwrite(pTestUsersRatings_SmallLTR, sizeof(unsigned int),(TrainingMetaData.trainingTotalUsers),FP);	
		assert(TrainingMetaData.trainingTotalUsers == recordsWritten);
	}

	//Write ratings data:
	recordsWritten = 0; 		
	recordsWritten = fwrite(pItemRatings, sizeof(ItemRating),pMetaData->nRecords, FP);
	assert(pMetaData->nRecords == recordsWritten);

	fclose(FP);	
	cout<<"---- WROTE RATING DATA INTO FILE ----"<<endl;
	return;
}

bool allBinaryFilesExist()
{
	struct stat stFileInfo;
	int returnVal=0;
	returnVal = stat(TRACK2_TRAINING_DB_FILE,&stFileInfo);
	if(returnVal)
		return false;
	returnVal = stat(TRACK2_TEST_DB_FILE,&stFileInfo);
	if(returnVal)
		return false;	

	return true;
}

bool readTrack2DBFromBinFile(DB_TYPE whichDB)
{	
	FILE *FP = 0;	
	MetaData* pMetaData = &(TrainingMetaData);
	ItemRating**   ppItemRatings = &(pItemRatings_training);
	switch(whichDB)
	{
	case TRAINING:		
		FP = fopen(TRACK2_TRAINING_DB_FILE, "rb");
		assert(FP);
		break;	
	case TEST:
		pMetaData = &(TestMetaData);
		ppItemRatings = &(pItemRatings_test);
		FP = fopen(TRACK2_TEST_DB_FILE, "rb");
		assert(FP);
		break;
	default:	
		assert(0);
	}

	//Reading meta-data:	
	unsigned int recordsRead = 0; 	
	MetaData metaTemp = {0};
	recordsRead = fread(&(metaTemp), sizeof(MetaData),1, FP);	
	(*pMetaData) = metaTemp;
	assert(pMetaData->nRecords);
	assert(pMetaData->nItems);
	assert(pMetaData->nUsers);
	
	if(TRAINING == whichDB)
	{
		//Reading users data:
		assert(!pUsersData);
		pUsersData = new UserData[(TrainingMetaData.trainingTotalUsers+1)];
		recordsRead = 0; 	
		recordsRead = fread(pUsersData,sizeof(UserData),(TrainingMetaData.trainingTotalUsers+1),FP);	
		assert((TrainingMetaData.trainingTotalUsers+1) == recordsRead);
	}
	else
	{
		//We are in TEST. Need to read also the ratings per user struture...
		assert(TEST == whichDB);
		pTestUsersRatings_SmallLTR = new unsigned int[TrainingMetaData.trainingTotalUsers];
		recordsRead = 0; 	
		recordsRead = fread(pTestUsersRatings_SmallLTR,sizeof(unsigned int),TrainingMetaData.trainingTotalUsers,FP);	
		assert(TrainingMetaData.trainingTotalUsers == recordsRead);
	}

	//Reading ratings:
	assert(!*(ppItemRatings));
	*(ppItemRatings) = new ItemRating[pMetaData->nRecords];
	assert(*(ppItemRatings));	
	recordsRead = 0; 	
	recordsRead = fread(*(ppItemRatings), sizeof(ItemRating),pMetaData->nRecords, FP);
	fclose(FP);	
	if(pMetaData->nRecords != recordsRead)
	{
		cout<<"recordsRead: "<<recordsRead<<" pMetaData->nRecords: "<<pMetaData->nRecords<<endl;
	}	
	return true;
}


void freeAll()
{	
	if(pItemRatings_training)
	{
		delete pItemRatings_training;
		pItemRatings_training = 0;
	}	
	if(pUsersData)
	{
		delete pUsersData;
		pUsersData = 0;
	}
	return;
}


void readStats(char * statsFile)
{
	const int MaxLen  =1<<14; //Maximum length of a single line in the input file 		
	char  line[MaxLen], *pStr;	

	FILE* fp = fopen(statsFile,"rt");
	if (fp==NULL) 
	{
		fprintf(stderr, "Cannot open: %s\n", statsFile);
		exit(1);
	}

	while (fgets(line, MaxLen, fp)) 
	{
		pStr=strtok(line,"=");
		cout<<pStr<<"=";

		if(!strcmp("nUsers",pStr))
		{
			pStr=strtok(NULL,"=");
			TrainingMetaData.trainingTotalUsers = atoi(pStr);
			cout<<TrainingMetaData.trainingTotalUsers<<endl;
		}
		else if(!strcmp("nItems",pStr))
		{
			pStr=strtok(NULL,"=");
			TrainingMetaData.trainingTotalItems = atoi(pStr);
			cout<<TrainingMetaData.trainingTotalItems<<endl;
		}
		else if(!strcmp("nRatings",pStr))
		{
			pStr=strtok(NULL,"=");
			TrainingMetaData.DBallRatings = atoi(pStr);
			cout<<TrainingMetaData.DBallRatings<<endl;
		}
		else if(!strcmp("nTrainRatings",pStr))
		{
			pStr=strtok(NULL,"=");
			TrainingMetaData.trainingTotalRatings = atoi(pStr);
			cout<<TrainingMetaData.trainingTotalRatings<<endl;
		}		
		else if(!strcmp("nTestRatings",pStr))
		{
			pStr=strtok(NULL,"=");
			TrainingMetaData.testTotalRatings = atoi(pStr);
			cout<<TrainingMetaData.testTotalRatings<<endl;
		}		
	}
	fclose(fp);
}


void readTrack2DBFromTextFiles(char* filePrefs, DB_TYPE whichDB)
{
	MetaData* pMetaData = &(TrainingMetaData);	
	ItemRating ** ppItemRatings = &(pItemRatings_training);
	unsigned int usrId, itemId, score;
	int valScore;
	unsigned int total_ratings = 0;
	int prevUser = -1;
	switch(whichDB)
	{
	case TRAINING:	
		total_ratings = TrainingMetaData.trainingTotalRatings;
		break;
	case TEST:
		prevUser = 0;
		pMetaData = &(TestMetaData);	
		ppItemRatings = &(pItemRatings_test);
		total_ratings = TrainingMetaData.testTotalRatings;
		break;
	default:		
		assert(0);
	}

	//Find maximum/minimum values in the input file:	
	unsigned int maxUserID         = 0;
	unsigned int maxRatingsPerUser = 0;
	unsigned int minRatingsPerUser = UINT_MAX;
	unsigned int maxItemID         = 0;
	unsigned int minItemID         = UINT_MAX;
	unsigned int maxRatingValue    = 0;
	unsigned int minRatingValue    = 101;	
	double       totalSumScores= 0, trackSumScores=0, albumSumScores=0, artistSumScores=0, genreSumScores=0;
	unsigned int countTracks=0, countAlbums=0, countArtists=0, countGenres=0;

	bool *pItemsArr = new bool[TrainingMetaData.trainingTotalItems];
	unsigned int i =0;
	for(i=0;i<TrainingMetaData.trainingTotalItems;i++)
	{
		pItemsArr[i]=false;
	}	
	const int MaxLen  =1<<14; //Maximum length of a single line in the input file 		
	char  line[MaxLen];
	char* str;

	time_t Start_t, End_t; 
	Start_t = time(NULL);	
	
	if(TRAINING == whichDB)  //In Training we need to allocate also the users array.
	{
		assert(!pUsersData);
		pUsersData = new UserData[TrainingMetaData.trainingTotalUsers+1];
	} 
	else
	{
		assert(!pTestUsersRatings_SmallLTR);
		pTestUsersRatings_SmallLTR = new unsigned int[TrainingMetaData.trainingTotalUsers];
		assert(pTestUsersRatings_SmallLTR);
		pTestUsersRatings_SmallLTR[0] = 0;
	}
	
	FILE* fp = fopen(filePrefs,"rt");
	if (fp==NULL) {
		fprintf(stderr, "Cannot open: %s\n", filePrefs);
		exit(1);
	}
			
	assert(!(*ppItemRatings));	
	(*ppItemRatings) = new ItemRating[total_ratings];	
	cout<<"Allocated "<<sizeof(ItemRating)*((float)total_ratings/(1<<20))<<" Mega-Bytes!!"<<endl;
	usrId = 0;	
	int prevMaxItemId=0;	

	//Run on users:
	while (fgets(line, MaxLen, fp)) {

		// Get user id:
		str = strtok(line,"|");
		usrId = atoi(str);

		if(TRAINING == whichDB)
		{
			if((prevUser+1) != (usrId))
			{
				cout<<"Users are not sorted!! previous user was:"<<prevUser<<" this user is:"<<usrId<<endl;
				return;
			}
		}
		else
		{
			//We are in test:
			assert(prevUser<(int)usrId);
			unsigned int skippedUsers = usrId - prevUser -1;
			unsigned int j=0;
			for(j=0;j<skippedUsers;j++)
			{
				pTestUsersRatings_SmallLTR[prevUser +1 +j] = 0; //Remember 0 ratings for all the skipped users...
			}
		}

		prevUser = usrId;
		if(usrId>maxUserID)
		{
			maxUserID = usrId;
		}		
		pMetaData->nUsers++;

		// Get number of ratings:
		unsigned int nUserRatings;
		str = strtok(NULL,"|");
		nUserRatings = atoi(str);
		if(TRAINING == whichDB)
		{			
			assert(nUserRatings>0);
			pUsersData[usrId].ratings = nUserRatings;
		}				
		else if (TEST == whichDB)
		{
			assert(RATINGS_PER_USER_TEST == nUserRatings);
			pTestUsersRatings_SmallLTR[usrId] = RATINGS_PER_USER_TEST;
		}
		else
		{
			assert(0);
		}
				
		if(nUserRatings>maxRatingsPerUser)
		{
			maxRatingsPerUser=nUserRatings;
		}
		if(nUserRatings<minRatingsPerUser)
		{
			minRatingsPerUser=nUserRatings;
		}		
		
		//Run on the rating of one user:		
		for(unsigned int rating_idx=0; rating_idx<nUserRatings; rating_idx++)
		{
			fgets(line, MaxLen, fp);									

			//Get the item ID:			
			str = strtok(line,"\t");
			itemId = atoi(str);
			assert(itemId<TrainingMetaData.trainingTotalItems);
			pItemsArr[itemId]=1;
			if(itemId>maxItemID)
			{
				maxItemID=itemId;
			}
			if(itemId<minItemID)
			{
				minItemID=itemId;
			}
			(*ppItemRatings)[pMetaData->nRecords].item = itemId;

			if(TRAINING == whichDB)
			{
				//Get the score:
				str = strtok(NULL,"\t");
				valScore = atoi(str);										
				assert((valScore>=0)&&(valScore<=100));
				score = (unsigned int)valScore;
			}
			else
			{
				score =0;
			}

			(*ppItemRatings)[pMetaData->nRecords].score = score;
			if(score>maxRatingValue)
			{
				maxRatingValue=score;
			}
			if(score<minRatingValue)
			{
				minRatingValue=score;
			}
			
			//Sum scores:
			totalSumScores+=score;			
			pMetaData->nRecords++; 
			assert(pMetaData->nRecords); 
		}
	}
	
	if(TEST == whichDB)
	{
		//Run on the rest of the user, until the end:
		unsigned int u = usrId +1;	
		for(;u<TrainingMetaData.trainingTotalUsers;u++)
		{
			pTestUsersRatings_SmallLTR[u] = 0; //No ratings for this user...
		}		
	}

	fclose(fp);
	
	for(unsigned int i=0; i<TrainingMetaData.trainingTotalItems; i++)
	{
		pMetaData->nItems+=pItemsArr[i];
	}
	
	pMetaData->totalMeanScore=(totalSumScores/pMetaData->nRecords);
	

	End_t = time(NULL);   
	cout<<"Reading ratings is complete!  Elapsed time (secs): "<<difftime(End_t, Start_t)<<endl;
	cout<<"----  STATISTICS:  ----"<<endl
		<<" Users:"<<pMetaData->nUsers<<" Records:"<<pMetaData->nRecords<<" Items:"<<pMetaData->nItems<<endl
		<<" Tracks:"<<countTracks<<" Albums:"<<countAlbums<<" Artists:"<<countArtists<<" Genres:"<<countGenres<<endl
		<<" Max ratings per user:"<<maxRatingsPerUser<<" Min Ratings per User:"<<minRatingsPerUser<<" Max User ID:"<<maxUserID<<endl
		<<" Total Mean score:"<<pMetaData->totalMeanScore<<endl
		<<" Max Item ID:"<<maxItemID<<" Min Item ID:"<<minItemID<<endl
		<<" Max Rating Value:"<<maxRatingValue<<" Min Rating Value:"<<minRatingValue<<endl;		
	return;
}




