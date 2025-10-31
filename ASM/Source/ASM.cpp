#include "asm.h"

static void init_labels_value(int *labels, int size);

int main(int argc, char *argv[]){
    const char *file_name     = "";
    const char *file_out_name = "";
    check_argument(argc, argv, &file_name, &file_out_name);

    data_text program = init_data_text();

    if (!open_file(&program, file_name)){
        printf("File was not opened!\n");
        return 1;
    }

    bytecode buffer = {};
    init_code(&buffer);

    labels labels_arr = {
        .labels_value = {},
        .all_labels_added = true,
        .labels = {
            .capacity   = minLabelsLen,
            .size       = 0,
            .data = (int *)calloc(minLabelsLen, sizeof(int)),
        },
    };

    init_labels_value(labels_arr.labels_value, leabelNum);
    
    if (run_commands(&buffer, &program, file_name, &labels_arr)){
        printf("Compilation was not finished!\n");
        return 1;
    }

    FILE *stream = fopen(file_out_name, "w");

    if (stream == NULL){
        printf("Output file was not opened!\n");
        return 1;
    }

    fprintf(stream, "%d ", buffer.size);
    for (int cmd_pos = 0; cmd_pos < buffer.size; cmd_pos++){
        fprintf(stream, "%d ", buffer.data[cmd_pos]);
    }
    fclose(stream);

    printf("COMPLETED\n");
    return 0;
}

static void init_labels_value(int *labels, int size){
    for(int i = 0; i < size; i++) {
        labels[i] = -1;
    }
}

int init_code(bytecode *data){
    if (data == NULL) return 1;

    *data = {
        .size = 0,
        .capacity = kStartCodeSize,
        .data = (int *)calloc(kStartCodeSize, sizeof(int))
    };

    if (data->data == NULL) return 1;
    return 0;
}

void check_argument(int argc, char **argv, const char **file_name, const char **file_out_name){  
    if (argc > 1){
        *file_name = argv[1];
    }
    else{
        *file_name = "source.asm";
    }
    
    if (argc > 2){
        *file_out_name = argv[2];
    } 
    else{
        *file_out_name = "byte_code";
    }
}