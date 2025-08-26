TEST( HashTableInsertOne )
{
    ht_t ht = {0};

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

	LOG_INFO("\n");
	const char* keys[] = { "1", "2", "3" };
	for (int i = 0; i < 3; i++) {
		HTinsertInt( &ht, keys[i], i + 1 );

		HTlog(&ht);
	}

    FAIL_IF_NOT( HT_LOOKUP_INT( &ht, "1" ) == 1,
                 "Expected 1");

	return TEST_PASSED();
}
