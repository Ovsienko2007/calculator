#ifndef FILE_READ_H_
#define FILE_READ_H_

#include <assert.h>
#include <fcntl.h>
#include <sys/stat.h>

struct text_buffer{
    int size;
    char *text;
};

struct lines_data{
    int lines_count;
    char **lines;
};

struct data_text{
    text_buffer buffer;
    lines_data text;
};

/**
 * @brief reads all lines from file write number of lines in text_len
 * 
 * @param [in]   file_name  name of file
 * @param [out]  text_len   number of lines in file
 * 
 * @return string with lines from file, NULL in case of error
 */
int read_file(const char * file_name, data_text *data);

/**
 * @brief finds the size of the given file in bytes
 * 
 * @param [in]   file_name  name of file
 * 
 * @return  size of the given file
 */
off_t find_file_size(const char * file_name);

/**
 * @brief finds the number of lines in the given text
 * 
 * @param [in]   text  text
 * 
 * @return  the number of lines in the given text, 0 in case of error
 */
int find_len_text_lines(data_text data);

/**
 * @brief replaces the first line_count enters of text with \0
 * 
 * @param [out]  text        text
 * @param [in]   line_count  number of lines
 * 
 * @return  array of pointers to the first elements of the rows, NULL in case of error
 */
int split_text(data_text *data);

bool open_file(data_text *data, const char *file_name);

data_text init_data_text();
#endif