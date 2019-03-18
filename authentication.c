#include "authentication.h"

const char *delimiter = ":";

unsigned long hash(const char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++) != 0)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

FILE *openFile(const char *fileName, const char *mode)
{
    FILE* fileDescriptor = fopen(fileName, mode);
    if (fileDescriptor == NULL)
    {
        printf("Error opening file %s\n", fileName);
        exit(1);
    }

    return fileDescriptor;
}

bool authenticate(const char *username, const char *password)
{
    char line[100];
    char endOfLineDelimitator[] = "\n";
    bool correctUserName = false;
    bool correctPassword = false;

    FILE *dbFile = openFile(DB_FILE, "r");

    /* read line by line from file */
    while ((fgets(line, sizeof(line), dbFile)) != NULL)
    {
        /* check if the line contains the delimitator, otherwise go to the next line */
        if (strstr(line, delimiter) != NULL)
        {
            /* get the username from the line */
            char *ptr = strtok(line, delimiter);
        
            /* if the username is correct, set the variable to true */
            if (strcmp(ptr, username) == 0)
            {
                correctUserName = true;
            }
            else
            {
                /* if the username is not correct, go to the next line */
                continue;
            }

            if (correctUserName == true)
            {
                /* do hash function for the user's password */
                unsigned long userPassword = hash(password);

                /* get the password from the current line */
                ptr = ptr + strlen(ptr) + strlen(delimiter);
                ptr = strtok(ptr, endOfLineDelimitator);

                /* convert the resulted password to the unsigned long type */
                char *stringPart=(char*)malloc(sizeof(char));
                unsigned long rez= strtoul(ptr, &stringPart, 10);
                   
                if (userPassword == rez)
                {
                    correctPassword = true;
                    break;
                }
            }
        }
        else
        {
            /* go to the next line */
            continue;
        }
    }

    fclose(dbFile);

    return correctUserName && correctPassword;
}

bool registration(const char *username, const char *password)
{
    FILE *dbFile = openFile(DB_FILE, "a");
    
    int charsWritten = fprintf(dbFile, "%s%s%s\n", username, delimiter, password);

    fclose(dbFile);

    return charsWritten > 0;
}

enum MsgType check_msg_type(const char *message)
{
    char type[10], username[30], password[30];
    int n;
    
    if (strncmp(message, "reg:", 4) == 0 || strncmp(message, "login:", 6) == 0) {
        n = sscanf(message, "%[^:]:%[^:]:%[^:]", type, username, password);
        if (n == 3) {
            if (strlen(username) == 0 || strlen(password) == 0)
                return REGULAR;
            if (strcmp(type, "reg") == 0)
                return REGISTER;
            if (strcmp(type, "login") == 0)
                return LOGIN;
        }
    }
    return REGULAR;
}

