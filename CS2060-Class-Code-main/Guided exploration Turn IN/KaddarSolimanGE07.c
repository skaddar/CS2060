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
    struct pet* nextPetPtr;
}Pet;


char* custom_fgets(char*, size_t, FILE*);
int askForInsert();
int compareStrings(const char*, const char*);
void displayPets(Pet*);
Pet* createPet(Pet**, unsigned int*);
void writePetsToFile(Pet*);
void removeRemainingPets(Pet**);
bool deletePet(Pet**, char*);
void deletePetsFunction(Pet*);


int main(void)
{

    bool flag = false;
    unsigned int numOfPets = 0;


    Pet* headPtr = NULL;
    Pet* pet1 = createPet(&headPtr, &numOfPets);

    do
    {
        puts("created a new pet");
        unsigned int addPet = askForInsert();

        if (addPet == 1)
        {
            printf("%s\n", "Add a new pet");
            Pet* petPtr = createPet(&headPtr, &numOfPets);
        }
        if (addPet == 0)
        {
            displayPets(headPtr);
            writePetsToFile(headPtr);
            flag = true;
        }

    } while (flag == false);

    deletePetsFunction(headPtr);

    removeRemainingPets(&headPtr);

    displayPets(headPtr);
    return 0;
}//-----------------------------------MAIN----------------------------------------------------------

char* custom_fgets(char* stringPtr, size_t maxNumOfChars, FILE* inputStream)
{

    //regular fgets with the new line character
    //Get the length of the input string and make sure
    char* input = fgets(stringPtr, SIZE, inputStream);
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


int compareStrings(const char* str1, const char* str2) {

    int result = 0;

    if(str1 != NULL && str2 != NULL)
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

void deletePetsFunction(Pet* headPtr)
{
    bool flag = false;
    bool petsLeft = true;

    while (flag == false && petsLeft ==true)
{

    puts("Would you like to delete a pet?");
    unsigned int addPet = askForInsert();

    if (addPet == 1)
    {
        char petToDelete[SIZE] = "";
        puts("enter pet name:");
        custom_fgets(petToDelete, SIZE, stdin);

        petsLeft = deletePet(&headPtr, petToDelete);

    }
    if (addPet == 0)
    {
        displayPets(headPtr);
        flag = true;
        petsLeft = false;
    }
}

}//-----------------------------------Insert Pets function---------------------------


int askForInsert()
{

    char inputStr[SIZE] = "";
    bool gotValid = false;
    int result = 0;


    printf("%s\n", "Do you want to insert another pet? (y)es or (n)o?");
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
}//------------------------------------end askForReceipt-----------------------------------------

Pet* createPet(Pet** headPtr, unsigned int* numOfPets)
{
    Pet *newAnimal = malloc(sizeof(Pet));

    if (newAnimal != NULL)
    {
        printf("%s\n", "Enter the name of the pet: ");
        custom_fgets(newAnimal->name, SIZE, stdin);

        printf("%s\n", "Enter the age of the pet: ");
        scanf("%d", &newAnimal->age);

        puts("");
        newAnimal->nextPetPtr = NULL;

        Pet* previousPetPtr = NULL;

        Pet* currentPetPtr = *headPtr;

        while (currentPetPtr != NULL && compareStrings(newAnimal->name, currentPetPtr->name) == 1)
        {
            previousPetPtr = currentPetPtr;

            currentPetPtr = currentPetPtr->nextPetPtr;
        }

        if (previousPetPtr == NULL)
        {
            *headPtr = newAnimal;
        }
        else
        {
            previousPetPtr->nextPetPtr = newAnimal;
        }

        newAnimal->nextPetPtr = currentPetPtr;
    }
    else
    {
        printf("%s\n", "No memory to create node");
    }

    return newAnimal;
}//------------------------------------------------CREATE PET METHOD---------------------------------------

void displayPets(Pet* headPtr) 
{
    if(headPtr == NULL)
    {
        puts("No pets in list");
    }
    else 
    {
        Pet* currentPet = NULL;
        currentPet = headPtr;
        while (currentPet != NULL)
        {
            printf("%s%s%d\n", currentPet->name, " == ", currentPet->age);
            currentPet = currentPet->nextPetPtr;
        }
        puts("NULL");
    }
}

void writePetsToFile(Pet *headPtr) 
{
    FILE* pfPtr;

    puts("");
    // fopen opens file. Exit program if unable to create file 
    if ((pfPtr = fopen("C:\\CS2060Files\\pets.txt", "w")) == NULL) {
        puts("File could not be opened");
    }
    else{

        puts("wrote in file");
        Pet* currentPet = NULL;
        currentPet = headPtr;
        while (currentPet != NULL)
        {
            fprintf(pfPtr, "%s%s%d\n", currentPet->name, " == ", currentPet->age);
            currentPet = currentPet->nextPetPtr;
        }

        fclose(pfPtr);
    }

}

bool deletePet(Pet** headPtr, char* petToDelete)
{
    
    Pet* previousPtr = NULL;
    Pet* currentPtr = *headPtr;

    bool petsLeft = true;

    
    if (*headPtr != NULL)
    {
        if (compareStrings((*headPtr)-> name, petToDelete ) == 0)
        {

            *headPtr = (*headPtr)->nextPetPtr;

            free(currentPtr);
            currentPtr = NULL;
        }
        else 
        {
         
            while (currentPtr != NULL && compareStrings(currentPtr->name, petToDelete) !=0)
            {
             
                previousPtr = currentPtr;
                currentPtr = currentPtr->nextPetPtr;
            }

     	
            if (currentPtr != NULL)
            {
        
                previousPtr -> nextPetPtr = currentPtr->nextPetPtr;
                free(currentPtr);
                currentPtr = NULL;
            }
            else
            {
                puts("pet to delete not found");
            }

            if (headPtr == NULL) 
            {
                puts("No more pets in the list");
                petsLeft = false;
            }
        }
    }
    else 
    {
        puts("There aren't any pets in the list");
    }

    return petsLeft;
} //deletePet

void removeRemainingPets(Pet** headPtr) 
{
    Pet* currentPtr = *headPtr;
    Pet* nextNodePtr = NULL;

    while (currentPtr != NULL)
    {
        nextNodePtr = currentPtr->nextPetPtr;
        free(currentPtr);
        currentPtr = nextNodePtr;
    }

    *headPtr = NULL;

}