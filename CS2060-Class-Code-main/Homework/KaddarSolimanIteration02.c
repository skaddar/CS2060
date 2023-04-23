//
// Name: Soliman Kaddar
// Class: CS2060 (M/W)
// Iteration 02 Implementation
// Due:25 April 2023
// 
// Description:
// 
// 
// 
// 
//


#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>


#define SIZE 80
#define URL_PARTS_LENGTH 30
#define ADMIN_PIN 'Q'
#define ZIPCODE_LENGTH 5
#define CARD_PROCESS_FEE 0.031
const char urlFirstPart[] = "https:donate.com/";
const char urlSecondPart[] = "?form=popup#";
const char fileReceipt[] = "-receipt.txt";

//structure for the fundraisers 
typedef struct organization {

    char organizationName[80];
    char bossName[80];
    char purpose[80];
    char email[80];
    char password[80];
    char organizationUrl[URL_PARTS_LENGTH + SIZE];
    double goal;
    double amountRaised;
    unsigned int numOfDonors;
    double creditCardFees;

    struct organization* nextOrgPtr;

}Organization;


char* custom_fgets(char*, int, FILE*);
void setUpOrganization(Organization**);
int askForInsert();
int compareStrings(const char* , const char*);
void getValidDouble(double* validDouble);
void displayOrgs(Organization* );
void addOrgToList(Organization** , Organization* );
void createUrl(char*);
char* removeSpaces(char*, const char*);
void displayOrganizationUrl(const Organization);


int main(void)
{


    printf("%s\n", "---------------------------------------------------------");
    printf("%s\n", "              Set Up Fundraiser");
    printf("%s\n\n", "---------------------------------------------------------");

    Organization* headPtr = NULL;
    setUpOrganization(&headPtr);
    bool finishAdding = false;

    do 
    {
        int choice = askForInsert();

        if (choice == 1) 
        {
            setUpOrganization(&headPtr);
        }
        else 
        {
            finishAdding = true;
        }

    } while (finishAdding == false);

    displayOrgs(headPtr);


    return 0;
}


//Same parameters as fgets and returns a string 
//This function will remove the new line character from an input and return the updated input
char* custom_fgets(char* stringPtr, int maxNumOfChars, FILE* inputStream)
{
    //regular fgets with the new line character
    //Get the length of the input string and make sure
    char* input = fgets(stringPtr, maxNumOfChars, inputStream);
    size_t length = strlen(stringPtr);

    //make sure the input is not null and that the user actually entered something then
    //replace newline with null
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
}//-----------------------------custom fgets------------------------------------------------


int askForInsert()
{

    char inputStr[SIZE] = "";
    bool gotValid = false;
    int result = 0;


    printf("%s\n", "Do you want to insert another organization? (y)es or (n)o?");
    do
    {
        custom_fgets(inputStr, SIZE, stdin);

        //Make sure input is one character

        if (strlen(inputStr) <= 1)
        {
            //Make sure that character can be lower or uppercase and is N or Y
            if (toupper(inputStr[0]) == 'Y' || toupper(inputStr[0]) == 'N')
            {
                gotValid = true;
            }
            else
            {
                printf("%s\n", "You did not enter a y or n.");
            }
        }
        else
        {
            printf("%s\n", "You did not enter a y or n.");
        }
    } while (gotValid == false);

    //if Y is entered print receipt
    //if N is entered exit function
    if (toupper(inputStr[0]) == 'Y') {

        result = 1;
    }
    if (toupper(inputStr[0]) == 'N')
    {
        result = 0;

    }

    return result;
}//------------------------------------end ask for insert-----------------------------------------

//initializes all of the organization data members 
//no return the paramter is just the organization passed by reference using a pointer
void setUpOrganization(Organization** head)
{
    Organization* newOrg = malloc(sizeof(Organization));

    if (newOrg != NULL)
    {
        newOrg->goal = 0;
        newOrg->amountRaised = 0;
        newOrg->numOfDonors = 0;
        newOrg->creditCardFees = 0;
        newOrg->nextOrgPtr = NULL;

        printf("%s", "Enter a name for the organization:");
        custom_fgets(newOrg->organizationName, SIZE, stdin);

        printf("%s", "Enter the purpose of the organization:");
        custom_fgets(newOrg->purpose, SIZE, stdin);

        printf("%s", "Enter First and Last Name:");
        custom_fgets(newOrg->bossName, SIZE, stdin);

        printf("%s", "Enter goal amount:");
        getValidDouble(&(newOrg->goal));

        printf("%s\n", "Enter email address:");
        custom_fgets(newOrg->email, SIZE, stdin);

        printf("%s\n", "Enter password:");
        custom_fgets(newOrg->password, SIZE, stdin);

        createUrl(newOrg->organizationName);
        displayOrganizationUrl(*newOrg);

        addOrgToList(&(*head), newOrg);
    }
    else
    {
        printf("%s\n", "No memory to create node");
    }

}//------------------------------------end setUpOrganization-----------------------------------------

