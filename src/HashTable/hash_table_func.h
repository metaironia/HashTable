#ifndef HASH_TABLE_FUNC_H
#define HASH_TABLE_FUNC_H

#include "../DoubleLinkedList/fast_list_func.h"

const int DEFAULT_LIST_CAPACITY       = 1;
const int DEFAULT_HASH_TABLE_CAPACITY = 100;

enum HashTableStatus {

    HASH_TABLE_STATUS_OK,
    HASH_TABLE_STATUS_FAIL
};

struct HashTable {

    FastList *cell;
    size_t size;
};

HashTableStatus HashTableCtor (HashTable *hash_table);

HashTableStatus HashTableDataCtor (HashTable *hash_table);

HashTableStatus HashTableDataDtor (HashTable *hash_table);

HashTableStatus HashTableDtor (HashTable *hash_table);

#endif