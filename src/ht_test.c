TEST( HashTableCreate )
{
    ht_t ht = HTcreate();
    FAIL_IF_NOT( ht.size == HT_INIAL_SIZE, "Error Creating Hash Table" );

    return TEST_PASSED();
}

TEST( HashTableDelete )
{
    ht_t ht = HTcreate();
    HTdelete( &ht );
    FAIL_IF_NOT( ht.elements == NULL, "Error Deleting Hash Table" );

    return TEST_PASSED();
}

TEST( HashTableInsertOne )
{
    ht_t ht = HTcreate();

    static int abc = 2;
    HTinsert( &ht, "abc", ( void* ) &abc );

    FAIL_IF_NOT( ht.count == 1, "Error Inserting One Element" );

    return TEST_PASSED();
}

TEST( HashTableLookupOne )
{
    ht_t ht = HTcreate();

    int abc = 2;
    HTinsert( &ht, "abc", ( void* ) &abc );

    FAIL_IF_NOT( *( int* ) ( HTlookup( &ht, "abc" )->value ) == 2,
                 "Did Not Get the same element back when inserting one element" );

    return TEST_PASSED();
}
