#ifndef HASH_TABLE_FUNC_H
#define HASH_TABLE_FUNC_H

#include <stdint.h>
#include <stdio.h>

#include "../DoubleLinkedList/fast_list_func.h"

typedef FastListElem_t HashTableElem_t;

const int DEFAULT_LIST_CAPACITY       = 1;
const int DEFAULT_HASH_TABLE_CAPACITY = 787;

const int MAX_BENCHMARK_COMP_NUM = 20000;

enum HashTableFuncStatus {

    HASH_TABLE_FUNC_STATUS_OK,
    HASH_TABLE_FUNC_STATUS_FAIL
};

struct HashTable {

    FastList *cell;
    int64_t size;
};

struct Words {

    char *word;
    int64_t num_of_words;
};

HashTableFuncStatus HashTableCtor (HashTable *hash_table, int64_t hash_table_capacity);

HashTableFuncStatus HashTableDataCtor (HashTable *hash_table);

HashTableFuncStatus HashTableDataDtor (HashTable *hash_table);

HashTableFuncStatus HashTableDtor (HashTable *hash_table);

HashTableFuncStatus HashTableInsert (HashTable *hash_table, const HashTableElem_t data, const int64_t key);

char *MyStrdup32Bytes (const char *word);

HashTableFuncStatus HashTableFind (HashTable *hash_table, const HashTableElem_t data);

HashTableFuncStatus HashTableFindBenchmark (HashTable *hash_table, Words *words);

HashTableFuncStatus HashTableReadData (const char *input_file_name, HashTable *hash_table, 
                                       uint32_t (*hash_func) (const HashTableElem_t));

HashTableFuncStatus HashTableClear (HashTable *hash_table);

double HashTableVarianceEval (const HashTable *hash_table);

HashTableFuncStatus HashTableTestHashes (const char *input_file_name);

HashTableFuncStatus HashTableTestFind (const char *input_file_name);

HashTableFuncStatus WordsCtor (FILE *data_file, Words *words);

HashTableFuncStatus WordsDtor (Words *words);

#endif