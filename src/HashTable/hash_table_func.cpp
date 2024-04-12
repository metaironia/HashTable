#include <assert.h>
#include <stdlib.h>

#include "hash_table_func.h"
#include "hash_table_dsl.h"

HashTableStatus HashTableCtor (HashTable *hash_table) {

    assert (hash_table);

    HASH_TABLE_SIZE_ = DEFAULT_HASH_TABLE_CAPACITY;

    if (HashTableDataCtor (hash_table) == HASH_TABLE_STATUS_FAIL)
        return HASH_TABLE_STATUS_FAIL;

    return HASH_TABLE_STATUS_OK;
}

HashTableStatus HashTableDataCtor (HashTable *hash_table) {
    
    assert (hash_table);
    
    HASH_TABLE_CELL_ = (FastList *) calloc (HASH_TABLE_SIZE_, sizeof (FastList));
    assert (HASH_TABLE_CELL_);

    for (size_t i = 0; i < HASH_TABLE_SIZE_; i++)         
        FastListCtor (HASH_TABLE_CELL_ + i, DEFAULT_LIST_CAPACITY);

    return HASH_TABLE_STATUS_OK;
}

HashTableStatus HashTableDataDtor (HashTable *hash_table) {

    assert (hash_table);
    assert (HASH_TABLE_CELL_);

    for (size_t i = 0; i < HASH_TABLE_SIZE_; i++)
        FastListDtor (HASH_TABLE_CELL_ + i);

    return HASH_TABLE_STATUS_OK;
}

HashTableStatus HashTableDtor (HashTable *hash_table) {

    assert (hash_table);

    HashTableDataDtor (hash_table);
    HASH_TABLE_SIZE_ = 0;

    return HASH_TABLE_STATUS_OK;
}