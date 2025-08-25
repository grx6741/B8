#pragma once

#include "stddef.h"
#include "stdbool.h"

// My Hash Table Implementation

#define HT_INIAL_SIZE 2

typedef struct
{
    char* key;
    void* value;
} ht_element_t;

typedef struct
{
    ht_element_t** elements;
    size_t size;
    size_t count;
} ht_t;

ht_t HTcreate();
void HTdelete( ht_t* HT );

void HTinsert( ht_t* HT, const char* key, void* value );
ht_element_t* HTlookup( ht_t* HT, const char* key );
