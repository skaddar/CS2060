//
// Name: Soliman Kaddar
// Class: CS2060 (M/W)
// Guided Exploration 07
// Due:14 April 2023
// 
// Description: 
// 

#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SIZE 80

typedef struct pet {

    char name[80];
    unsigned int age;
    Pet *petPtr
}Pet;




int main(void)
{


    return 0;
}



int compareStrings(const char* str1, const char* str2) {


    int str1Len = strlen(str1);
    int str2Len = strlen(str2);

    char str1Lower[SIZE] = "";
    char str2Lower[SIZE] = "";

    for (int i = 0; i < str1Len; i++) {
       str1Lower[i] = tolower(str1[i]);
    }
    str1Lower[str1Len] = '\0';

    for (int i = 0; i < str2Len; i++) {
        str2Lower[i] = tolower(str2[i]);
    }
    str2Lower[str2Len] = '\0';

    int result = strcmp(str1Lower, str2Lower);

    if (result < 0) {

        result = -1;
    }
    if(result>0) {

        result = 1;
    }

    return result;
}
