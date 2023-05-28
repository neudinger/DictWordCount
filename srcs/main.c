/*
 * File main.c
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