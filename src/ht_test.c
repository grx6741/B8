TEST( HashTableInsertOne )
{
    ht_t ht = { 0 };

    HTinsertInt( &ht, "abc", 2 );

    FAIL_IF_NOT( ht.count == 1, "Error Inserting One Element" );

    return TEST_PASSED();
}

TEST( HashTableLookupOne )
{
    ht_t ht = { 0 };

    HTinsertInt( &ht, "abc", 2 );

    FAIL_IF_NOT( HT_LOOKUP_INT( &ht, "abc" ) == 2,
                 "Did Not Get the same element back when inserting one element" );

    return TEST_PASSED();
}

TEST( HashTableLookupMultiple )
{
    ht_t ht = { .size = 4 };

    const char* keys[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
    for ( int i = 0; i < 9; i++ ) {
        HTinsertInt( &ht, keys[i], i + 1 );
    }

    FAIL_IF_NOT( HT_LOOKUP_INT( &ht, "1" ) == 1, "Expected 1" );

    return TEST_PASSED();
}

TEST( HashTableInsertandUpdate )
{
    ht_t ht = { 0 };
    HTinsertInt( &ht, "a", 0 );
    HTinsertInt( &ht, "a", 1 );
    FAIL_IF_NOT( HT_LOOKUP_INT( &ht, "a" ) == 1, "Expected 1" );

    return TEST_PASSED();
}
