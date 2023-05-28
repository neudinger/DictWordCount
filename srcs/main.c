#include <dict_word_count.h>

static int print_usage()
{
    printf("Usage: wordcount dictfile [textfile1 textfile2 ...]\n");
    return EXIT_SUCCESS;
}

static void print_result(const Dictionary *dict,
                         const uint words_length)
{
    for (uint dict_idx = 0; dict_idx < dict->dict_length; dict_idx++)
        if (dict->dict_count[dict_idx] > 0)
            printf("%d\t%s\n", dict->dict_count[dict_idx], dict->words_table[dict_idx]);
    printf("%d total words\n", words_length);
}

int main(int argc, char const **argv)
{

    if (argc < 2)
        return print_usage();

    int return_value = EXIT_SUCCESS;
    // Process the dictionary file
    // Read the dictionary file
    FILE *dictfile;
    if ((dictfile = fopen(argv[1], "r")) == NULL)
    {
        fprintf(stderr, "Error opening dictionary file: %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    Dictionary dict;
    dict.dict_length = 0;
    // Index the words in the Dictionary hash table
    dict_setup(dictfile, &dict);

    // =============================
    // Process the input stream
    FILE *input_stream;
    uint words_length = 0;

    if (argc < 3)
    {
        // Processes its input stream:
        // of stdin
        input_stream = stdin;
        words_length += analyse(input_stream, &dict);
        fclose(input_stream);
    }
    else
    {
        // wordcount then processes its input stream:
        // the concatenation of all text files specified on the command line
        for (uint file_idx = 2; file_idx < argc; file_idx++)
        {
            // Read from text files specified on the command line
            input_stream = fopen(argv[file_idx], "r");
            if (input_stream == NULL)
                fprintf(stderr,
                        "Error opening input file: %s\n",
                        argv[file_idx]);
            else
            {
                words_length += analyse(input_stream, &dict);
                fclose(input_stream);
            }
        }
    }
    // Print word counts
    print_result(&dict, words_length);
    return return_value;
}