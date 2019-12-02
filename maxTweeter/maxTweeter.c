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
char *Mystrtok(char *string, char *seperator)
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
    printf("token: %s", token);
    if (token != NULL)
    {
        if (token[0] == '"' && token[strlen(token) - 1] == '"')
        {
            ++token;
            token[strlen(token) - 1] = 0;
        }
        else if (token[0] != '"' && token[strlen(token) - 1] != '"')
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

        // printf("contains: %s", names);
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
    int namePos = 0;
    int headerHasQuotes = 0;

    //getting the header line
    fgets(line, numChar, stream);

    /************************* check headers *****************************/

    /************************* check headers *****************************/
    namePos = getContentIndex(strdup(line), &headerHasQuotes);
    if (namePos == -1)
    {
        printf("Invalid Header Format");
        exit(-1);
    }
    //read all the remaining lines
    while (fgets(line, numChar, stream))
    {

        //check valid line
        //when valid
        char *token = Mystrtok(strdup(line), ",");
        int count = 0;
        while (token)
        {
            int nameExists = -1;
            char **candidate = 0;
            // int validText = 0;
            if (count == namePos)
            {
                if (token)
                {
                    //************************** check contains quotation marks, if does, remove****************/

                    if (headerHasQuotes && (useQuote(token) == 1))
                    {
                        token = removeQuote(token);
                    }
                    else if (headerHasQuotes && (useQuote(token) == 0))
                    {
                        printf("Invalid Name Format");
                    }
                    else if (!headerHasQuotes && (useQuote(token) == 1))
                    {
                        printf("Invalid Name Format");
                    }
                    else if ((useQuote(token) == -1))
                    {
                        printf("Invalid Name Format");
                    }

                    //check dups
                    //if name has been inserted, return the idex in nameTweetCount
                    //if its a new name, return -1
                    nameExists = isContain(token);
                    candidate = &token;
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
                        if (*candidate != NULL)
                        {
                            nameTweetCount[iterator].name = *candidate;
                            nameTweetCount[iterator].tweetsCount = 1;
                            iterator++;
                        }
                    }
                }
                else
                {
                    printf("Invalid name field");
                    return -1;
                }
            }

            token = Mystrtok(NULL, ",");
            ++count;
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
