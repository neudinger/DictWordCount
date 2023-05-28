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
static uint64_t MurmurHash64A(const Key *key,
                              const uint64_t seed)
{
    static const uint64_t m = 14313749767032793493UL;
    const size_t len = strlen(key->str);
    const int r = 47;
    uint64_t h = seed ^ (len * m);
    const uint64_t *data = key->eightBytes;
    const uint64_t *end = (len >> 3) + data;

    while (data not_eq end)
    {
        uint64_t k = htole64(*data++);
        k *= m, k ^= k >> r, k *= m, h ^= k, h *= m;
    }
    const u_char *data2 = key->byte;
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

static int is_empty_or_commented(const char *line)
{
    return (line[0] == '\0' or line[0] == '#' or line[0] == '\n');
}

static int check_line_has_one_word(FILE *dictfile,
                                   const char *word)
{
    if (strchr(word, ' ') not_eq NULL)
    {
        fprintf(stderr, "Error: Dictionary line has more than one word: %s", word);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

static int check_if_already_in_dict(const Dictionary *dict,
                                    const char *word)
{
    int return_value = EXIT_SUCCESS;
    Key *key = (Key *)&(word);
    const uint64_t hash = MurmurHash64A(key, SEED);
#pragma omp parallel for shared(dict) firstprivate(hash)
    for (uint dict_idx = 0U; dict_idx < dict->dict_length; dict_idx++)
        // Fast check in hash table
        // and Check if it is not a collision
        if (hash == dict->hash_table[dict_idx] and
            not strcmp(word, dict->words_table[dict_idx]))
        {
            return_value = EXIT_FAILURE;
/*        */#pragma omp cancellation point for
        }
    return return_value;
}

static void add_word_in_dict(Dictionary *dict,
                             const char *word)
{
    Key *key = (Key *)&(word);
    dict->hash_table[dict->dict_length] = MurmurHash64A(key, SEED);
    strcpy(dict->words_table[dict->dict_length], word);
    dict->dict_length += 1;
}

uint analyse(FILE *input_stream,
             Dictionary *dict)
{
    uint words_length = 0, words_detected = 0;
#pragma omp parallel default(none) shared(words_length, dict, words_detected, input_stream)
    {
#pragma omp single nowait
        {
#pragma omp task shared(words_length, dict, words_detected, input_stream)
            {
                char word[MAX_WORD_LENGTH];
                // Setup the union Key view on buffer
                // For fast memory access
                const char *word_ptr = (char *)word;
                Key *key = (Key *)&word_ptr;
                while (fscanf(input_stream, "%s", word) not_eq EOF)
                {
                    words_length += 1;
                    // Check if word exists in the dictionary
                    const uint64_t hash = MurmurHash64A(key, SEED);
                    for (uint dict_idx = 0U; dict_idx < dict->dict_length; dict_idx++)
                        // Fast check in hash table
                        // Check if it is not a hash collision
                        if (hash == dict->hash_table[dict_idx] and
                            strcmp(word, dict->words_table[dict_idx]) == 0)
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
