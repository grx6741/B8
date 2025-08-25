#include "ht.h"

#include <stdlib.h>

// http://www.cse.yorku.ca/~oz/hash.html
size_t __hash( unsigned char* str )
{
    unsigned long hash = 5381;
    int c;

    while ( ( c = *str++ ) )
        hash = ( ( hash << 5 ) + hash ) + c; /* hash * 33 + c */

    return hash;
}

ht_t HTcreate()
{
    return ( ht_t ) {
        .elements = calloc( HT_INIAL_SIZE, sizeof( ht_element_t* ) ),
        .size = HT_INIAL_SIZE,
        .count = 0,
    };
}

void HTdelete( ht_t* HT )
{
    free( HT->elements );
    HT->elements = NULL;
}

void HTinsert( ht_t* HT, const char* key, void* value )
{
    size_t hash = __hash( ( unsigned char* ) key );
    size_t index = hash % HT->size;

    HT->elements[index] = malloc( sizeof( ht_element_t ) );
    HT->elements[index]->key = ( char* ) key;
    HT->elements[index]->value = value;

    HT->count++;
}

ht_element_t* HTlookup( ht_t* HT, const char* key )
{
    size_t hash = __hash( ( unsigned char* ) key );
    size_t index = hash % HT->size;

    return HT->elements[index];
}
