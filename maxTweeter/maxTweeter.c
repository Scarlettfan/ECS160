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

int getContentIndex(char *header, char *str)
{
    char *word = Mystrtok(header, ",");
    int index = 0;
    while (word != NULL)
    {
        // if find the col position, break
        if (!strcmp(str, word))
        {
            return index;
        }
        word = Mystrtok(NULL, ",");
        index++;
    }
    return index;
}
int isContain(char *names)
{
    if (names)
    {

        // printf("contains: %s", names);
        for (int i = 0; i < iterator; i++)
        {
            if (!strcmp(nameTweetCount[i].name, names))
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
            printf("name: %s        count: %d\n", list[i].name, list[i].tweetsCount);
    }
}

int main(int argc, const char *argv[])
{
    //check for input
    if (argc != 2)
    {
        printf("Invalid Input Format");
        exit(0);
    }
    //read files
    FILE *stream = fopen(argv[1], "r");

    //line: to store each line
    //nameTweetCount: store tweeters and their tweets count, unsorted
    char line[numChar];

    //getting the header line
    fgets(line, numChar, stream);

    /************************* check headers *****************************/
    /************************* check headers *****************************/
    /************************* check headers *****************************/

    int namePos = getContentIndex(strdup(line), "name");
    int textPos = getContentIndex(strdup(line), "text");

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
            int validText = 0;
            if (count == namePos)
            {

                //check dups
                //if name has been inserted, return the idex in nameTweetCount
                //if its a new name, return -1
                if (token)
                {
                    //************************** check contains quotation marks, if does, remove****************/
                    //************************** check contains quotation marks, if does, remove****************/
                    //************************** check contains quotation marks, if does, remove****************/
                    //************************** check contains quotation marks, if does, remove****************/

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
                    printf("empty name field");
                    return -1;
                }
            }
            /************************** check text field?? ****************************/
            /************************** check text field?? ****************************/
            /************************** check text field?? ****************************/

            // if (count == textPos)
            // {
            //     //check valid text conetent
            //     if (token)
            //     {
            //         validText = isTextValid(token);
            //     }
            //     else
            //     {
            //         printf("empty text field");
            //         return -1;
            //     }
            // }

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
