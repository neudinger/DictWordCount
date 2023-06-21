/*
 * File dict_word_count.h
 * Created on Mon May 29 2023
 * Copyright (c) 2023 Etherogene
 *
 * The MIT License (MIT)
 * Copyright (c) 2023 Barre kevin
 * Contact barre_k@laposte.net, kevin.barre@epitech.eu
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
 * TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdio.h>  // ISO C99 Standard: 7.19 Input/output
#include <string.h> // strlen, strchr, strcmp, strcpy, strcspn
#include <endian.h> // htole64
#include <stdint.h> // uint8_t, uint64_t
#include <stdlib.h> // uint
#include <iso646.h> // ISO C Standard:  7.9  Alternative spellings
#include <sys/types.h>

#pragma once

#if not defined(DICT_WORD_COUNT)
#define DICT_WORD_COUNT

#define MAX_WORD_LENGTH 100
#define MAX_WORDS_TABLE 1000
#define SEED 42

typedef u_int8_t bool;

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
    uint64_t dict_count[MAX_WORDS_TABLE];
    char words_table[MAX_WORDS_TABLE][MAX_WORD_LENGTH];
    uint64_t dict_length;
} Dictionary;

int dict_setup(FILE *, Dictionary *);
uint64_t analyse(FILE *, Dictionary *);
#endif // DICT_WORD_COUNT