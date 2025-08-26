#pragma once

#include "stddef.h"
#include "stdbool.h"

// My Hash Table Implementation

#define HT_INITIAL_SIZE 2

typedef enum
{
	kHTint,
	kHTcustom,
	kHTelementTypeCount
} HTelementType;

typedef struct
{
	HTelementType type;
    char* key;
	union {
		int int_val;
		void* custom_val;
	};
} ht_element_t;

typedef struct
{
    ht_element_t** elements;
    size_t size;
    size_t count;
} ht_t;

void HTdelete( ht_t* HT );

void HTinsertInt( ht_t* HT, const char* key, int value );
void HTinsertCustom( ht_t* HT, const char* key, void* value );

#define HT_LOOKUP_INT(HT, key) \
	(HTlookup( HT, key ))->int_val
#define HT_LOOKUP_CUSTOM(HT, key) \
	(HTlookup( HT, key ))->custom_val
ht_element_t* HTlookup( ht_t* HT, const char* key );

void HTlog(ht_t* HT);
