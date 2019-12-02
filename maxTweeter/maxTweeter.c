//
//  maxTweeters.c
//  maxTweeter
//
//  Created by Scarlett Fan on 11/25/19.
//  Copyright © 2019 Scarlett Fan. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <stdarg.h>
#define numLine 20000
#define numChar 1024

struct nameTweets
{
    char *name;
    int tweetsCount;
};
typedef struct nameTweets nameTweetsPair;

nameTweetsPair nameTweetCount[numLine];
//interator: counts the number of nametweets in the nameTweetCount
int iterator = 0;

//code from https://www.geeksforgeeks.org/bubble-sort/
void swap(nameTweetsPair *xp, nameTweetsPair *yp)
{
    nameTweetsPair temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void Mysort(void)
{
    int i, j;
    for (i = 0; i < iterator - 1; i++)

        // Last i elements are already in place
        for (j = 0; j < iterator - i - 1; j++)
            if (nameTweetCount[j].tweetsCount < nameTweetCount[j + 1].tweetsCount)
                swap(&nameTweetCount[j], &nameTweetCount[j + 1]);
}

//to seperate the form of "**,,**"
char* Mystrtok(char *string, char *seperator)
{
    static char *source = 0;
    char *p, *token = 0;

    if (string)
        source = string;
    if (!source)
        return 0;
    //returns first encountered ',' + the res of the string
    p = strpbrk(source, seperator);
    if (p)
    {
        *p = 0;
        token = source;
        source = ++p;
    }
    else
    {
        token = source;
        source = NULL;
    }

    return token;
}

int useQuote(char *token)
{
    //using quote
    if (token[0] == '"' && token[strlen(token) - 1] == '"')
    {
        return 1;
    }
    //not using quote
    else if (token[0] != '"' && token[strlen(token) - 1] != '"')
    {
        return 0;
    }
    //invalid
    else if (token[0] == '"' && token[strlen(token) - 1] != '"')
    {
        return -1;
    }
    //invalid
    else if (token[0] != '"' && token[strlen(token) - 1] == '"')
    {
        return -1;
    }
    return -1;
}
char *removeQuote(char *name)
{
    char *token = name;
    //printf("token: %s", token);
    if (token != NULL)
    {
        if (token[0] == '"' && token[strlen(token) - 1] == '"')
        {
            ++token;
            token[strlen(token) - 1] = '\0';
        }
        else
        {
            return name;
        }
    }
    return token;
}

int getContentIndex(char *header, int *hasQuotes)
{
    char *word = Mystrtok(header, ",");
    int index = 0;
    while (word != NULL)
    {
        if (useQuote(word))
        {
            *hasQuotes = 1;
        }
        else if (!useQuote(word))
        {
            *hasQuotes = 0;
        }
        // if find the col position, break
        if (*hasQuotes == 0 && strlen(word) == strlen("name") && !strcmp(word, "name"))
            return index;
        if (*hasQuotes == 1 && strlen(word) == strlen("\"name\"") && !strcmp(word, "\"name\""))
            return index;
        word = Mystrtok(NULL, ",");
        index++;
    }
    return -1;
}
int isContain(char *names)
{
    if (names)
    {
        for (int i = 0; i < iterator; i++)
        {
            if (strlen(nameTweetCount[i].name) == strlen(names) && !strcmp(nameTweetCount[i].name, names))
            {
                return i;
            }
        }
    }

    return -1;
}

bool isTextValid(char *text)
{
    if (text)
    {
        return true;
    }
    return false;
}

void printData(nameTweetsPair *list)
{
    for (int i = 0; i < 10; i++)
    {
        if (list[i].name && list[i].tweetsCount)
            printf("%s: %d\n", list[i].name, list[i].tweetsCount);
    }
}

int nextComma(char* r, int len, int s)
{
    if (s >= len)
        return -1;
    int i;
    for(i=s;i<len;i++) {
        if (r[i] == ',')
            return i;
    }
    return i;
}

char* strip(char *s, int l)
{
    for(int i=l-1;i>=0;i--)
    {
        if (s[i] == ' ' || s[i] == '\n' || s[i] == '\r')
            s[i] = '\0';
    }
    while (s[0] == ' ' || s[0] == '\n' || s[0] == '\r')
        s++;
    return s;
}
int parseRow(char* row, int len, char* cols[],int qtd[], int* size)
{
    *size = 0;
    int i = 0;
    int c;

    do {
        c = nextComma(row, len, i);
        char* cur_col = strndup(row + i,c-i);
        cur_col = strip(cur_col,strlen(cur_col));
        int isqtd = 0;
        if (cur_col[0] == '\"' && cur_col[strlen(cur_col) - 1] == '\"') {
            cur_col = removeQuote(cur_col);
            isqtd = 1;
        }
        qtd[(*size)] = isqtd;
        cols[(*size)++] = cur_col;
        i = c + 1;
    }while(c < len);
    return 1;
}

int findIndex(char* name, char* cols[], int qtd[], int size)
{
    for (int i=0;i<size;i++) {
        if (strcmp(name,cols[i]) == 0) {
            return i;
        }
    }
    return -1;
}

int main(int argc, const char *argv[])
{
    //check for input
    if (argc != 2)
    {
        printf("Invalid Input Format");
        exit(-1);
    }
    //read files
    FILE *stream = fopen(argv[1], "r");

    //line: to store each line
    char line[numChar];
    int nameIdx = -1;

    //getting the header line
    fgets(line, numChar, stream);
    if (strlen(line) <= 0) {
        printf("Invalid Header Format");
        exit(-1);
    }
    char* header[1024];
    int isHeaderQuoted[1024];
    int headerSize;
    parseRow(line,strlen(line),header,isHeaderQuoted,&headerSize);

    /************************* check headers *****************************/

    nameIdx = findIndex("name", header, isHeaderQuoted, headerSize);
    if (nameIdx == -1)
    {
        printf("Invalid Header Format");
        exit(-1);
    }
    //read all the remaining lines
    while (fgets(line, numChar, stream))
    {
        char* cells[1024];
        int isQuoted[1024];
        int rowSize;
        parseRow(line,strlen(line),cells,isQuoted,&rowSize);
        //check valid line
        if (rowSize != headerSize) {
            printf("Invalid Input Format\n");
            exit(-1);
        }
        //when valid
        char* nameField = cells[nameIdx];
        if (strlen(nameField) > 0)
        {
            //************************** check contains quotation marks, if does, remove****************/

            if (isQuoted[nameIdx] != isHeaderQuoted[nameIdx]) {
                //printf("%d, %d, %d\n",nameIdx,isQuoted[nameIdx],isHeaderQuoted[nameIdx]);
                printf("Invalid Input Format\n");
                exit(-1);
            }
            if (isQuoted[nameIdx]) {
                nameField = removeQuote(nameField);
            }

            //check dups
            //if name has been inserted, return the idex in nameTweetCount
            //if its a new name, return -1
            int nameExists = isContain(nameField);
            if (nameExists >= 0)
            {
                //if contains
                //increnment the tweets count of that name
                nameTweetCount[nameExists].tweetsCount += 1;
            }
            else if (nameExists == -1)
            {
                //add the name to the list
                //increment the count
                if (nameField != NULL)
                {
                    nameTweetCount[iterator].name = nameField;
                    nameTweetCount[iterator].tweetsCount = 1;
                    iterator++;
                }
            }
        }
        else
        {
            printf("Invalid name field");
            exit(-1);
        }
    }

    //sort the list
    Mysort();

    //take top ten
    nameTweetsPair topTen[10];
    for (int i = 0; i < 10; i++)
    {
        topTen[i] = nameTweetCount[i];
    }
    printData(topTen);
}
