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
#define PASSWORD_MINIMUM 7
const char urlFirstPart[] = "https:donate.com/";
const char urlSecondPart[] = "?form=popup#";
const char fundraisersPath[] = "C:\\fundraisers\\";
const char fileReceipt[] = "-receipt.txt";

//structure for the fundraisers 
typedef struct organization {

    char organizationName[80];
    char bossName[80];
    char purpose[80];
    char email[80];
    char password[80];
    char filePath[200];
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
bool getValidDouble(double* validDouble);
void displayOrgs(Organization* );
void addOrgToList(Organization** , Organization* );
void createUrl(Organization*);
char* removeSpaces(char*, const char*);
void displayOrganizationUrl(const Organization);
bool isValidEmail(const char*);
bool isValidPass(const char*, int);
void createReceiptFile(Organization*);
void displayFunds(const Organization*);
Organization* getOrgbyName(Organization*, char*);


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

        printf("%s\n", "Do you want to insert another organization? (y)es or (n)o?");
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


    printf("%s\n", "---------------------------------------------------------");
    printf("%s\n", "                    Donate!");
    printf("%s\n\n", "---------------------------------------------------------");

    //boolean flag will only be set as true when the admin is verified 
    bool finishDonating = false;

    do 
    {
        displayOrgs(headPtr);

        char inputOrgName[] = "";
        custom_fgets(inputOrgName, SIZE, stdin);

        Organization* chosenOrg = getOrgbyName(headPtr, inputOrgName);

        if (chosenOrg != NULL) 
        {
            displayFunds(chosenOrg);
            puts("--------------------------------------------------------------\n");
            puts("Enter fundraiser name: ");


        }
        else 
        {
            puts("No organizations found of that name");
        }

    
    } while (!finishDonating);

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

        int validEmailQuestion = 0;

        do 
        {
            printf("%s\n", "Enter email address:");
            custom_fgets(newOrg->email, SIZE, stdin);

            if (isValidEmail(newOrg->email) == true)
            {
                printf("%s%s%s\n", "Is [",newOrg->email ,"] a valid email? (y)es or (n)o?");
                validEmailQuestion = askForInsert();
            }

        } while (validEmailQuestion == 0);

        int validPassQuestion = 0;

        do
        {
            printf("%s\n", "Enter password:");
            custom_fgets(newOrg->password, SIZE, stdin);

        } while (isValidPass(newOrg->password, PASSWORD_MINIMUM) != true);


        createUrl(newOrg);
        displayOrganizationUrl(*newOrg);

        createReceiptFile(newOrg);

        addOrgToList(&(*head), newOrg);
    }
    else
    {
        printf("%s\n", "No memory to create node");
    }

}//------------------------------------end setUpOrganization-----------------------------------------

bool getValidDouble(double* validDouble)
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

    return gotValid;
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
    for (int i = 0; i < strlen(name); i++)
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
void createUrl(Organization * org)
{
    //will store the return of remove spaces
    char nameWithNoSpaces[SIZE] = "";
    strcpy(org->organizationUrl, urlFirstPart);

    removeSpaces(nameWithNoSpaces, org->organizationName);

    strcat(org->organizationUrl, nameWithNoSpaces);
    strcat(org->organizationUrl, urlSecondPart);

}//------------------------------------end createUrl-----------------------------------------

void displayOrganizationUrl(const Organization org)
{
    //“Thank you [first and last name]. The url to
    //raise funds for [Organization] is  https:donate.com/[name-of-organization]?form=popup#.”
    printf("%s%s. ", "Thank you ", org.bossName);

    printf("%s%s%s\n", "The url to raise funds for ", org.organizationName, " is: ");

    printf("%s\n\n", org.organizationUrl);

}

