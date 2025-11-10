//Name: sarang krishna
//Title: Mp3 tag reader project
//Date: 4/09/25
//Description: Implementing an application to tags from given mp3 files using c functions.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Function prototyping.
int view_mp3();
int fun_for_storing(char*, char*);
void swapBytes(void *, size_t);
int edit_mp3(char *field, char *newValue, char *original_fname);
uint32_t swapEndian(uint32_t num);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage:\n");
        printf("  %s -v\n", argv[0]);
        printf("  %s -e <field> <value>\n", argv[0]);
        return 0;
    }

    if (argv[1][1] == 'v')
        view_mp3();
    else if(argv[1][1] == 'h'){
    printf("\n.............Help Menu.....................\n\n");
    printf("1. -v -> to view mp3 file contents\n");
    printf("2. -e -> to edit mp3 file contents\n");
    printf(" 2.1. -t -> to edit song title\n");
    printf(" 2.2. -A -> to edit artist name\n");
    printf(" 2.3. -a -> to edit album name\n");
    printf(" 2.4. -y -> to edit song year\n");
    printf(" 2.5. -m -> to edit song content\n");
    printf(" 2.6. -c -> to edit song comment\n");
    printf("\n............................................\n\n");

    }
    else if (argv[1][1] == 'e') {
        if (argc < 4) {
            printf("❌ Missing arguments for edit.\n");
            return 0;
        }
        edit_mp3(argv[2], argv[3], argv[4]);
    }
    else
        printf("❗WRONG INPUT❗\n");

    return 0;
}

// Function to view the tags.
int view_mp3() {
    int n = 0;
    FILE* fptr = fopen("sample.mp3", "rb");
    if (fptr == NULL) {
        printf("❌ File can't be opened. ❌\n");
        return 0;
    }

    unsigned char buffer[4] = {0};

    if (fread(buffer, 1, 3, fptr) != 3) {
        printf("❌ Failed to read file.\n");
        fclose(fptr);
        return 0;
    }

    buffer[3] = '\0';

    if (strcmp((char *)buffer, "ID3") != 0) {
        printf("❌ The file is not MP3. ❌\n");
        fclose(fptr);
        return 0;
    }

    // Skip ID3 header (10 bytes)
    fseek(fptr, 10, SEEK_SET);

    printf("-------------------------------------------------------------------------------------------\n");
    while (n <= 6) {
        unsigned char tag[5] = {0};
        if (fread(tag, 1, 4, fptr) != 4) break;  // no more tags
        tag[4] = '\0';

        int size;
        if (fread(&size, 1, 4, fptr) != 4) break;
        swapBytes(&size, sizeof(size));

        unsigned char flags[2];
        fread(flags, 1, 2, fptr);

        if (size <= 0) break;

        unsigned char *info = malloc(size + 1);
        fread(info, 1, size, fptr);
        info[size] = '\0';

        // Skip encoding byte (first byte of text frame)
        char *text = (char*)(info + 1);

        n += fun_for_storing((char*)tag, text);
        free(info);
    }
    printf("-------------------------------------------------------------------------------------------\n");
    fclose(fptr);
    return 0;
}

//Function for storing
int fun_for_storing(char* tag, char* info) {
    if (info == NULL || strlen(info) == 0) {
        return 0;  // nothing to print
    }

    if (!strcmp(tag, "TIT2")) {
        printf("Title    : %s\n", info);
        return 1;
    }
    else if (!strcmp(tag, "TYER")) {
        printf("Year     : %s\n", info);
        return 1;
    }
    else if (!strcmp(tag, "TALB")) {
        printf("Album    : %s\n", info);
        return 1;
    }
    else if (!strcmp(tag, "TPE1")) {
        printf("Artist   : %s\n", info);
        return 1;
    }
    else if (!strcmp(tag, "COMM")) {
        printf("Comments : %s\n", info);
        return 1;
    }
    else if (!strcmp(tag, "TCON")) {
        printf("Genre    : %s\n", info);
        return 1;
    }
    else {
        return 0;  // unrecognized frame
    }
}

// Function to change endianness
void swapBytes(void *value, size_t size) {
    unsigned char *ptr = (unsigned char*)value;
    for (size_t i = 0; i < size / 2; i++) {
        unsigned char temp = ptr[i];
        ptr[i] = ptr[size - 1 - i];
        ptr[size - 1 - i] = temp;
    }
}
//Function to edit
int edit_mp3(char *field, char *newValue, char* original_fname) {
    FILE* fptr1 = fopen(original_fname, "r");
    FILE* fptr2 = fopen("output.mp3", "w");

    if (fptr1 == NULL || fptr2 == NULL) {
        printf("❌File can't be opened.❌\n");
        return 0;
    }

    unsigned char buffer[10];
    fread(buffer, 1, 10, fptr1);
    fwrite(buffer, 1, 10, fptr2);

    unsigned char tag[5];
    while (fread(tag, 1, 4, fptr1) == 4) 
    {
        tag[4] = '\0';
        fwrite(tag, 1, 4, fptr2);

        int size;
        fread(&size, 4, 1, fptr1);
        swapBytes(&size, sizeof(size));

        unsigned char flag[3];
        fread(flag, 1, 3, fptr1);

        char info[size-1];
        fread(info, 1, size-1, fptr1);

        if ((strcmp(field, "-t") == 0 && !strcmp((char*)tag, "TIT2")) ||
            (strcmp(field, "-a") == 0 && !strcmp((char*)tag, "TPE1")) ||
            (strcmp(field, "-y") == 0 && !strcmp((char*)tag, "TYER")) ||
            (strcmp(field, "-A") == 0 && !strcmp((char*)tag, "TALB")) ||
            (strcmp(field, "-C") == 0 && !strcmp((char*)tag, "COMM")) ||
            (strcmp(field, "-g") == 0 && !strcmp((char*)tag, "TCON"))) 
        {

            int n = strlen(newValue) + 1;
            int newSize = swapEndian((uint32_t)n);
            fwrite(&newSize, 4, 1, fptr2);
            fwrite(flag, 1, 3, fptr2);
            fwrite(newValue, 1, n - 1, fptr2);
            printf("✅ File copied successfully from sample.mp3 to output.mp3\n");

            unsigned char ch[100];
            int s=100;
            while(s == 100)
            {
                s = fread(ch, 1, 100, fptr1);
                fwrite(ch, 1, s, fptr2);
            }

            break;
        }
        else 
        {
            int origSize = swapEndian((uint32_t)size);
            fwrite(&origSize, 4, 1, fptr2);
            fwrite(flag, 1, 3, fptr2);
            // unsigned char *info = malloc(size);
            
            fwrite(info, 1, size-1, fptr2);
            // free(info);
        }
    }

    printf("✅ File copied successfully from sample.mp3 to output.mp3\n");

    fclose(fptr1);
    fclose(fptr2);

    remove(original_fname);
    rename("output.mp3", original_fname);
    return 1;
}



//changing endianess
uint32_t swapEndian(uint32_t num) {
    return ((num >> 24) & 0x000000FF) | 
           ((num >> 8)  & 0x0000FF00) | 
           ((num << 8)  & 0x00FF0000) | 
           ((num << 24) & 0xFF000000);
}
