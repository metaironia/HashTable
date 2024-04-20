#ifndef HASH_TABLE_LOG_H
#define HASH_TABLE_LOG_H

#define HASH_TABLE_DOT_FILE_NAME  "hash_table_dot_file.dot"

HashTableFuncStatus HashTableGraphDump (const HashTable *hash_table);

HashTableFuncStatus HashTableDotFileBegin (FILE *hash_table_dot);

HashTableFuncStatus HashTableDotFileEnd (FILE *hash_table_dot);

HashTableFuncStatus HashTableDotFilePrint (FILE *hash_table_dot, const HashTable *hash_table);

#endif