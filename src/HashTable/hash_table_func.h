#ifndef HASH_TABLE_FUNC_H
#define HASH_TABLE_FUNC_H

#include "../DoubleLinkedList/fast_list_func.h"

const int DEFAULT_LIST_CAPACITY       = 1;
const int DEFAULT_HASH_TABLE_CAPACITY = 100;

enum HashTableFuncStatus {

    HASH_TABLE_FUNC_STATUS_OK,
    HASH_TABLE_FUNC_STATUS_FAIL
};

struct HashTable {

    FastList *cell;
    size_t size;
};

HashTableFuncStatus HashTableCtor (HashTable *hash_table);

HashTableFuncStatus HashTableDataCtor (HashTable *hash_table);

HashTableFuncStatus HashTableDataDtor (HashTable *hash_table);

HashTableFuncStatus HashTableDtor (HashTable *hash_table);

#endif