void getValidDouble(double* validDouble)
{
    //the end pointer will store the last char in the string
    //errno set to 0 but will change if error is encountered
    //got valid is a boolean flag
    //double test will store what strtod will output
    char* end;
    errno = 0;
    bool gotValid = false;
    double doubleTest = 0;
    char inputStr[SIZE];

    //will loop until string can be succesfulyy converted to a double
    do
    {
        custom_fgets(inputStr, SIZE, stdin);
        double doubleTest = strtod(inputStr, &end);

        //first Make sure the end has moved meaning it read some values
        //then check that end is null because that means it read all values
        //if it passes those tests then update the double and exit loop
        if (end != inputStr) {
            
            if ('\0' == *end) {

                if (doubleTest > 0) 
                {
                    *validDouble = doubleTest;
                    gotValid = true;
                }
                else{
                    puts("number must be greater than 0");
                } 
            }
            else{
                printf("%s: extra characters at end of input: %s\n", inputStr, end);
            }
        }
        else{
            printf("%s: not a decimal number\n", inputStr);
        }
     
    } while (gotValid != true);
}//------------------------------------end getValidDouble--------------------------------

int compareStrings(const char* str1, const char* str2) {

    int result = 0;

    if (str1 != NULL && str2 != NULL)
    {
        size_t str1Len = strlen(str1);
        size_t str2Len = strlen(str2);

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

        result = strcmp(str1Lower, str2Lower);

        if (result < 0) {

            result = -1;
        }
        if (result > 0) {

            result = 1;
        }
    }
    return result;
}//String compare method -----------------------------------------------------------------------------


void addOrgToList(Organization** head, Organization* orgToAdd) 
{
    Organization* previousOrgPtr = NULL;
    Organization* currentOrgPtr = *head;


        while (currentOrgPtr != NULL && compareStrings(currentOrgPtr->organizationName, orgToAdd->organizationName) == 1)
        {
            previousOrgPtr = currentOrgPtr;

            currentOrgPtr = currentOrgPtr->nextOrgPtr;
        }

        if (previousOrgPtr == NULL)
        {
            *head = orgToAdd;
        }
        else
        {
            previousOrgPtr->nextOrgPtr = orgToAdd;
        }

        orgToAdd->nextOrgPtr = currentOrgPtr;

}

void displayOrgs(Organization* headPtr)
{
    if (headPtr == NULL)
    {
        puts("No orgs in list");
    }
    else
    {
        puts("\nSelect the organization you would like to make a donation..");
        puts("\nOrganization Name\tgoal\tAmount Raised\n");
        Organization* currentOrg = NULL;
        currentOrg = headPtr;
        while (currentOrg != NULL)
        {
            printf("%s\t\t$%.2lf\t\t$%.2lf\n", currentOrg->organizationName,currentOrg->goal, currentOrg->amountRaised);
            currentOrg = currentOrg->nextOrgPtr;
        }
        
    }
}

//returns a string with no spaces. parameters is the organization passed as a constant and a string to store
//the string without the spaces
char* removeSpaces(char* name, const char* organizationName)
{
    //get the name of the organization and store the string in name
    //Then loop through all of the characters in the string and replace any spaces with a dash
    strcpy(name, organizationName);
    for (int i = 0; i < strlen(organizationName); i++)
    {
        if (name[i] == ' ')
        {
            name[i] = '-';
        }
    }
    return name;
}//------------------------------------end remove spaces-----------------------------------------

//uses the organization name and the url member to store the URL
//will call the remove spaces method and use strcat to put the prefix and suffix of the URL
void createUrl(char* organizationUrl)
{
    //will store the return of remove spaces
    char nameWithNoSpaces[SIZE] = "";
    strcpy(organizationUrl, urlFirstPart);

    removeSpaces(nameWithNoSpaces, organizationUrl);

    strcat((organizationUrl), nameWithNoSpaces);
    strcat(organizationUrl, urlSecondPart);

}//------------------------------------end createUrl-----------------------------------------

void displayOrganizationUrl(const Organization org)
{
    //�Thank you [first and last name]. The url to
    //raise funds for [Organization] is  https:donate.com/[name-of-organization]?form=popup#.�
    printf("%s%s. ", "Thank you ", org.bossName);

    printf("%s%s%s\n", "The url to raise funds for ", org.organizationName, " is: ");

    printf("%s\n\n", org.organizationUrl);

}