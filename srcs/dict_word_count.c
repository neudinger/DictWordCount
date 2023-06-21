/*
 * File dict_word_count.c
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

#include <dict_word_count.h>

#if defined(_OPENMP)
#include <omp.h>
#pragma message("OPENMP ON")
#endif // _OPENMP

//-----------------------------------------------------------------------------
// MurmurHash2, 64-bit versions, by Austin Appleby

// The same caveats as 32-bit MurmurHash2 apply here - beware of alignment
// and endian-ness issues if used across multiple platforms.
// 64-bit hash for 64-bit platforms
// unsigned int hash_high = (hash >> 32);
// unsigned int hash_low = (unsigned int)hash;
static uint64_t MurmurHash64A(Key const *key,
                              uint64_t const seed)
{
    static const uint64_t m = 14313749767032793493UL;
    size_t const len = strlen(key->str);
    int const r = 47;
    uint64_t const *data = key->eightBytes;
    uint64_t const *end = (len >> 3) + data;
    uint64_t h = seed ^ (len * m);

    while (data not_eq end)
    {
        uint64_t k = htole64(*data++);
        k *= m, k ^= k >> r, k *= m, h ^= k, h *= m;
    }
    uint8_t const *data2 = key->byte;
    switch (len & 7) // faster operation than (len % 8)
    {
    case 7:
        h ^= (uint64_t)(data2[6]) << 48;
    case 6:
        h ^= (uint64_t)(data2[5]) << 40;
    case 5:
        h ^= (uint64_t)(data2[4]) << 32;
    case 4:
        h ^= (uint64_t)(data2[3]) << 24;
    case 3:
        h ^= (uint64_t)(data2[2]) << 16;
    case 2:
        h ^= (uint64_t)(data2[1]) << 8;
    case 1:
        h ^= (uint64_t)(data2[0]);
        h *= m;
    };
    return h ^= h >> r, h *= m, h ^= h >> r;
}

static int is_empty_or_commented(char const *line)
{
    return (line[0] == '\0' or line[0] == '#' or line[0] == '\n');
}

static int check_line_has_one_word(FILE *dictfile,
                                   char const *word)
{
    if (strchr(word, ' ') not_eq NULL)
    {
        fprintf(stderr, "Error: Dictionary line has more than one word: %s", word);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

static inline bool fastcheck(Dictionary const *dict,
                             uint64_t const dict_idx,
                             char const *word,
                             uint64_t const hash)
{
    return (hash == dict->hash_table[dict_idx] and not strcmp(word, dict->words_table[dict_idx]));
}

static const int check_if_already_in_dict(const Dictionary *dict,
                                          const char *word)
{
    Key const *key = (Key *)&(word);
    uint64_t const hash = MurmurHash64A(key, SEED);
    for (uint64_t dict_idx = 0U; dict_idx < dict->dict_length; dict_idx++)
        // Fast check in hash table
        // and Check if it is not a collision
        if (fastcheck(dict, dict_idx, word, hash))
            return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

static void add_word_in_dict(Dictionary *dict,
                             char const *word)
{
    Key *key = (Key *)&(word);
    dict->hash_table[dict->dict_length] = MurmurHash64A(key, SEED);
    strcpy(dict->words_table[dict->dict_length], word);
    dict->dict_length += 1;
}

uint64_t analyse(FILE *input_stream,
                 Dictionary *dict)
{
    uint64_t words_length = 0, words_detected = 0;
#pragma omp parallel default(none) shared(words_length, dict, words_detected, input_stream)
    {
#pragma omp single nowait
        {
#pragma omp task shared(word, words_length, dict, words_detected, input_stream)
            {
                char word[MAX_WORD_LENGTH];
                // Setup the union Key view on buffer
                // For fast memory access
                char const *word_ptr = (char *)word;
                Key const *key = (Key *)&word_ptr;
                while (fscanf(input_stream, "%s", word) not_eq EOF)
                {
                    words_length += 1;
                    // Check if word exists in the dictionary
                    uint64_t const hash = MurmurHash64A(key, SEED);
                    for (uint64_t dict_idx = 0U; dict_idx < dict->dict_length; dict_idx++)
                        // Fast check in hash table
                        // Check if it is not a hash collision
                        if (fastcheck(dict, dict_idx, word, hash))
                        {
                            words_detected += 1;
                            dict->dict_count[dict_idx] += 1;
                            break;
                        }
                }
            }
        }
    }
    return words_length;
}

int dict_setup(FILE *dictfile,
               Dictionary *dict)
{
    char word[MAX_WORD_LENGTH];
    while (fgets(word, sizeof(word), dictfile))
    {
        // Ignore empty lines and comments
        if (is_empty_or_commented(word))
            continue;
        // Check
        if (check_line_has_one_word(dictfile, word))
        {
            fclose(dictfile);
            exit(EXIT_FAILURE);
        }
        word[strcspn(word, "\n")] = '\0';
        if (check_if_already_in_dict(dict, word))
        {
            fprintf(stderr,
                    "Error: Duplicate word in the dictionary: %s\n",
                    word);
            fclose(dictfile);
            exit(EXIT_FAILURE);
        }
        add_word_in_dict(dict, word);
    }
    fclose(dictfile);
    return EXIT_SUCCESS;
}
