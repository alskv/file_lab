//testing string:./search/ just KEY aTtAcK
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#define MAX_LEN 50

int open_files(char *name, char *way, char *argv[], int argc);
int search_word(char word[MAX_LEN], FILE *file);

// command line arguments take search keywords
int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Error: enter search words, e.g.: ./main <YOUR_PATH> <YOUR_WORD>\n");
        return 1;
    }

    for (int i = 2; i < argc; i++)
    {
        int j = 0;
        while (argv[i][j] != '\0')
        {
            if (!(isalpha(argv[i][j])))
            {
                fprintf(stderr, "Error: entered strings are not words\n");
                return 1;
            }
            // make the same register for correct comparison
            else
                argv[i][j] = tolower(argv[i][j]);
            j++;
        }
    }

    DIR *dir;
    struct dirent *entry;

    // working directory with files
    char *dirinput = argv[1];
    dir = opendir(dirinput);
    if (dir == NULL)
    {
        perror("Error opening directory");
        return 1;
    }

    while ( (entry = readdir(dir)) != NULL)
    {
        char *name = entry->d_name;
        if ((strcmp(name,"..") == 0) || (strcmp(name,".") == 0))
            continue;

        char *way = argv[1];
        char way_copy[MAX_LEN];
        memcpy(way_copy,way,MAX_LEN);

        if (open_files(name, way_copy, (char **)argv, argc))
        {
            perror("Error opening file");
            return 1;
        }
        memset(way_copy, 0, MAX_LEN);
    }

    closedir(dir);
    return 0;
}


int open_files(char *name, char *way, char *argv[], int argc)
{
    //file path creation
    strcat(way,name);
    FILE *file;
    file = fopen(way,"r");

    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    for (int j = 2; j < argc; j++)
    {
        if (!(search_word(argv[j], file)))
        {
            printf("Path to the file with the word [%s]: %s\n", argv[j], way);
        }
    }

    fclose(file);
    return 0;
}

int search_word(char word[MAX_LEN], FILE *file)
{
    char sym;
    char fileword[MAX_LEN] = {0};

    int j = 0;
    // for each next keyword put a pointer to beginning of the file
    fseek(file, 0, SEEK_SET);
    while ((sym=getc(file)) != EOF)
    {
        if (isalpha(sym))
        {
            fileword[j]=tolower(sym);
            j++;
        }

        if (!isalpha(sym) && !strcmp(word, fileword))
            return 0;
        if (!isalpha(sym) && strcmp(word, fileword))
        {
            memset(fileword, 0, MAX_LEN);
            j=0;
        }
    }
    if (!strcmp(word, fileword))
        return 0;
    return 1;
}
