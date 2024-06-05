#include <stdio.h>

long int calulContenLen(FILE *file) {
    long int size;

    // Save the current position
    long int currentPos = ftell(file);

    // Seek to the end of the file
    if (fseek(file, 0, SEEK_END) == -1) {
        fprintf(stderr, "Error seeking to the end of file\n");
        return -1;
    }

    // Get the position which is the size of the file
    size = ftell(file);
    if (size == -1) {
        fprintf(stderr, "Error getting file size\n");
        return -1;
    }

    // Restore the file position
    if (fseek(file, currentPos, SEEK_SET) == -1) {
        fprintf(stderr, "Error seeking to the beginning of file\n");
        return -1;
    }

    return size;
}

/*int main(int argc, char *argv[]) {*/
/*    if (argc != 2) {*/
/*        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);*/
/*        return 1;*/
/*    }*/
/**/
/*    // Open the file*/
/*    FILE *file = fopen(argv[1], "r");*/
/**/
/**/
/*    // Get the file size*/
/*    long int size = calulContenLen(file);*/
/*    if (size == -1) {*/
/*        fprintf(stderr, "Failed to get file size.\n");*/
/*        fclose(file);*/
/*        return 1;*/
/*    }*/
/**/
/*    printf("Size of file %s: %ld bytes\n", argv[1], size);*/
/**/
/*    // Close the file*/
/*    fclose(file);*/
/**/
/*    return 0;*/
/*}*/
