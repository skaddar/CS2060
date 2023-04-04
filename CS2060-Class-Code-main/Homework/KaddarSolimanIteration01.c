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
#include <time.h>


#define SIZE 80
#define URL_PARTS_LENGTH 30
#define ADMIN_PIN 'Q'
#define ZIPCODE_LENGTH 5
#define CARD_PROCESS_FEE 0.029
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
    unsigned int numOfDonors;
    double creditCardFees;
}Organization;

char* custom_fgets(char*, size_t, FILE*);
void getValidDouble(double*);
void setUpOrganization(Organization*);
void displayOrganization(Organization);
char* removeSpaces(char* , const Organization*);
void createUrl(Organization*);
unsigned int getDonation(Organization*, double* );
void displayFunds(const Organization*);
void getValidZip(size_t, double);
void askForReceipt(const Organization* , double);
bool comparePass(const Organization* org);
bool compareEmail(const Organization* org);
void fundraiserSummary(const Organization* org);
void getDonorName();


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
             getDonorName();
            getValidZip(ZIPCODE_LENGTH, donatedAmount);

            askForReceipt(&fundraiser1, donatedAmount);
       
        }
        //if admin pin is entered
        else 
        {
            printf("\n%s\n", "---------------------------------------------------------");
            printf("%s\n", "                    Report Mode");
            printf("%s\n\n", "---------------------------------------------------------");
            
            if (compareEmail(&fundraiser1))
            {
                if (comparePass(&fundraiser1)) 
                {

                    fundraiserSummary(&fundraiser1);
                    finishDonating = true;
                }
                else 
                {
                    printf("%s", "Wrong Password. Going back to donations mode.\n");
                }
            }
            else 
            {
                printf("%s", "Wrong Email. Going back to donations mode.\n");
            }

            
        }
    } while (finishDonating != true);

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
    organization->numOfDonors = 0;
    organization->creditCardFees = 0;

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

char* removeSpaces(char* name, const Organization* org)
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
    char nameWithNoSpaces[SIZE] = "";
        strcpy(&(org->organizationUrl), urlFirstPart);

        removeSpaces(&(nameWithNoSpaces), &(org->organizationName));

        strcat(&(org->organizationUrl), nameWithNoSpaces);
        strcat(&(org->organizationUrl), urlSecondPart);
}


void displayOrganization(Organization org)
{

    //�Thank you [first and last name]. The url to
    //raise funds for [Organization] is  https:donate.com/[name-of-organization]?form=popup#.� 

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

                    double processingFee = doubleTest * CARD_PROCESS_FEE;

                    org->amountRaised = org->amountRaised + (doubleTest - processingFee);
                    org->creditCardFees = org->creditCardFees + processingFee;
                    org->numOfDonors = org->numOfDonors + 1;
                    *validDouble = doubleTest;
                    gotValid = true;
                    adminInput = 0;
                }
            }
           
        } while (gotValid != true);

        return adminInput;
}

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
}

void getValidZip(size_t zipcodeSize, double donatedAmount) 
{
    bool flag = false;
    char zipcode[SIZE] = "";
    do 
    {
        printf("%s\n", "Enter a valid Zipcode: ");
        custom_fgets(zipcode, SIZE, stdin);

        //checks if the first character is a digit that is not 0
        if (zipcode[0]!='0' && isdigit(zipcode[0]))
        {
            //Checks if the length after removing the newline character is equal to a zip length
            if (strlen(zipcode) == ZIPCODE_LENGTH)
            {
                //Loops until it counts 5 digits or encounters a non digit 
                unsigned int counter = 0;
                bool isDigit = true;
                while (counter<=5 && isDigit)
                {
                    if (isdigit(zipcode[counter]))
                    {
                        counter++;
                    }
                    else 
                    {
                        isDigit = false;
                    }
                }
                //Checks if the last loop repeated 5 times meaning it counted 5 digits
                if (counter==5)
                {
                    flag = true;
                }
                else
                {
                    printf("%s\n", "zipcode must only contain valid integer values");
                }
            }
            else 
            {
                printf("%s\n", "Zipcode can only be 5 characters long");
            }
        }
        else 
        {
            printf("%s\n", "First Character must be a number greater than 0");
        }
    } while (flag == false);

    double processingFee = donatedAmount * CARD_PROCESS_FEE;
    printf("%s%.2lf%s\n", "Thank you for your donation.There is a ", CARD_PROCESS_FEE*100,
        " % credit card processing");
    printf("%s%.2lf%s%.2lf%s\n", "fee of [$ ", processingFee ,"] . [$ ", donatedAmount - processingFee
        ,"] will be donated.");
     
}

void askForReceipt(const Organization* org, double amountDonated) 
{

    char inputStr[SIZE] = "";
    bool gotValid = false;
    double processingFee = amountDonated * CARD_PROCESS_FEE;
    double processedDonatedAmt = amountDonated - processingFee;

    printf("%s\n", "Do you want a receipt? (y)es or (n)o?");
    do
    {
        custom_fgets(inputStr, SIZE, stdin);

        if (strlen(inputStr) <= 1)
        {
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

    if (toupper(inputStr[0]) == 'Y') {
        
        time_t donationDate;
        time(&donationDate);

        printf("%s\n", "Receipt:");
        printf("%s%s\n", "Organization: ", org->organizationName);
        printf("%s%.2lf\n", "Donation Amount:", processedDonatedAmt );
        printf("%s%s \n", "Donation Date : ", ctime(&donationDate));
        printf("%s\n\n", "---------------------------------------------------------");
    }
    if (toupper(inputStr[0]) == 'N')
    {
        printf("%s\n\n", "---------------------------------------------------------");
        
    }

}

bool compareEmail(const Organization* org) 
{
    unsigned int counter = 0;
    bool validStringEntered = false;
    char input[SIZE] = "";

    while (counter < 2 && validStringEntered == false)
    {
        printf("%s\n", "Enter email: ");
        custom_fgets(input, SIZE, stdin);

        if (strcmp(input, org->email) == 0) 
        {
            validStringEntered = true;
        }
        else 
        {
            printf("%s%d%s\n", "Incorrect email. ", 2-counter, " attempts left");
            counter++;
        }
    }
    return validStringEntered;
}

bool comparePass(const Organization* org)
{
    unsigned int counter = 0;
    bool validStringEntered = false;
    char input[SIZE] = "";

    while (counter < 2 && validStringEntered == false)
    {
        printf("%s\n", "Enter Password: ");
        custom_fgets(input, SIZE, stdin);

        if (strcmp(input, org->password) == 0)
        {
            validStringEntered = true;
        }
        else
        {
            printf("%s%d%s\n", "Incorrect Password. ", 2 - counter, " attempts left");
            counter++;
        }
    }
    if (counter == 2) {
        printf("%s", "Going back to donations mode.\n");
    }

    return validStringEntered;
}

void fundraiserSummary(const Organization* org) 
{
    printf("\n%s%s\n", "Summary for ", org->organizationName);
    printf("%s\n", "----------------------------------------------------------------");
    printf("%s%d\n","number of donors: ", org->numOfDonors);
    printf("%s%.2lf\n", "amount raised: ", org->amountRaised );
    printf("%s%.2lf\n", "amount raised in credit card fees: ", org->creditCardFees);
}

void getDonorName() 
{

    char donorFirstName[SIZE] = "";
    char donorLastName[SIZE] = "";
    printf("%s\n", "First name:");
    custom_fgets(donorFirstName, SIZE, stdin);
    printf("%s\n", "Last name:");
    custom_fgets(donorLastName, SIZE, stdin);
}