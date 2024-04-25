#include <string.h>
#include <stdint.h>
#include <immintrin.h>

#include "hash_table_func.h"
#include "hash_func.h"

uint32_t ConstHash (const HashTableElem_t value) {

    return 0;
}

uint32_t FirstSymHash (const HashTableElem_t value) {

    return ((int64_t) value[0]);
}

uint32_t LenHash (const HashTableElem_t value) {

    return ((uint32_t) strlen (value));
}

uint32_t AsciiSumHash (const HashTableElem_t value) {

    uint32_t ascii_codes_sum = 0;
    uint32_t word_length     = (uint32_t) strlen (value);

    for (size_t i = 0; i < word_length; i++)
        ascii_codes_sum += value[i];

    return ascii_codes_sum;
}

uint32_t AsciiSumDivLenHash (const HashTableElem_t value) {

    uint32_t ascii_codes_sum = 0;

    uint32_t word_length = (uint32_t) strlen (value);

    for (size_t i = 0; i < word_length; i++)
        ascii_codes_sum += value[i];

    return ((uint32_t) (ascii_codes_sum / word_length));
}

uint32_t RorHash (const HashTableElem_t value) {

    uint32_t word_length = (uint32_t) strlen (value);

    if (word_length == 0)
        return 0;

    uint32_t hash = value[0];

    for (size_t i = 1; i <= word_length; i++) {

        hash = MyRor (hash, 1);
        hash ^= value[i];
    }

    return hash;
}

uint32_t RolHash (const HashTableElem_t value) {

    uint32_t word_length = (uint32_t) strlen (value);

    if (word_length == 0)
        return 0;

    uint32_t hash = value[0];

    for (size_t i = 1; i <= word_length; i++) {

        hash = MyRol (hash, 1);
        hash ^= value[i];
    }

    return hash;
}

uint32_t MurmurHash (const HashTableElem_t value) {

    const uint8_t *key  = (const uint8_t *) value;
    const uint32_t len  = (uint32_t) strlen ((const char *) key);
/*
    uint32_t len = 0;

    __asm__ (".intel_syntax noprefix\n\t"
                             "vlddqu ymm1, [%1]\n\t"
                             "vpxor ymm2, ymm2, ymm2\n\t"
                             "vpcmpeqb ymm1, ymm1, ymm2\n\t"
                             "vpmovmskb %0, ymm1\n\t"
                             "tzcnt %0, %0"
                             : "=X" (len)
                             : "r" (value));

*/
    const uint32_t seed = 0;
          
    uint32_t hash             = seed;
    uint32_t four_bytes_block = 0;

    for (uint32_t i = len / 4; i > 0; i--) {

        memcpy (&four_bytes_block, key, sizeof (uint32_t));

        key += sizeof (uint32_t);

        four_bytes_block *= MAGIC_NUM_1;
        four_bytes_block  = MyRol (four_bytes_block, MAGIC_NUM_FOR_ROL_1);
        four_bytes_block *= MAGIC_NUM_2;

        hash ^= four_bytes_block;
        hash  = MyRol (hash, MAGIC_NUM_FOR_ROL_2);
        hash  = hash * MAGIC_NUM_6 + MAGIC_NUM_3;
    }

    four_bytes_block = 0;

    for (uint32_t i = len % 4; i > 0; i--) {

        four_bytes_block <<= 8;
        four_bytes_block  |= key [i - 1];
    }

    four_bytes_block *= MAGIC_NUM_1;
    four_bytes_block  = MyRol (four_bytes_block, MAGIC_NUM_FOR_ROL_1);
    four_bytes_block *= MAGIC_NUM_2;

    hash ^= four_bytes_block;

    hash ^= len;

    hash ^= (hash >> MAGIC_NUM_FOR_BIT_SHIFT_1);
    hash *= MAGIC_NUM_4;
    hash ^= (hash >> MAGIC_NUM_FOR_BIT_SHIFT_2);
    hash *= MAGIC_NUM_5;
    hash ^= (hash >> MAGIC_NUM_FOR_BIT_SHIFT_1);

    return hash;
}

uint32_t MyRol (const uint32_t number, int num_to_rol) {

    num_to_rol %= sizeof (uint32_t) * 8;

    return ((number << num_to_rol) | (number >> ((sizeof (uint32_t)) * 8 - num_to_rol)));
}

uint32_t MyRor (const uint32_t number, int num_to_ror) {

    num_to_ror %= sizeof (uint32_t) * 8;

    return (number << ((sizeof (uint32_t)) * 8 - num_to_ror)) | (number >> num_to_ror);
}
