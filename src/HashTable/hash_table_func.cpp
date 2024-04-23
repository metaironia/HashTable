#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <inttypes.h>

#ifndef VISUAL_STUDIO
    #include <immintrin.h>
#else
    #include <intrin.h>  // espessialy for visual studio msvc  
#endif

#include "../TextPreparer/text_preparer.h"

#include "hash_func.h"
#include "hash_table_func.h"
#include "hash_table_dsl.h"
#include "hash_table_log.h"

#define HASH_ALGORITHM_VAR_PRINT(hash_table, algorithm_name)                                          \
                                    {                                                                 \
                                        printf (algorithm_name ":\n");                                \
                                        HashTableVariancePrint (HashTableVarianceEval (hash_table));  \
                                    }

HashTableFuncStatus HashTableCtor (HashTable *hash_table, int64_t hash_table_capacity) {

    assert (hash_table);

    HASH_TABLE_SIZE_ = hash_table_capacity;

    if (HashTableDataCtor (hash_table) == HASH_TABLE_FUNC_STATUS_FAIL)
        return HASH_TABLE_FUNC_STATUS_FAIL;

    return HASH_TABLE_FUNC_STATUS_OK;
}

HashTableFuncStatus HashTableDataCtor (HashTable *hash_table) {
    
    assert (hash_table);
    
    HASH_TABLE_CELL_ = (FastList *) calloc (HASH_TABLE_SIZE_, sizeof (FastList));
    assert (HASH_TABLE_CELL_);

    for (size_t i = 0; i < ((size_t) HASH_TABLE_SIZE_); i++)         
        FastListCtor (HASH_TABLE_CELL_ + i, DEFAULT_LIST_CAPACITY);

    return HASH_TABLE_FUNC_STATUS_OK;
}

HashTableFuncStatus HashTableDataDtor (HashTable *hash_table) {

    assert (hash_table);
    assert (HASH_TABLE_CELL_);

    for (size_t i = 0; i < ((size_t) HASH_TABLE_SIZE_); i++)
        FastListDtor (HASH_TABLE_CELL_ + i);

    return HASH_TABLE_FUNC_STATUS_OK;
}

HashTableFuncStatus HashTableDtor (HashTable *hash_table) {

    assert (hash_table);

    HashTableDataDtor (hash_table);
    HASH_TABLE_SIZE_ = 0;

    return HASH_TABLE_FUNC_STATUS_OK;
}

HashTableFuncStatus HashTableInsert (HashTable *hash_table, const HashTableElem_t data, const int64_t key) {

    assert (hash_table);

    if (key < 0)
        return HASH_TABLE_FUNC_STATUS_FAIL;

    int64_t   cell_num = key % HASH_TABLE_SIZE_;
    FastList *cell_ptr = HASH_TABLE_CELL_ + cell_num;

    int64_t stub = 0;

    if (FastListFindElem (cell_ptr, data) == LIST_FUNC_STATUS_FAIL)
        FastListAddElemAfter (cell_ptr, DUMMY_ELEM_POS, &stub, data);

    return HASH_TABLE_FUNC_STATUS_OK;
}

HashTableFuncStatus HashTableFind (HashTable *hash_table, const HashTableElem_t data) {

    assert (hash_table);

    int64_t   cell_num = EighthHash (data) % HASH_TABLE_SIZE_;
    FastList *cell_ptr = HASH_TABLE_CELL_ + cell_num;

    if (FastListFindElem (cell_ptr, data) == LIST_FUNC_STATUS_FAIL)
        return HASH_TABLE_FUNC_STATUS_FAIL;

    return HASH_TABLE_FUNC_STATUS_OK;
}

HashTableFuncStatus HashTableReadData (const char *input_file_name, HashTable *hash_table, 
                                       uint32_t (*hash_func) (const HashTableElem_t)) {

    assert (input_file_name);
    assert (hash_table);
    assert (hash_func);

    FILE *data_file = NULL;
    
    if (!(data_file = fopen (TextPreparedFileNameGen (input_file_name), "r"))) {        // TODO as func

        if (TextPrepare (input_file_name) == PREPARING_FUNC_STATUS_FAIL)
            return HASH_TABLE_FUNC_STATUS_FAIL;

        data_file = fopen (TextPreparedFileNameGen (input_file_name), "r");
    }
    
    char word[MAX_WORD_LENGTH] = {};

    while (!feof (data_file)) {

        int fscanf_status = fscanf (data_file, "%s", word);

        if (fscanf_status != EOF && fscanf_status != 0)
            HashTableInsert (hash_table, strdup (word), hash_func (word));
    }

    return HASH_TABLE_FUNC_STATUS_OK;
}

