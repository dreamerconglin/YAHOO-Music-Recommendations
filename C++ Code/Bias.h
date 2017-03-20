// This file is provided for the sample code with the EE810D project. 
// You need to read and revise to the desired algorithm for the music 
// track rating prediction 
//	-------------------------------------------------------------------

#ifndef _BIAS_H_
#define _BIAS_H_

#include "readUserData.h"
#include <assert.h>
#include <iostream>
using namespace std;
#define max(A,B) ((A)>=(B) ? (A) : (B))
#define min(A,B) ((A)<=(B) ? (A) : (B))

/*
 *
 * The Bias Manager class (CBiasManager) handles the items and users biases.
 *
*/
class CBiasManager
{
public:	

	CBiasManager();
	~CBiasManager();
	void allocate(); // Allocate biases. This method is called before SGD starts iterating on the training dataset.

	inline double getMu();

	//Item biases:
	inline double getItemBias(ItemRating ratingData);
	inline void updateItemBias(ItemRating ratingData, double err);

	//User biases:
	inline double getUserBias(unsigned int user);
	inline void updateUserBias(unsigned int user, double err);
		
private:
	float	*pItemsBase; //An array of all the items biases
	float   *pUsersBase; //An array of all the users biases

	bool allocateOrCleanBaseVector(float** ppBase, unsigned int length);  // A utility method to allocate new arrays	
	inline double doStep(double origVal, double err, double step, double reg);	// A utility method to update the bias paramters after each SGD step.
};
extern CBiasManager BiasManager;


//A Simple implmenttation of Stochastic Gradient Descent (SGD):
void gradientDescent(int nIterations, double & correspondingTrainingRmse);

//This method takes a rating instance and a user ID, and predicts the rating score (used both in SGD and after SGD is complete):
inline double estimate(ItemRating itemRating,unsigned int user);

//Called by SGD to update biases after each rating line:
inline void	update(ItemRating ratingData, unsigned int user, unsigned int item, double err);

//This method iterates on the test dataset (for track-2), and predicts simulated results
void predictTrack2Test(char * filename);

/**********************************************************************
			Inline Functions Go Below:
**********************************************************************/


inline double estimate(ItemRating itemRating, unsigned int user)
{		
	unsigned int item = itemRating.item;
	double estimation = 0;		

	estimation = BiasManager.getMu();
	estimation+= BiasManager.getUserBias(user);				
	estimation+= BiasManager.getItemBias(itemRating);		
	
	return estimation;
}
inline void update(ItemRating ratingData, unsigned int user, unsigned int item, double err)
{			
	BiasManager.updateItemBias(ratingData,err);
	BiasManager.updateUserBias(user,err);
	return;
}

inline double CBiasManager::getMu()
{		
	return TrainingMetaData.totalMeanScore;	
}

inline double CBiasManager::getItemBias(ItemRating ratingData)
{		
	assert(pItemsBase); 	
	assert(ratingData.item<TrainingMetaData.trainingTotalItems);		
	return pItemsBase[ratingData.item];	
}

inline void CBiasManager::updateItemBias(ItemRating ratingData, double err)
{
	unsigned int item = ratingData.item;
	double oldBias=-1, newBias=-1; 
	assert(pItemsBase); assert(item<=TrainingMetaData.trainingTotalItems);		
	oldBias = pItemsBase[ratingData.item];
	newBias = doStep(oldBias,err,itemsStep,itemsReg);
	pItemsBase[ratingData.item] = (float)newBias;
}

inline double CBiasManager::getUserBias(unsigned int user)
{
	assert(pUsersBase);	
	return pUsersBase[user];
}

inline void CBiasManager::updateUserBias(unsigned int user, double err)
{
	assert(pUsersBase);
	double oldBias=0, newBias=0; 
	oldBias = pUsersBase[user];
	newBias = doStep(oldBias,err,usersStep,usersReg);		
	pUsersBase[user] = (float)newBias;
}

inline double CBiasManager::doStep(double origVal, double err, double step, double reg)
{
	/*
	 * This utility method updates a paramter according to it's derivative. 
	 * The SGD algorithm uses this method after any rating in order to update the user bias or the item bias. 
	 * Please note that the update of both biases according to their derivatives is similar,
	 * given the values of the bias itself, the step size and regularization parameter.
	*/

	double newVal = 0;
	newVal = origVal + step*(err-reg*origVal);		
	return newVal;
}

#endif