bool isValidEmail(const char* email) 
{
    bool gotValidEmail = false;
    size_t length = strlen(email);
    int numOfAts = 0;
    int numOfPer = 0;
    int numOfSpaces = 0;
    int atIndex = 0;
    int dotIndex = 0;

    for (int i = 0; i < length; i++) 
    {
        if (email[i]=='@')
        {
            atIndex = i;
            numOfAts++;
        }
        else if (email[i] == '.')
        {
            dotIndex = i;
            numOfPer++;
        }
        else if(email[i]==' ')
        {
            numOfSpaces++;
        }
    }

    if (numOfSpaces == 0)
    {
        if (atIndex != 0 && numOfAts == 1)
        {
            if (dotIndex > atIndex + 1 && numOfPer == 1)
            {
                int count = 0;
                while (email[dotIndex + 1] != '\0')
                {
                    count++;
                    dotIndex++;
                }
                if (count == 3)
                {
                    gotValidEmail = true;
                }
                else
                {
                    puts("Incorrect 3 letter extension");
                }
            }
            else
            {
                puts("Wrong formatting of . symbol");
            }
        }
        else
        {
            puts("Wrong formatting of @ symbol");
        }
    }
    else 
    {
        puts("No spaces allowed in email");
    }
    return gotValidEmail;
}


bool isValidPass(const char* pass, int minimum)
{
    bool gotValidPass = false;
    size_t length = strlen(pass);
    int numOfUpper = 0;
    int numOfLower = 0;
    int numOfSpaces = 0;
    int numOfDigits = 0;


    for (int i = 0; i < length; i++)
    {
        if (pass[i] == ' ')
        {
            numOfSpaces++;
        }
        else if (isupper(pass[i]))
        {
            numOfUpper++;
        }
        else if (islower(pass[i]))
        {
            numOfLower++;
        }
        else if (isdigit(pass[i]))
        {
            numOfDigits++;
        }
    }

    if (length>=minimum && numOfSpaces == 0 && numOfUpper>0 && numOfLower>0 && numOfDigits>0)
    {
        gotValidPass = true;
    }
    else
    {
        puts("Incorrect format. Password must contain at least 1 capital, 1 lower cased letter, 1 number, and no spaces");
        printf("%s%d%s\n","Password must also be at least ", minimum, " characters long");
    }
    return gotValidPass;
}

void createReceiptFile(Organization* org) 
{
    char fileName[SIZE] = "";
    char filePath[SIZE] = "";

    removeSpaces(fileName, org->organizationName);
    strcat(fileName, fileReceipt);

    strcpy(filePath, fundraisersPath);
    strcat(filePath, fileName);

    strcpy(org->filePath, filePath);

    FILE* orgfPtr;

    puts("");
    // fopen opens file. Exit program if unable to create file 
    if ((orgfPtr = fopen(filePath, "w")) == NULL) {
        puts("File could not be opened");
    }
    else {

        puts("wrote in file");
        fprintf(orgfPtr, "%s%s\n", org->organizationName, " receipt file.");
        fclose(orgfPtr);
    }

}

Organization* getOrgbyName(Organization* headPtr, char* name)
{
    Organization* orgFound = NULL;

    if (headPtr == NULL)
    {
        puts("No orgs in list");
    }
    else
    {
        Organization* currentOrg = NULL;
        currentOrg = headPtr;
        while (currentOrg != NULL)
        {
            if (compareStrings(currentOrg->organizationName, name)==0) 
            {
                orgFound = currentOrg;
                currentOrg = currentOrg->nextOrgPtr;
            }
            else 
            {
                currentOrg = currentOrg->nextOrgPtr;
            }
        }

    }

    return orgFound;
}

//simple display function
//calculates the percentage towards the goal using amount raised and goal in the organization
void displayFunds(const Organization* org)
{
    printf("%s\n\n", org->organizationUrl);
    printf("%s\n", "MAKE A DIFFERENCE BY YOUR DONATION");

    printf("%s%s\n", "Organization: ", org->organizationName);
    printf("%s%s\n\n", "Purpose: ", org->purpose);

    printf("%s%.2lf \n", "We currently have raised: ", org->amountRaised);

    if (org->amountRaised >= org->goal)
    {
        printf("%s\n\n", "We have reached our goal but could still use the donation");
    }
    else
    {
        double percentage = (org->amountRaised / org->goal) * 100;
        printf("%s%.2lf%s%.2lf \n\n", "We are ", percentage, " percent towards our goal of ", org->goal);
    }
}//------------------------------------end displayFunds-----------------------------------------
