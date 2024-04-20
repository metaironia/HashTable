#ifndef HASH_TABLE_LOG_H
#define HASH_TABLE_LOG_H

#define HASH_TABLE_DOT_FILE_NAME  "hash_table_dot_file.dot"

const int MAX_FILE_NAME_LENGTH = 32;

HashTableFuncStatus HashTableGraphDump (const HashTable *hash_table);

HashTableFuncStatus HashTableDotFileBegin (FILE *hash_table_dot);

HashTableFuncStatus HashTableDotFileEnd (FILE *hash_table_dot);

HashTableFuncStatus HashTableDotFilePrint (FILE *hash_table_dot, const HashTable *hash_table);

HashTableFuncStatus HashTableLoadDump (const HashTable *hash_table);

const char *HashTableLoadFileNameGen (void);

#endif