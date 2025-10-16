#include "file_read.h"

int split_text(data_text *data){
    assert(data->buffer.text != NULL);

    char *text = data->buffer.text;
    data->text.lines = (char **)calloc(data->text.lines_count, sizeof(char *));
    if (data->text.lines == NULL) return -1;

    for (int text_position = 0; text_position < data->text.lines_count; text_position++){
        data->text.lines[text_position] = text;

        text = strchr(text, '\n');
        if  (text == NULL) break;

        *text = '\0';
        text++;
    }
    free(text);
    return 1;
}

int find_len_text_lines(data_text data){
    assert(data.buffer.text != NULL);

    int line_count = 0;
    while (true){
        data.buffer.text = strchr(data.buffer.text, '\n');
        if (data.buffer.text == NULL) break;
        data.buffer.text++;
        line_count++;
    }
    return line_count + 1;
} 

int read_file(const char * file_name, data_text *data){
    assert(file_name != NULL);
    assert(data != NULL);

    size_t file_len = find_file_size(file_name) / sizeof(char) + 1;

    data->buffer.text = (char *)calloc(file_len, sizeof(char));
    if (data->buffer.text == NULL)  return -1;

    int file_descriptor = open(file_name, O_RDONLY);
    
    if (file_descriptor == -1){
        close(file_descriptor);
        return 0;
    }

    data->buffer.size = read(file_descriptor, data->buffer.text, find_file_size(file_name));

    close(file_descriptor);

    if (data->buffer.size == -1) return -1;

    data->buffer.text[data->buffer.size] = '\0';
    return 0;
} 

off_t find_file_size(const char * file_name){
    assert(file_name != NULL);
    struct stat file_stat;

    if (stat(file_name, &file_stat) == -1) return 0;

    return file_stat.st_size;
}

bool open_file(data_text *data, const char *file_name){
    read_file(file_name, data);
    if (data->buffer.text == NULL) return false;

    data->text.lines_count = find_len_text_lines(*data);

    if (data->text.lines_count == 0) return false;

    split_text(data);
    if (data->text.lines == NULL) return false;

    return true;
}

data_text init_data_text(){
    data_text program = {
        .buffer = {
            .size = 0,
            .text = NULL,
        },
        .text = {
            .lines_count = 0,
            .lines = NULL,
        },
    };
    return program;
}