HashTableFuncStatus HashTableClear (HashTable *hash_table) {

    assert (hash_table);

    for (size_t i = 0; i < (size_t) HASH_TABLE_SIZE_; i++) {
    
        FastList *curr_cell = HASH_TABLE_CELL_ + i;

        (curr_cell -> controlItems).free = 0;
        curr_cell  -> list_size          = 0;

        FillFastList                  (curr_cell, DUMMY_ELEM_POS);
        FastListConnectNeighbourElems (curr_cell, DUMMY_ELEM_POS, DUMMY_ELEM_POS);
    }

    return HASH_TABLE_FUNC_STATUS_OK;
}

double HashTableVarianceEval (const HashTable *hash_table) {

    assert (hash_table);

    size_t num_of_elems = 0;

    for (size_t i = 0; i < ((size_t) HASH_TABLE_SIZE_); i++)
        num_of_elems = ((HASH_TABLE_CELL_ + i) -> list_size);

    const double average_num_of_elems = ((double) num_of_elems) / ((double) HASH_TABLE_SIZE_);

    double variance = 0;

    for (size_t i = 0; i < ((size_t) HASH_TABLE_SIZE_); i++)
        variance += pow (((double) ((HASH_TABLE_CELL_ + i) -> list_size)) - average_num_of_elems, 2);

    variance /= ((double) HASH_TABLE_SIZE_) - 1;  

    return variance;
}

HashTableFuncStatus HashTableTestHashes (const char *input_file_name) {

    assert (input_file_name);

    HashTable hash_table = {};
    HashTableCtor (&hash_table, DEFAULT_HASH_TABLE_CAPACITY);

    if (HashTableReadData (input_file_name, &hash_table, FirstHash) == HASH_TABLE_FUNC_STATUS_FAIL)
        return HASH_TABLE_FUNC_STATUS_FAIL;

    HashTableLoadDump        (&hash_table);
    HASH_ALGORITHM_VAR_PRINT (&hash_table, "First hash");
    HashTableClear           (&hash_table);

    if (HashTableReadData (input_file_name, &hash_table, SecondHash) == HASH_TABLE_FUNC_STATUS_FAIL)
        return HASH_TABLE_FUNC_STATUS_FAIL;

    HashTableLoadDump        (&hash_table);
    HASH_ALGORITHM_VAR_PRINT (&hash_table, "Second hash");
    HashTableClear           (&hash_table);

    if (HashTableReadData (input_file_name, &hash_table, ThirdHash) == HASH_TABLE_FUNC_STATUS_FAIL)
        return HASH_TABLE_FUNC_STATUS_FAIL;

    HashTableLoadDump        (&hash_table);
    HASH_ALGORITHM_VAR_PRINT (&hash_table, "Third hash");
    HashTableClear           (&hash_table);

    HashTable hash_table_cap_101 = {};
    HashTableCtor (&hash_table_cap_101, 101);

    if (HashTableReadData (input_file_name, &hash_table_cap_101, FourthHash) == HASH_TABLE_FUNC_STATUS_FAIL)
        return HASH_TABLE_FUNC_STATUS_FAIL;

    HashTableLoadDump        (&hash_table_cap_101);
    HASH_ALGORITHM_VAR_PRINT (&hash_table_cap_101, "Fourth hash, hash table capacity = 101");

    HashTableDtor     (&hash_table_cap_101);

    if (HashTableReadData (input_file_name, &hash_table, FourthHash) == HASH_TABLE_FUNC_STATUS_FAIL)
        return HASH_TABLE_FUNC_STATUS_FAIL;

    HashTableLoadDump        (&hash_table);
    HASH_ALGORITHM_VAR_PRINT (&hash_table, "Fourth hash, load factor ~= 7");
    HashTableClear           (&hash_table);

    if (HashTableReadData (input_file_name, &hash_table, FifthHash) == HASH_TABLE_FUNC_STATUS_FAIL)
        return HASH_TABLE_FUNC_STATUS_FAIL;

    HashTableLoadDump        (&hash_table);
    HASH_ALGORITHM_VAR_PRINT (&hash_table, "Fifth hash");
    HashTableClear           (&hash_table);

    if (HashTableReadData (input_file_name, &hash_table, SixthHash) == HASH_TABLE_FUNC_STATUS_FAIL)
        return HASH_TABLE_FUNC_STATUS_FAIL;

    HashTableLoadDump        (&hash_table);
    HASH_ALGORITHM_VAR_PRINT (&hash_table, "Sixth hash");
    HashTableClear           (&hash_table);

    if (HashTableReadData (input_file_name, &hash_table, SeventhHash) == HASH_TABLE_FUNC_STATUS_FAIL)
        return HASH_TABLE_FUNC_STATUS_FAIL;

    HashTableLoadDump        (&hash_table);
    HASH_ALGORITHM_VAR_PRINT (&hash_table, "Seventh hash");
    HashTableClear           (&hash_table);

    if (HashTableReadData (input_file_name, &hash_table, EighthHash) == HASH_TABLE_FUNC_STATUS_FAIL)
        return HASH_TABLE_FUNC_STATUS_FAIL;

    HashTableLoadDump        (&hash_table);
    HASH_ALGORITHM_VAR_PRINT (&hash_table, "Eighth hash");

    HashTableDtor (&hash_table);

    return HASH_TABLE_FUNC_STATUS_OK;
}

