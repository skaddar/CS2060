//
// Name: Soliman Kaddar
// Class: CS2060 (M/W)
// Homework 4
// Due: 23 February 2023
// 
// Description: Write a program to simulate how canvas calculates your grades using 
// weights and reports the class average. The program will use one demensional and multi demensional
// arrays to store and calculate grades for students.
// 
// 
// 
//

#include <stdio.h>
#include <stdbool.h>

double getValidHours();
void printCategoriesWeights();


#define STUDENTS 4
#define GRADE_CATEGORIES 5
#define CATEGORIES "1. Learning Activity 2. Homework 3. Project 4. Midterm 5. Final "
const double GRADE_CATEGORY_WEIGHT[] = { 0.1, 0.3, 0.3, 0.15, .15 };
const int MAXIMUM_GRADE = 105;
const int MINIMUM_GRADE = 0;

int main(void)
{

	puts("This program will calculate the grades for these categories");
	printf("%s", CATEGORIES);
	//print the category weights
	//category weights array is a global constant meaning it wont need to be passed to 
	//the function.
	printCategoriesWeights();

	double studentGrades[STUDENTS][GRADE_CATEGORIES];
	
}

void printCategoriesWeights()
{
	puts("\n");
	for (int i = 0; i < GRADE_CATEGORIES; i++) 
	{
		printf("%s%i%s", "Category[", i, "] =");
		printf("%.2lf \n", GRADE_CATEGORY_WEIGHT[i]);
	}
}//printCategoriesWeights

void addStudentGrades(double studentGrades[][GRADE_CATEGORIES])
{
}

double getValidHours()
{
	double gradeInput = 0;
	int scannedVariable = 0;
	bool aquiredValidGrade = false;

	//repeat until valid grades are entered
	do
	{
		printf("%s%i%s%i%s", "Please enter a valid grade between",MINIMUM_GRADE,"-", MAXIMUM_GRADE, "\n");
		//prompt for input and store in gradeInput variable.
		//The return of scanf will be stored in Scanned variable to check for invalid input
		scannedVariable = scanf("%lf", &gradeInput);
		//clear the buffer
		while ((getchar()) != '\n');

		//If scanf did read the input succesfully, then check if the input is in the valid range
		if (scannedVariable == 1)
		{
			//If the input is in the valid range, change the boolean flag and break out of the loop
			if (gradeInput >= MINIMUM_GRADE && gradeInput <= MAXIMUM_GRADE)
			{
				aquiredValidGrade = true;
			}
			else
			{
				puts("Invalid Grade. Try Again.");
			}
		}
		else
		{
			puts("A character is not a valid number for Grades");
		}

	} while (aquiredValidGrade == false);

	return gradeInput;

}//getValidInput