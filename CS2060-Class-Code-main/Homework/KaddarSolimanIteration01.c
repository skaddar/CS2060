//
// Name: Soliman Kaddar
// Class: CS2060 (M/W)
// Iteration 01 Implementation
// Due:4 April 2023
// 
// Description:
// 
// 

#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>


#define SIZE 80
const char urlFirstPart[] = "https:donate.com/";
const char urlSecondPart[] = "?form=popup#";

typedef struct organization {

    char organizationName[80];
    char bossName[80];
    char purpose[80];
    char email[80];
    char password[80];
    char organizationUrl[]
    double goal;

}Organization;

char* custom_fgets(char*, size_t, FILE*);
bool getValidDouble(double*);
void setUpOrganization(Organization*);
void displayOrganization(Organization);
char* createUrl(Organization*);


int main(void)
{

    Organization org1;
    setUpOrganization(&org1);
    char* url = createUrl(&org1);
 


	return 0;
}

char* custom_fgets(char* stringPtr, size_t maxNumOfChars, FILE* inputStream) 
{
    char* input = fgets(stringPtr, maxNumOfChars, inputStream);
    size_t length = strlen(stringPtr);

    if (input != NULL) 
    {
        if (length > 0)
        {
            if (stringPtr[length - 1] == '\n')
            {
                stringPtr[length - 1] = '\0';
            }
        }
    }
    return input;
}

bool getValidDouble(double* validDouble) 
{
    char* end;
    errno = 0;
    bool gotValid = false;
    double doubleTest = 0;
    char inputStr[SIZE];

    do
    {
        printf("%s", "Enter a positive number:");
        custom_fgets(inputStr, SIZE, stdin);

        double doubleTest = strtod(inputStr, &end, 10);

        if (end == inputStr) {
            printf("%s: not a decimal number\n", inputStr);
        }
        else if ('\0' != *end) {
            printf("%s: extra characters at end of input: %s\n", inputStr, end);
        }
        else {
            //the long is explicitly casted to an int and stored in validInt
            *validDouble = doubleTest;
            gotValid = true;
        }
    } while (gotValid != true);
}


void setUpOrganization(Organization* organization)
{
    printf("%s\n", "Enter a name for the organization:");
    custom_fgets(organization->organizationName, SIZE, stdin);

    printf("%s\n", "Enter the purpose of the organization:");
    custom_fgets(organization->purpose, SIZE, stdin);

    printf("%s\n", "Enter First and Last Name:");
    custom_fgets(organization->bossName, SIZE, stdin);

    printf("%s\n", "Enter goal amount:");
    getValidDouble(&(organization->goal));

    printf("%s\n", "Enter email address:");
    custom_fgets(organization->email, SIZE, stdin);
    
    printf("%s\n", "Enter password:");
    custom_fgets(organization->password, SIZE, stdin);
}

char* createUrl(Organization* org) 
{
    const char str1[] = "https:donate.com/";
    const char str2[] = "?form=popup#";
    int length = strlen(str1) + strlen(str2) + strlen(&(org->organizationName)) + 1;

    char result[]
        strcpy(result, str1);
        strcat(result, &(org->organizationName));
        strcat(result, str2);
        return result;

}


void displayOrganization(Organization org)
{
    printf("%s%s\n\n", "Organization Name: ", org.organizationName);

    printf("%s%s\n\n", "Organization Purpose: ", org.purpose);

    printf("%s%s\n\n", "Owner name: ", org.bossName);

    printf("%s%.2lf\n\n", "Organization goal: ", org.goal);
}
