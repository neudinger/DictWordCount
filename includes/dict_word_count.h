#include <stdio.h>  // ISO C99 Standard: 7.19 Input/output
#include <string.h> // strlen, strchr, strcmp, strcpy, strcspn
#include <endian.h> // htole64
#include <stdint.h> // uint8_t, uint64_t
#include <stdlib.h> // uint
#include <iso646.h> // ISO C Standard:  7.9  Alternative spellings

#pragma once

#if not defined(DICT_WORD_COUNT)
#define DICT_WORD_COUNT

#define MAX_WORD_LENGTH 100
#define MAX_WORDS_TABLE 1000
#define SEED 42

// A memory view on the word
typedef union Key
{
    char *str;
    uint8_t *byte;
    uint64_t *eightBytes;
} Key;

// Dictionary struct hash table with murmur hash key
typedef struct Dictionary
{
    uint64_t hash_table[MAX_WORDS_TABLE];
    uint dict_count[MAX_WORDS_TABLE];
    char words_table[MAX_WORDS_TABLE][MAX_WORD_LENGTH];
    uint dict_length;
} Dictionary;

int dict_setup(FILE *, Dictionary *);
uint analyse(FILE *, Dictionary *);
#endif // DICT_WORD_COUNT