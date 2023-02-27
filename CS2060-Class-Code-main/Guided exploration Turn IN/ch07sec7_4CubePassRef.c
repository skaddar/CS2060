// Passing Primitive Data Types and pointers
//Add comments to explain the code

#include <stdio.h>	
#include <stdlib.h>

int cubePass1(int number);
int cubePass2(int * numberPtr);

int main(void) {

	int numberMain = 5;
	int result = 0;
	
	//print the value of numberMain and its address
	printf ("In main before cubePass1  numberMain = %d\n", numberMain);
	printf("&numberMain = %p\n", &numberMain);
	//call the cubePass1 method and send it numberMain
	result = cubePass1(numberMain);
	//print the result of cubePass1
	printf ("In main after cubePass1  numberMain = %d\n", numberMain);
	printf ("Result = %d\n", result);
	printf("\nIn main before cubePass2  numberMain = %d\n", numberMain);
	result = cubePass2(&numberMain);
	printf("\nIn main after cubePass2  numberMain = %d\n", numberMain);
	printf("result = %d\n", result);

} // main

//Function does not use a pointer so it is passed by value 
//That is why this function must return the updated value and it must be set to that in main
//while if it was a pointer then it would change the value in memory and not just this function
int cubePass1 (int number) 
{
	int cube = 0;
	puts("\nIn cubePass1");
	printf("number = %d\n", number);
	printf("&number = %p\n", &number);
	cube = number * number * number;
	printf("cube  = %d\n", cube);
	number = cube;
	printf("number = %d\n", number);
	return cube;
} 

//The value the pointer is referring to cannot be changed
int cubePass2 (int * numberPtr) 
{
	/*
	int cube = 0;
	puts ("\nIn cubePass2");
	printf("numberPtr = %p\n", numberPtr);
	printf ("*numberPtr = %d\n", *numberPtr);
	printf("&numberPtr = %p\n", &numberPtr);
	cube = (* numberPtr )* (* numberPtr )* (* numberPtr); 
	*numberPtr = cube;
	printf ("*numberPtr = %d\n", *numberPtr);
	return cube;
	*/
	//updated code
	printf("   *numberPtr = %d\n", *numberPtr);
	printf("   numberPtr = %p\n", numberPtr);
	int cube = (*numberPtr) * (*numberPtr) * (*numberPtr);
	*numberPtr = cube;
	numberPtr = &cube;
	//tried to derefrence the pointer to an address
	//it printed out junk i beleive not even an address
	*numberPtr = &cube;
	printf("   *numberPtr = %d\n\n", *numberPtr);
	return cube;
	

} 


