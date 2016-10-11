#include "fileControl.h"

int16_t get_file_dimensions(const char *file_location){
    FILE *fp;
    int8_t current_char;
    int16_t lines = 0;
    fp = fopen(file_location,"r");
    if (fp == NULL) {
        printf("Couldn't open file: %s",file_location);
        return -1;
    }
    do {
        current_char = fgetc(fp);
        if (current_char == '\n'){
            lines++;
        }
    } while(!feof(fp));

    fclose(fp);
    return lines;
}
