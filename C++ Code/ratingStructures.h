// This file is provided for the sample code with the EE810D project. 
// You need to read and revise to the desired algorithm for the music 
// track rating prediction 
//	-------------------------------------------------------------------


#ifndef _RATINGS_STRUCTURE_H_
#define _RATINGS_STRUCTURE_H_


struct ItemRating {	
	unsigned int item			: 20;
	unsigned int score			: 7;

	// For sorting/searching
	bool operator<=(const ItemRating & other) {
		return item<=other.item;
	}
	bool operator<(const ItemRating & other) {
		return item<other.item;
	}
	bool operator>=(const ItemRating & other) {
		return item>other.item;
	}
	bool operator>(const ItemRating & other) {
		return item>other.item;
	}
	bool operator==(const ItemRating & other) { // This kind of equality is important for binary search
		return item==other.item;;
	}
};


typedef enum
{
	TRACK = 0,
	ALBUM,
	ARTIST,
	GENRE,
	NOT_DEFINED
} ITEM_TYPE;

#endif

