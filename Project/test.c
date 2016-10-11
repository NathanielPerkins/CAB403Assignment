#include "fileControl.h"

int main(){
    const char *file_location = "Database/Accounts.txt";
    uint16_t lines = get_file_dimensions(file_location);
    printf("get_file_dimensions: ");
    printf("%s\n", lines ? "Succeeded": "Failed");
    printf("Lines: %d\n",lines);
}
