//
// Name: Soliman Kaddar
// Class: CS2060 (M/W)
// Iteration 01 Implementation
// Due:4 April 2023
// 
// Description: Use strings and structures to create a fundraiser program that allows users to keep donating
// until the admin checks out. Will use string functions to get information such as email, password, names, and even
// the zipcode. Learn to ditch scanf for fgets and converting that string to a number. The structure will be the 
// fundraiser and will use pointers to pass the structure and other strings by reference to be updated.
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

    //declare the fundraiser and pass it by reference to the method to initialize the data in the struct
    //also to create the URL for the organization
    //pass theorganization by value to display its information
    Organization fundraiser1;
    setUpOrganization(&fundraiser1);
    createUrl(&fundraiser1);
    displayOrganization(fundraiser1);
 
    printf("%s\n", "---------------------------------------------------------");
    printf("%s\n", "                    Donate!");
    printf("%s\n\n", "---------------------------------------------------------");
    
    //boolean flag will only be set as true when the admin is verified 
    bool finishDonating = false;

    //This will loop until the admin is verified 
    do 
    {
        displayFunds(&fundraiser1);
        printf("%s\n", "Enter Donation:");

        //donated amount will be passed and updated in the getDonation function
        //declared in main because it will be used for the receipt 
        //if not admin pin
        //getDonation will return 0 if the admin pin was not entered
        //if the admin pin was not entered. get the donor name and ask for receipt and then loop again
        
        double donatedAmount = 0;
        if (getDonation(&fundraiser1, &donatedAmount) == 0) 
        {
            getDonorName();
            getValidZip(ZIPCODE_LENGTH, donatedAmount);
            askForReceipt(&fundraiser1, donatedAmount);
       
        }

        //if admin pin is entered ask for the email and password and theyhave 2 tries to enter them correctly
        //If incorrect then exit if statement and loop donation mode again. If correct then set boolean flag 
        //To true and exit the loop

        else 
        {
            printf("\n%s\n", "---------------------------------------------------------");
            printf("%s\n", "                    Report Mode");
            printf("%s\n\n", "---------------------------------------------------------");
            
            //If the email is verified
            if (compareEmail(&fundraiser1))
            {
                //If the password is verified 
                if (comparePass(&fundraiser1)) 
                {
                    //Summarize fundraiser data and exit loop 
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
}   //------------------------------------------END MAIN----------------------------------------------

//Same parameters as fgets and returns a string 
//This function will remove the new line character from an input and return the updated input
char* custom_fgets(char* stringPtr, size_t maxNumOfChars, FILE* inputStream) 
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

//parameters is just a double passed by reference and stored as a pointer
//does not return anything because the value is updated in the function
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
        double doubleTest = strtod(inputStr, &end, 10);

        //first Make sure the end has moved meaning it read some values
        //then check that end is null because that means it read all values
        //if it passes those tests then update the double and exit loop
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
}//------------------------------------end getValidDouble--------------------------------


//initializes all of the organization data members 
//no return the paramter is just the organization passed by reference using a pointer
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

}//------------------------------------end setUpOrganization-----------------------------------------

//returns a string with no spaces. parameters is the organization passed as a constant and a string to store
//the string without the spaces
char* removeSpaces(char* name, const Organization* org)
{
    //get the name of the organization and store the string in name
    //Then loop through all of the characters in the string and replace any spaces with a dash
    strcpy(name, org->organizationName);
    for (int i = 0; i < strlen(org->organizationName); i++)
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
void createUrl(Organization* org) 
{
    //will store the return of remove spaces
    char nameWithNoSpaces[SIZE] = "";
        strcpy(&(org->organizationUrl), urlFirstPart);

        removeSpaces(&(nameWithNoSpaces), &(org->organizationName));

        strcat(&(org->organizationUrl), nameWithNoSpaces);
        strcat(&(org->organizationUrl), urlSecondPart);

}//------------------------------------end createUrl-----------------------------------------


void displayOrganization(Organization org)
{
    //“Thank you [first and last name]. The url to
    //raise funds for [Organization] is  https:donate.com/[name-of-organization]?form=popup#.”
    printf("%s%s. ", "Thank you ", org.bossName);

    printf("%s%s%s\n", "The url to raise funds for ", org.organizationName, " is: ");

    printf("%s\n\n", org.organizationUrl);

}

//gets donations and checks for admin pin
//basically the same as getValidDouble but uses another check for the admin pin at the start
//returns a 0 if admin pin is not entered and 1 if admin pin is entered
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
            //check if the input is one character long and it is not a digit and that char is the admin Pin 
            custom_fgets(inputStr, SIZE, stdin);
            if (strlen(inputStr)<=1 && isdigit(inputStr[0])==0)
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
}//------------------------------------end getDonation-----------------------------------------

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

//function to get a valid 5 digit number not starting with 0
//Parameters only need the size that the zipcode can be and how much is donated
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

    //calculates processing fee and prints out the thanks for donation

    double processingFee = donatedAmount * CARD_PROCESS_FEE;
    printf("%s%.2lf%s\n", "Thank you for your donation.There is a ", CARD_PROCESS_FEE*100,
        " % credit card processing");
    printf("%s%.2lf%s%.2lf%s\n", "fee of [$ ", processingFee ,"] . [$ ", donatedAmount - processingFee
        ,"] will be donated.");
     
}//------------------------------------end getValidZip-----------------------------------------

//no returns. uses amount donated and the organization as a constant to ask and print a receipt
//This function will check for one character
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

}//------------------------------------end askForReceipt-----------------------------------------

//function that gives admin two tries to enter a valid email that matches the organization email
//returns a true if email is verified and false if both tries are used up
bool compareEmail(const Organization* org) 
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
        if (strcmp(input, org->email) == 0) 
        {
            validStringEntered = true;
        }
        //if email is incorrect display attempts left add to counter and loop again
        else 
        {
            printf("%s%d%s\n", "Incorrect email. ", 1-counter, " attempts left");
            counter++;
        }
    }
    //return boolean
    return validStringEntered;
}//------------------------------------end compareEmail-----------------------------------------

//this function is the exact same as compare email except it checks for password. could have made one function
//but printing is different
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
}//------------------------------------end comparePass-----------------------------------------

//print summary of organization
void fundraiserSummary(const Organization* org) 
{
    printf("\n%s%s\n", "Summary for ", org->organizationName);
    printf("%s\n", "----------------------------------------------------------------");
    printf("%s%d\n","number of donors: ", org->numOfDonors);
    printf("%s%.2lf\n", "amount raised: ", org->amountRaised );
    printf("%s%.2lf\n", "amount raised in credit card fees: ", org->creditCardFees);
}

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