HashTableFuncStatus HashTableTestFind (const char *input_file_name) {

    assert (input_file_name);

    HashTable hash_table = {};
    HashTableCtor (&hash_table, DEFAULT_HASH_TABLE_CAPACITY);

    if (HashTableReadData (input_file_name, &hash_table, EighthHash) == HASH_TABLE_FUNC_STATUS_FAIL)
        return HASH_TABLE_FUNC_STATUS_FAIL;

    FILE *data_file = NULL;

    if (!(data_file = fopen (TextPreparedFileNameGen (input_file_name), "r"))) {

        if (TextPrepare (input_file_name) == PREPARING_FUNC_STATUS_FAIL)
            return HASH_TABLE_FUNC_STATUS_FAIL;

        data_file = fopen (TextPreparedFileNameGen (input_file_name), "r");
    }

    Words words_from_file = {};    
    WordsCtor (data_file, &words_from_file);
    
    int64_t cycle_start = __rdtsc();
 
    HashTableFindBenchmark (&hash_table, &words_from_file);

    int64_t cycle_end = __rdtsc();

    WordsDtor (&words_from_file);
    
    fclose (data_file);
    data_file = NULL;

    printf ("Hash table find benchmark, %d times\n"
            "Initial CPU cycle     = %" PRId64 "\n"
            "End CPU cycle         = %" PRId64 "\n"
            "Cycles to compute all = %" PRId64 "\n",
            MAX_BENCHMARK_COMP_NUM,
            cycle_start, cycle_end, cycle_end - cycle_start);

    return HASH_TABLE_FUNC_STATUS_OK;
}

HashTableFuncStatus HashTableFindBenchmark (HashTable *hash_table, Words *words) {

    assert (hash_table);
    assert (words);
    assert (words -> word);

    for (size_t i = 0; i < MAX_BENCHMARK_COMP_NUM; i++) {

        char *curr_word = (words -> word);

        for (size_t word_num = 0; word_num < ((size_t) words -> num_of_words); word_num++) {

            //HashTableFind (hash_table, curr_word);
            curr_word += MAX_WORD_LENGTH;
        }
    }

    return HASH_TABLE_FUNC_STATUS_OK;
}

HashTableFuncStatus WordsCtor (FILE *data_file, Words *words) {

    assert (data_file);
    assert (words);

    for (; !feof (data_file); (words -> num_of_words)++)
        fscanf (data_file, "%*s");

    (words -> word) = (char *) calloc ((words -> num_of_words),
                                       MAX_WORD_LENGTH);

    fseek (data_file, 0, SEEK_SET);

    for (size_t i = 0; !feof (data_file); i++)
        fscanf (data_file, "%s", (words -> word) + i * MAX_WORD_LENGTH);

    return HASH_TABLE_FUNC_STATUS_OK;
}

HashTableFuncStatus WordsDtor (Words *words) {

    assert (words);

    free (words -> word);
    (words -> word) = NULL;   
   
    words -> num_of_words = 0;

    return HASH_TABLE_FUNC_STATUS_OK;
}