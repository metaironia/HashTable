#ifndef HASH_TABLE_FUNC_H
#define HASH_TABLE_FUNC_H

#include <stdint.h>

#include "../DoubleLinkedList/fast_list_func.h"

const int DEFAULT_LIST_CAPACITY       = 1;
const int DEFAULT_HASH_TABLE_CAPACITY = 787;

typedef FastListElem_t HashTableElem_t;

enum HashTableFuncStatus {

    HASH_TABLE_FUNC_STATUS_OK,
    HASH_TABLE_FUNC_STATUS_FAIL
};

struct HashTable {

    FastList *cell;
    int64_t size;
};

HashTableFuncStatus HashTableCtor (HashTable *hash_table, int64_t hash_table_capacity);

HashTableFuncStatus HashTableDataCtor (HashTable *hash_table);

HashTableFuncStatus HashTableDataDtor (HashTable *hash_table);

HashTableFuncStatus HashTableDtor (HashTable *hash_table);

HashTableFuncStatus HashTableInsert (HashTable *hash_table, const HashTableElem_t data, const int64_t key);

HashTableFuncStatus HashTableReadData (const char *input_file_name, HashTable *hash_table, 
                                       uint32_t (*hash_func) (const HashTableElem_t));

HashTableFuncStatus HashTableClear (HashTable *hash_table);

HashTableFuncStatus HashTableTestHashes (const char *input_file_name);

#endif