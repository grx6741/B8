#include "ht.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define LOG_ENABLED 1
#define LOG_PREFIX "HT"
#include "util.h"

// http://www.cse.yorku.ca/~oz/hash.html
size_t __hash( unsigned char* str )
{
    unsigned long hash = 5381;
    int c;

    while ( ( c = *str++ ) )
        hash = ( ( hash << 5 ) + hash ) + c; /* hash * 33 + c */

    return hash;
}

void HTdelete( ht_t* HT )
{
    for ( int i = 0; i < HT->size; i++ ) {
        ht_element_t* e = HT->elements[i];
        if ( !e )
            continue;

        free( e->key );
        free( e );
    }

    free( HT->elements );
    HT->elements = NULL;
}

int __ht_alloc_and_insert( ht_t* HT, const char* key )
{
    // If the HT has no elements
    if ( !( HT->elements ) ) {
        HT->size = HT->size == 0 ? HT_INITIAL_SIZE : HT->size;
        HT->count = 0;
        HT->elements = calloc( HT->size, sizeof( ht_element_t* ) );
    }

    // If the HT has more elements than 3/4ths of the total size
    if ( HT->count >= ( 3 * HT->size / 4 ) ) {
        // Move to a bigger HT
        ht_t new_ht = ( ht_t ) { .size = HT->size * 2 };
        for ( int i = 0; i < HT->size; i++ ) {
            ht_element_t* e = HT->elements[i];
            if ( !( e ) )
                continue;

            switch ( e->type ) {
                case kHTint:
                    HTinsertInt( &new_ht, e->key, e->int_val );
                    break;
                case kHTcustom:
                    HTinsertCustom( &new_ht, e->key, e->custom_val );
                    break;
                default:
                    assert( 0 && "New HTelementType added, but didn't update this switch case" );
            }

            free( e->key );
            free( e );
            HT->elements[i] = NULL;
        }

        // Replace the new ht's elements with the original
        free( HT->elements );
        HT->elements = new_ht.elements;
        HT->size = new_ht.size;
        HT->count = new_ht.count;
    }

    size_t hash = __hash( ( unsigned char* ) key );
    size_t index = hash % HT->size;

    while ( HT->elements[index] ) {
        // if ( strcmp( HT->elements[index]->key, key ) )
        //     break;
        index = ( index + 1 ) % HT->size;
    }

    HT->elements[index] = malloc( sizeof( ht_element_t ) );
    HT->elements[index]->key = strdup( key );
    HT->count++;

    return index;
}

void HTinsertInt( ht_t* HT, const char* key, int value )
{
    int index = __ht_alloc_and_insert( HT, key );

    HT->elements[index]->int_val = value;
    HT->elements[index]->type = kHTint;
}

void HTinsertCustom( ht_t* HT, const char* key, void* value )
{
    int index = __ht_alloc_and_insert( HT, key );

    HT->elements[index]->custom_val = value;
    HT->elements[index]->type = kHTcustom;
}

ht_element_t* HTlookup( ht_t* HT, const char* key )
{
    if ( !HT->elements || HT->count == 0 ) {
        return NULL;
    }

    size_t hash = __hash( ( unsigned char* ) key );
    size_t start_index = hash % HT->size;
    size_t index = start_index;

    do {
        if ( HT->elements[index] == NULL ) {
            return NULL;
        }

        if ( strcmp( key, HT->elements[index]->key ) == 0 ) {
            return HT->elements[index];
        }

        index = ( index + 1 ) % HT->size;
    } while ( index != start_index );

    return NULL;
}

void HTlog( ht_t* HT )
{
    LOG_INFO( "{\n" );
    for ( int i = 0; i < HT->size; i++ ) {
        ht_element_t* e = HT->elements[i];
        if ( !e )
            continue;
        switch ( e->type ) {
            case kHTint:
                LOG_INFO( "    %s : %d\n", e->key, e->int_val );
                break;
            case kHTcustom:
                LOG_INFO( "    %s : %p\n", e->key, e->custom_val );
                break;
            default:
                assert( 0 && "New HTelementType added, but didn't update this switch case" );
        }
    }
    LOG_INFO( "} [%d/%d]\n", ( int ) HT->count, ( int ) HT->size );
}
