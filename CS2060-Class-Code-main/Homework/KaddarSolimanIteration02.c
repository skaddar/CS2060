//
// Name: Soliman Kaddar
// Class: CS2060 (M/W)
// Iteration 02 Implementation
// Due:25 April 2023
// 
// Description: Implement iteration 01 with the fundraisers but allow 
// for more than one organization to be added and they will all be stored
// in a linked list that will be sorted  alphabetically. Then write all 
// receipts for the organiaztions in seperate files and when the program
// is ready to quit write the organization summaries in a file all together
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
const char fundraisersSummaryFile[] = "org.txt";
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
void getValidZip(size_t, double);
unsigned int getDonation(Organization*, double*);
void getDonorName();
void removeRemainingOrgs(Organization**);
void askForReceipt(const Organization*, double);
bool compareEmail(const char*);
bool comparePass(const char*);
void fundraiserSummary(Organization*);

int main(void)
{

    printf("%s\n",  "---------------------------------------------------------");
    printf("%s\n", "              Set Up Fundraiser");
    printf("%s\n\n","---------------------------------------------------------");

    //declare the head pointer and mandatory setup of first organization
    //keep track of head pointer to manipulate linked list
    Organization* headPtr = NULL;
    setUpOrganization(&headPtr);
    bool finishAdding = false;

    //loop this until the user does not want to add any more organizations
    //basically initializing the whole linked list.
    //Set up mode::
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
        //display the organizations that the user can choose from along with their total donations and goal
        //After the user enters the name we will use the getOrgByName function to search for that organization
        //ignoring case
        //Check if the returned value is null meaning the organization name is not valid and repeat the question

        displayOrgs(headPtr);

        char inputOrgName[SIZE] = "";
        custom_fgets(inputOrgName, SIZE, stdin);

        Organization* chosenOrg = getOrgbyName(headPtr, inputOrgName);

        if (chosenOrg != NULL) 
        {
            displayFunds(chosenOrg);
            puts("--------------------------------------------------------------\n");

            printf("%s\n", "Enter Donation:");

            //donated amount will be passed and updated in the getDonation function
            //declared in main because it will be used for the receipt 
            //if not admin pin
            //getDonation will return 0 if the admin pin was not entered
            //if the admin pin was not entered. get the donor name and ask for receipt and then loop again

            double donatedAmount = 0;
            if (getDonation(chosenOrg, &donatedAmount) == 0)
            {
                getDonorName();
                getValidZip(ZIPCODE_LENGTH, donatedAmount);
                askForReceipt(chosenOrg, donatedAmount);

            }
            else 
            {
                printf("\n%s\n", "---------------------------------------------------------");
                printf("%s\n", "                    Report mode");
                printf("%s\n\n", "---------------------------------------------------------");

                //check email and password of that specific organization to quit the program
                //if email or password are incorrect then repeat the prompt to ask for organization to donate
                //if they are correct break out of the loop
                if (compareEmail(chosenOrg->email))
                {
                    //If the password is verified 
                    if (comparePass(chosenOrg->password))
                    {
                        //Summarize fundraiser data and exit loop 
                        finishDonating = true;
                        puts("\nThank you. printing summary and exiting program");

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

        }
        else 
        {
            puts("No organizations found of that name");
        }

    
    } while (!finishDonating);

    //When admin quits. free all organizations and display summary on console and file

    fundraiserSummary(headPtr);
    removeRemainingOrgs(&headPtr);
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

    //Allocate memory for a new organization 
    //check if the memory got allocated and if it did then initialize data members
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

        //Ask for email and if it is a valid email then confirm with the user
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

        //Ask for password and validate. if incorrect then repeat question

        int validPassQuestion = 0;

        do
        {
            printf("%s\n", "Enter password:");
            custom_fgets(newOrg->password, SIZE, stdin);

        } while (isValidPass(newOrg->password, PASSWORD_MINIMUM) != true);

        //Create a url for the fundraiser as well as a receipt file that bwill be stored
        //in the fundraisers file
        //Then at the end add the organization to the list

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

//function to compare two strings ignoring the case
//will be passed two strings and return an integer based on which one
//comes first in the alphabet
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

//function to add an organization to the linked list. it will be passed the
//head pointer as a double pointer and a pointer to the organization that will be added
//no return
void addOrgToList(Organization** head, Organization* orgToAdd) 
{
    Organization* previousOrgPtr = NULL;
    Organization* currentOrgPtr = *head;

    //repeat until found the correct spot for orgToAdd then add it using previous and current
        while (currentOrgPtr != NULL && compareStrings(currentOrgPtr->organizationName, orgToAdd->organizationName) == -1)
        {
            previousOrgPtr = currentOrgPtr;

            currentOrgPtr = currentOrgPtr->nextOrgPtr;
        }

        //This means there is no head 
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

//Verify the email. will be passed the email in the organization as a constant
//because it will not be changed
//returns true if valid email and false if not in correct format

bool isValidEmail(const char* email) 
{
    bool gotValidEmail = false;
    size_t length = strlen(email);
    int numOfAts = 0;
    int numOfPer = 0;
    int numOfSpaces = 0;
    int atIndex = 0;
    int dotIndex = 0;

    //count how many @s and dots there is and spaces because emails
    //should not contain spaces
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

                //This will make sure of the last 3 character extension
                //making sure that before the null character and after the dot there is 
                //only 3 characters

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

//Verify the pass. will be passed the pass in the organization as a constant
//because it will not be changed
//returns true if valid pass and false if not in correct format
bool isValidPass(const char* pass, int minimum)
{
    bool gotValidPass = false;
    size_t length = strlen(pass);
    int numOfUpper = 0;
    int numOfLower = 0;
    int numOfSpaces = 0;
    int numOfDigits = 0;

    //count how many capitals, upper cased letters, lower cased letters
    //and numbers there is in the string there should be more than 0 of each
    //Also no spaces
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

//function to create a recepit file for the organization
//no return only passed the organization to write the members to a file
void createReceiptFile(Organization* org) 
{
    char fileName[SIZE] = "";
    char filePath[SIZE] = "";

    //replace spaces with dashes for the org name

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

        fprintf(orgfPtr, "%s%s\n", org->organizationName, " receipt file.");
        fclose(orgfPtr);
    }

}

//find an organization by the name and return the address of that organization
//Will be passed the headptr to find the org in the list as well as the name of the org
Organization* getOrgbyName(Organization* headPtr, char* name)
{
    Organization* orgFound = NULL;

    //if head is empty there are no organizations in the list
    //if it is not then use a new node of current pointer to traverse the list and compare
    //each organizations name with the inputted name and if they match then update the organization
    //found with the address of that organization

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


//gets donations and checks for admin pin
//basically the same as getValidDouble but uses another check for the admin pin at the start
//returns a 0 if admin pin is not entered and 1 if admin pin is entered
unsigned int getDonation(Organization* org, double* validDouble)
{
    char* end;
    errno = 0;
    bool gotValid = false;
    unsigned int adminInput = 0;
    double doubleTest = 0;
    char inputStr[SIZE];

    do
    {
        //check if the input is one character long and it is not a digit and that char is the admin Pin 
        custom_fgets(inputStr, SIZE, stdin);
        if (strlen(inputStr) <= 1 && isdigit(inputStr[0]) == 0)
        {
            if (toupper(inputStr[0]) == ADMIN_PIN)
            {
                gotValid = true;
                adminInput = 1;
            }
        }

        //Same as getValidDouble
        if (gotValid == false)
        {
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
                        double processingFee = doubleTest * CARD_PROCESS_FEE;
                        org->amountRaised = org->amountRaised + (doubleTest - processingFee);
                        org->creditCardFees = org->creditCardFees + processingFee;
                        org->numOfDonors = org->numOfDonors + 1;
                        adminInput = 0;
                    }
                    else {
                        puts("number must be greater than 0");
                    }
                }
                else {
                    printf("%s: extra characters at end of input: %s\n", inputStr, end);
                }
            }
            else {
                printf("%s: not a decimal number\n", inputStr);
            }
   
        }

    } while (gotValid != true);

    return adminInput;
}//------------------------------------end getDonation-----------------------------------------

void getValidZip(size_t zipcodeSize, double donatedAmount)
{
    bool flag = false;
    char zipcode[SIZE] = "";
    do
    {
        printf("%s\n", "Enter a valid Zipcode: ");
        custom_fgets(zipcode, SIZE, stdin);

        //checks if the first character is a digit that is not 0
        if (zipcode[0] != '0' && isdigit(zipcode[0]))
        {
            //Checks if the length after removing the newline character is equal to a zip length
            if (strlen(zipcode) == ZIPCODE_LENGTH)
            {
                //Loops until it counts 5 digits or encounters a non digit 
                unsigned int counter = 0;
                bool isDigit = true;
                while (counter <= 5 && isDigit)
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
                if (counter == 5)
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

    //calculates processing fee and prints out the thanks for donation

    double processingFee = donatedAmount * CARD_PROCESS_FEE;
    printf("%s%.2lf%s\n", "Thank you for your donation.There is a ", CARD_PROCESS_FEE * 100,
        " % credit card processing");
    printf("%s%.2lf%s%.2lf%s\n", "fee of [$ ", processingFee, "] . [$ ", donatedAmount - processingFee
        , "] will be donated.");

}//------------------------------------end getValidZip-----------------------------------------

//get donor names no verification necessary
void getDonorName()
{
    char donorFirstName[SIZE] = "";
    char donorLastName[SIZE] = "";
    printf("%s\n", "First name:");
    custom_fgets(donorFirstName, SIZE, stdin);
    printf("%s\n", "Last name:");
    custom_fgets(donorLastName, SIZE, stdin);
}

//traverse the list and free each organization and at the end 
//free the head pointer
void removeRemainingOrgs(Organization** headPtr)
{
    Organization* currentPtr = *headPtr;
    Organization* nextNodePtr = NULL;

    while (currentPtr != NULL)
    {
        nextNodePtr = currentPtr->nextOrgPtr;
        free(currentPtr);
        currentPtr = nextNodePtr;
    }

    *headPtr = NULL;

}

//function to ask for recepit from donor and if they want a recepit add it to the
//organization receipt file
//no return.
void askForReceipt(const Organization* org, double amountDonated)
{

    printf("%s\n", "Do you want a receipt? (y)es or (n)o?");
    int choice = askForInsert();

    double processingFee = amountDonated * CARD_PROCESS_FEE;
    double processedDonatedAmt = amountDonated - processingFee;

    //if Y is entered print receipt
    //if N is entered exit function
    if (choice==1) {

        time_t donationDate;
        time(&donationDate);

        printf("%s\n", "Receipt:");
        printf("%s%s\n", "Organization: ", org->organizationName);
        printf("%s%.2lf\n", "Donation Amount:", processedDonatedAmt);
        printf("%s%s \n", "Donation Date : ", ctime(&donationDate));
        printf("%s\n\n", "---------------------------------------------------------");

        FILE* orgfPtr;

        puts("");
        // fopen opens file. Exit program if unable to create file 
        if ((orgfPtr = fopen(org->filePath, "a")) == NULL) {
            puts("File could not be opened");
        }
        else {

            fprintf(orgfPtr, "\n%s\n", "Receipt:");
            fprintf(orgfPtr, "%s%s\n", "Organization: ", org->organizationName);
            fprintf(orgfPtr, "%s%.2lf\n", "Donation Amount:", processedDonatedAmt);
            fprintf(orgfPtr, "%s%s \n", "Donation Date : ", ctime(&donationDate));
            fprintf(orgfPtr, "%s\n\n", "---------------------------------------------------------");
            fclose(orgfPtr);
        }


    }
    if (choice == 0)
    {
        printf("%s\n\n", "---------------------------------------------------------");

    }

}//------------------------------------end askForReceipt-----------------------------------------

//function that gives admin two tries to enter a valid email that matches the organization email
//returns a true if email is verified and false if both tries are used up
bool compareEmail(const char* email)
{
    unsigned int counter = 0;
    bool validStringEntered = false;
    char input[SIZE] = "";

    //loop while they have not used both attempts and valid email has not been entered
    while (counter < 2 && validStringEntered == false)
    {
        printf("%s\n", "Enter email: ");
        custom_fgets(input, SIZE, stdin);

        //if email is valid set boolean flag and exit loop
        if (compareStrings(input, email) == 0)
        {
            validStringEntered = true;
        }
        //if email is incorrect display attempts left add to counter and loop again
        else
        {
            printf("%s%d%s\n", "Incorrect email. ", 1 - counter, " attempts left");
            counter++;
        }
    }
    //return boolean
    return validStringEntered;
}//------------------------------------end compareEmail-----------------------------------------

//this function is the exact same as compare email except it checks for password. could have made one function
//but printing is different
bool comparePass(const char* pass)
{
    unsigned int counter = 0;
    bool validStringEntered = false;
    char input[SIZE] = "";

    while (counter < 2 && validStringEntered == false)
    {
        printf("%s\n", "Enter Password: ");
        custom_fgets(input, SIZE, stdin);

        if (strcmp(input, pass) == 0)
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
}//------------------------------------end comparePass-----------------------------------------

void fundraiserSummary(Organization* headPtr)
{
    if (headPtr == NULL)
    {
        puts("No orgs in list");
    }
    else
    {

        puts("Displaying all fundraisers summaries:\n");
        puts("----------------------------------------------------------------------------------\n\n");

        Organization* currentOrg = headPtr;

        while (currentOrg != NULL)
        {
            printf("\n%s%s\n", "Summary for ", currentOrg->organizationName);
            printf("%s\n", "----------------------------------------------------------------");
            printf("%s%d\n", "number of donors: ", currentOrg->numOfDonors);
            printf("%s%.2lf\n", "amount raised: ", currentOrg->amountRaised);
            printf("%s%.2lf\n", "amount raised in credit card fees: ", currentOrg->creditCardFees);

            currentOrg = currentOrg->nextOrgPtr;
        }

    }

    FILE* orgfPtr;

    char summaryFile[SIZE] = "";
    strcpy(summaryFile, fundraisersPath);
    strcat(summaryFile, fundraisersSummaryFile);

    puts("");
    // fopen opens file. Exit program if unable to create file 
    if ((orgfPtr = fopen(summaryFile, "w")) == NULL) {
        puts("File could not be opened");
    }
    else {

        fprintf(orgfPtr, "%s", "Displaying all fundraisers summaries:\n");
        fprintf(orgfPtr, "%s", "----------------------------------------------------------------------------------\n");

        Organization* currentOrg = headPtr;

        while (currentOrg != NULL)
        {
            fprintf(orgfPtr, "\n%s%s\n", "Summary for ", currentOrg->organizationName);
            fprintf(orgfPtr, "%s\n", "----------------------------------------------------------------");
            fprintf(orgfPtr, "%s%d\n", "number of donors: ", currentOrg->numOfDonors);
            fprintf(orgfPtr, "%s%.2lf\n", "amount raised: ", currentOrg->amountRaised);
            fprintf(orgfPtr, "%s%.2lf\n", "amount raised in credit card fees: ", currentOrg->creditCardFees);

            currentOrg = currentOrg->nextOrgPtr;
        }

        fclose(orgfPtr);
    }

}