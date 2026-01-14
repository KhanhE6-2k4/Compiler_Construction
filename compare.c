#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("compare: no input file name\n");
        return 1;
    }

    char file1_path[256];
    char file2_path[256];
    snprintf(file1_path, sizeof(file1_path), "%s", argv[1]);
    snprintf(file2_path, sizeof(file2_path), "./tests/%s", argv[1]);

    FILE *file1 = fopen(file1_path, "rb");
    if (file1 == NULL) {
        printf("Error opening file 1: %s\n", file1_path);
        return 1;
    }

    FILE *file2 = fopen(file2_path, "rb");
    if (file2 == NULL) {
        printf("Error opening file 2: %s\n", file2_path);
        fclose(file1);
        return 1;
    }

    // Get file sizes
    fseek(file1, 0, SEEK_END);
    long size1 = ftell(file1);
    fseek(file1, 0, SEEK_SET);

    fseek(file2, 0, SEEK_END);
    long size2 = ftell(file2);
    fseek(file2, 0, SEEK_SET);

    // Compare file sizes
    if (size1 != size2) {
        printf("Files differ in size: %s (%ld bytes) vs %s (%ld bytes)\n", 
               file1_path, size1, file2_path, size2);
        fclose(file1);
        fclose(file2);
        return 1;
    }

    int byte_count = 0;
    while (!feof(file2) && !feof(file1)) {
        unsigned char byte1 = fgetc(file1);
        unsigned char byte2 = fgetc(file2);
        byte_count++;

        if (byte1 != byte2) {
            printf("Files differ at byte %d\n", byte_count);
            fclose(file1);
            fclose(file2);
            return 1;
        }
    }

    printf("Files are identical with %ld bytes compared\n", size1);
    fclose(file1);
    fclose(file2);
    return 0;
}
