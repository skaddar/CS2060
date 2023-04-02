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
#include <ctype.h>


#define SIZE 80
#define URL_PARTS_LENGTH 30
#define ADMIN_PIN 'Q'
#define ZIPCODE_LENGTH 5
const char urlFirstPart[] = "https:donate.com/";
const char urlSecondPart[] = "?form=popup#";


typedef struct organization {

    char organizationName[80];
    char bossName[80];
    char purpose[80];
    char email[80];
    char password[80];
    char organizationUrl[URL_PARTS_LENGTH + SIZE];
    double goal;
    double amountRaised;

}Organization;

char* custom_fgets(char*, size_t, FILE*);
void getValidDouble(double*);
void setUpOrganization(Organization*);
void displayOrganization(Organization);
void removeSpaces(char* name, const Organization*);
void createUrl(Organization*);
unsigned int getDonation(Organization*, double* );
void displayFunds(Organization*);


int main(void)
{

    printf("%s\n", "---------------------------------------------------------");
    printf("%s\n", "              Set Up Fundraiser");
    printf("%s\n", "---------------------------------------------------------");
    Organization fundraiser1;
    setUpOrganization(&fundraiser1);
    createUrl(&fundraiser1);
    displayOrganization(fundraiser1);
 
    printf("%s\n", "---------------------------------------------------------");
    printf("%s\n", "                    Donate!");
    printf("%s\n\n", "---------------------------------------------------------");
    
    bool finishDonating = false;

    do 
    {
        displayFunds(&fundraiser1);
        printf("%s\n", "Enter Donation:");

        double donatedAmount = 0;
        //if not admin pin
        if (getDonation(&fundraiser1, &donatedAmount) == 0) 
        {
            char donorFirstName[] = "";
            char donorLastName[] = "";
            printf("%s\n", "First name:");
            custom_fgets(donorFirstName, SIZE, stdin);
            printf("%s\n", "Last name:");
            custom_fgets(donorLastName, SIZE, stdin);
            printf("%s\n\n", "---------------------------------------------------------");
        
        }
        //if admin pin is entered
        else 
        {
            printf("%s\n", "hello admin");
            finishDonating = true;
        
        }
    } while (finishDonating != true);

    printf("%s\n", "end donation mode");

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

void getValidDouble(double* validDouble) 
{
    char* end;
    errno = 0;
    bool gotValid = false;
    double doubleTest = 0;
    char inputStr[SIZE];

    do
    {
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

    organization->goal = 0;
    organization->amountRaised = 0;
    printf("%s", "Enter a name for the organization:");
    custom_fgets(organization->organizationName, SIZE, stdin);

    printf("%s", "Enter the purpose of the organization:");
    custom_fgets(organization->purpose, SIZE, stdin);

    printf("%s", "Enter First and Last Name:");
    custom_fgets(organization->bossName, SIZE, stdin);

    printf("%s", "Enter goal amount:");
    getValidDouble(&(organization->goal));

    printf("%s\n", "Enter email address:");
    custom_fgets(organization->email, SIZE, stdin);
    
    printf("%s\n", "Enter password:");
    custom_fgets(organization->password, SIZE, stdin);
}

void removeSpaces(char* name, const Organization* org)
{

    strcpy(name, org->organizationName);
    for (int i = 0; i < strlen(org->organizationName); i++)
    {
        if (name[i] == ' ')
        {
            name[i] = '-';
        }
    }
    return name;
}

void createUrl(Organization* org) 
{
    char nameWithNoSpaces[SIZE];
        strcpy(&(org->organizationUrl), urlFirstPart);

        removeSpaces(&(nameWithNoSpaces), &(org->organizationName));

        strcat(&(org->organizationUrl), nameWithNoSpaces);
        strcat(&(org->organizationUrl), urlSecondPart);
}


void displayOrganization(Organization org)
{

    //“Thank you [first and last name]. The url to
    //raise funds for [Organization] is  https:donate.com/[name-of-organization]?form=popup#.” 

    printf("%s%s. ", "Thank you ", org.bossName);

    printf("%s%s%s\n", "The url to raise funds for ", org.organizationName, " is: ");

    printf("%s\n\n", org.organizationUrl);

}

unsigned int getDonation(Organization* org, double* validDouble)
{
        char* end;
        errno = 0;
        bool gotValid = false;
        unsigned int adminInput= 0;
        double doubleTest = 0;
        char inputStr[SIZE];

        do
        {
            custom_fgets(inputStr, SIZE, stdin);

            if (strlen(inputStr)<=1 && isdigit(inputStr[0])==0)
            {
                if (toupper(inputStr[0]) == ADMIN_PIN)
                {
                    gotValid = true;
                    adminInput = 1;
                }
            }
            
            if (gotValid == false) 
            {
                double doubleTest = strtod(inputStr, &end, 10);
                if (end == inputStr) {
                    printf("%s: not a decimal number\n", inputStr);
                }
                else if ('\0' != *end)
                {
                    printf("%s: extra characters at end of input: %s\n", inputStr, end);
                }
                else {
                    org->amountRaised = org->amountRaised + doubleTest;
                    *validDouble = doubleTest;
                    gotValid = true;
                    adminInput = 0;
                }
            }
           
        } while (gotValid != true);

        return adminInput;
}

void displayFunds(Organization* org) 
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
}

bool getValidZip(int zipcodeSize) 
{
    bool flag = false;
    char zipcode[] = "";

    do 
    {
        printf("%s\n", "Enter a valid Zipcode: ");
        custom_fgets(zipcode, SIZE, stdin);

        if (zipcode[0]!='0')
        {
            if (strlen(zipcode) == ZIPCODE_LENGTH)
            {

            }
        }
    }

}
