#ifndef HASH_FUNC_H
#define HASH_FUNC_H

#include <stdint.h>

#include "hash_table_func.h"

const uint32_t MAGIC_NUM_1 = 0xcc9e2d51;
const uint32_t MAGIC_NUM_2 = 0x1b873593;
const uint32_t MAGIC_NUM_3 = 0xe6546b64;
const uint32_t MAGIC_NUM_4 = 0x85ebca6b;
const uint32_t MAGIC_NUM_5 = 0xc2b2ae35;
const int      MAGIC_NUM_6 = 5;

const int MAGIC_NUM_FOR_ROL_1 = 15;
const int MAGIC_NUM_FOR_ROL_2 = 13;

const int MAGIC_NUM_FOR_BIT_SHIFT_1 = 16;
const int MAGIC_NUM_FOR_BIT_SHIFT_2 = 13;

uint32_t ConstHash (const HashTableElem_t value);

uint32_t FirstSymHash (const HashTableElem_t value);

uint32_t LenHash (const HashTableElem_t value);

uint32_t AsciiSumHash (const HashTableElem_t value);

uint32_t AsciiSumDivLenHash (const HashTableElem_t value);

uint32_t RorHash (const HashTableElem_t value);

uint32_t RolHash (const HashTableElem_t value);

uint32_t MurmurHash (const HashTableElem_t value);

uint32_t MyRol (const uint32_t number, int num_to_rol);

uint32_t MyRor (const uint32_t number, int num_to_ror);

#endif