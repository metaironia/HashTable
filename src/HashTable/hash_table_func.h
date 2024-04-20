#ifndef HASH_TABLE_FUNC_H
#define HASH_TABLE_FUNC_H

#include <stdint.h>

#include "../DoubleLinkedList/fast_list_func.h"

const int DEFAULT_LIST_CAPACITY       = 1;
const int DEFAULT_HASH_TABLE_CAPACITY = 100;

typedef FastListElem_t HashTableElem_t;

enum HashTableFuncStatus {

    HASH_TABLE_FUNC_STATUS_OK,
    HASH_TABLE_FUNC_STATUS_FAIL
};

struct HashTable {

    FastList *cell;
    int64_t size;
};

HashTableFuncStatus HashTableCtor (HashTable *hash_table);

HashTableFuncStatus HashTableDataCtor (HashTable *hash_table);

HashTableFuncStatus HashTableDataDtor (HashTable *hash_table);

HashTableFuncStatus HashTableDtor (HashTable *hash_table);

HashTableFuncStatus HashTableInsert (HashTable *hash_table, const HashTableElem_t data, const int64_t key);

#endif