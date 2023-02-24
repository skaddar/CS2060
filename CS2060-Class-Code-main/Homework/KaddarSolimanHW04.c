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

#include <stdio.h>
#include <stdbool.h>

#define STUDENTS 4
#define GRADE_CATEGORIES 5
#define CATEGORIES "1. Learning Activity 2. Homework 3. Project 4. Midterm 5. Final "
const double GRADE_CATEGORY_WEIGHT[] = { 0.1, 0.3, 0.3, 0.15, .15 };
const int MAXIMUM_GRADE = 105;
const int MINIMUM_GRADE = 0;

double getValidGrade();
void printCategoriesWeights();
void addStudentGrades(double[][GRADE_CATEGORIES], size_t);
void displayStudentsGrades(const double[][GRADE_CATEGORIES], size_t);
void calculateFinalGrades(double[], const double[][GRADE_CATEGORIES], size_t);
void displayFinalGrades(const double[], size_t);
void calculateAverage(const double[], size_t);


int main(void)
{
	double studentGrades[STUDENTS][GRADE_CATEGORIES];
	double studentFinalGrades[STUDENTS];

	puts("This program will calculate the grades for these categories");
	printf("%s", CATEGORIES);
	//print the category weights
	//category weights array is a global constant meaning it wont need to be passed to 
	//the function.
	printCategoriesWeights();
	addStudentGrades(studentGrades, STUDENTS);
	displayStudentsGrades(studentGrades, STUDENTS);
	calculateFinalGrades(studentFinalGrades, studentGrades, STUDENTS);
	displayFinalGrades(studentFinalGrades, STUDENTS);
	calculateAverage(studentFinalGrades, STUDENTS);
}

void printCategoriesWeights()
{
	puts("\n");
	for (size_t i = 0; i < GRADE_CATEGORIES; i++)
	{
		printf("%s%zu%s", "Category[", i, "] =");
		printf("%.2lf \n", GRADE_CATEGORY_WEIGHT[i]);
	}
}//printCategoriesWeights

//method to add student grades to the 2D array this method will use the getValidGrade method to get 
//grades between the parameters.
void addStudentGrades(double studentGrades[][GRADE_CATEGORIES], size_t numOfStudents)
{
	printf("%s%s\n\n", "The correct order to enter grades for each student is:\n", CATEGORIES);
	for (size_t i = 0; i < numOfStudents; i++)
	{
		for (size_t j = 0; j < GRADE_CATEGORIES; j++)
		{
			printf("%s%zu%s%zu%s", "Enter the grade for each category for student ", i+1, " category ", j+1,":");
			double categoryGrade = getValidGrade();
			studentGrades[i][j] = categoryGrade;
		}
		puts("");
	}
}

double getValidGrade()
{
	double gradeInput = 0;
	int scannedVariable = 0;
	bool aquiredValidGrade = false;

	//repeat until valid grades are entered
	do
	{
		
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

//iterate and display the 2d array neatly
void displayStudentsGrades(const double studentGrades[][GRADE_CATEGORIES], size_t numOfStudents)
{
	puts("Grades for each student\n");
	for (size_t i = 0; i < numOfStudents; i++)
	{
		printf("%s%zu%s", "Student ", i+1, ": ");
		for (size_t j = 0; j < GRADE_CATEGORIES; j++)
		{
			printf("	%.2lf", studentGrades[i][j]);
		}
		puts("");
	}
	puts("");
}

//this method will calculate the final grades for each student and add them to an array
//the 2D array will be passed as a constant because the function will not need to modify it just 
//get the values in it.
//It will store the averages calculated from the 2D array in the 1D array
void calculateFinalGrades(double finalGrades[], const double studentGrades[][GRADE_CATEGORIES], size_t numOfStudents)
{
	//Multiply each category by the category weight constant.
	//then add all of them together for each category and thats the final grade
	//Add that to the array
	for (size_t i = 0; i < numOfStudents; i++)
	{
		double categoryGrade = 0;
		double finalGrade = 0;
		for (size_t j = 0; j < GRADE_CATEGORIES; j++)
		{
			categoryGrade = studentGrades[i][j] * GRADE_CATEGORY_WEIGHT[j];
			finalGrade = finalGrade + categoryGrade;
		}
		finalGrades[i] = finalGrade;
	}
}

//This method will check the final grades and assign a letter grade appropriatly
//array passed as a constant because it is not modified
void displayFinalGrades(const double finalGrades[], size_t numOfStudents)
{
	puts("Final grades for students, respectively:");
	for (size_t i = 0; i < numOfStudents; i++)
	{
		if (finalGrades[i]>=90)
		{
			printf("%s%zu%s	%.2lf%s\n", "Student ", i+1, " :", finalGrades[i], "	A");
		}
		else if (finalGrades[i] >= 80 && finalGrades[i] < 90)
		{
			printf("%s%zu%s	%.2lf%s\n", "Student ", i + 1, " :", finalGrades[i], "	B");
		}
		else if (finalGrades[i] >= 70 && finalGrades[i] < 80)
		{
			printf("%s%zu%s	%.2lf%s\n", "Student ", i + 1, " :", finalGrades[i], "	C");
		}
		else if (finalGrades[i] >= 60 && finalGrades[i] < 70)
		{
			printf("%s%zu%s	%.2lf%s\n", "Student ", i + 1, " :", finalGrades[i], "	D");
		}
		else if (finalGrades[i] <60)
		{
			printf("%s%zu%s	%.2lf%s\n", "Student ", i + 1, " :", finalGrades[i], "	F");
		}
	}
}

//This method will calculate the average by iterating and adding all of the final grades together 
//Then dividing them by the number of grades aka the number of students
void calculateAverage(const double finalGrades[], size_t numOfStudents)
{
	double sum = 0;
	for(size_t i = 0; i < numOfStudents; i++)
	{
		sum = sum + finalGrades[i];
	}
	double average = sum / numOfStudents;
	printf("%s%.2lf", "The class average = ", average);
}