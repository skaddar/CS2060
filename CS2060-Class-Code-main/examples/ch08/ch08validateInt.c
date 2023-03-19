
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define LENGTH 13

void  exploreValidateInt(const char* buff);
bool validateInt(char* buff, int* const validInt);
void printLimits();

int main(void)
{
	char inputStr[LENGTH]; // create char arintray
	size_t inputLength = 0;

	printLimits();

	for (unsigned int counter = 1; counter < 6; counter++)
	{
		puts("\nEnter an integer");
		fgets(inputStr, LENGTH, stdin);
		//Used this to clear the buffer
		while ((getchar()) != '\n');

		inputLength = strnlen(inputStr, LENGTH);

		if (inputLength > 0 && inputStr[inputLength - 1] == '\n')
		{
			inputStr[inputLength - 1] = '\0';
			inputLength--;
		}
		exploreValidateInt(inputStr);
	}

}



void printLimits()
{
	printf("The number of bits in a byte %d\n", CHAR_BIT);

	printf("The minimum value of SIGNED CHAR = %d\n", SCHAR_MIN);
	printf("The maximum value of SIGNED CHAR = %d\n", SCHAR_MAX);
	printf("The maximum value of UNSIGNED CHAR = %d\n", UCHAR_MAX);

	printf("The minimum value of SHORT INT = %d\n", SHRT_MIN);
	printf("The maximum value of SHORT INT = %d\n", SHRT_MAX);

	printf("The minimum value of INT = %d\n", INT_MIN);
	printf("The maximum value of INT = %d\n", INT_MAX);

	printf("The minimum value of CHAR = %d\n", CHAR_MIN);
	printf("The maximum value of CHAR = %d\n", CHAR_MAX);

	printf("The minimum value of LONG = %ld\n", LONG_MIN);
	printf("The maximum value of LONG = %ld\n", LONG_MAX);
}


bool  exploreValidateInt(const char* buff)
{
	//The address of this pointer will be passed to the strtol() function to verify that all chars were converted
	//If it stores the null character then all chars have been read
	char* end;

	//errno set to 0 if the function goes out of bounds then it is set to ERANGE
	errno = 0;

	//This will store the long casted into an integer
	int validInt = 0;

	//Read in the buff string and store into a long with base 10
	long intTest = strtol(buff, &end, 10);
	
	//If the end is equal to buff that means nothing was converted so the end pointer did not move
	//which means numbers were not inputed
	if (end == buff) {
		fprintf(stderr, "%s: not a decimal number\n", buff);
	}

	//If end is not null that means not all of the characters inputed were numbers
	else if ('\0' != *end) {
		fprintf(stderr, "%s: extra characters at end of input: %s\n", buff, end);
	}

	//This is when the long is less than the minimum number of values for a long or higher than the maximum
	//This will result with an ERANGE error which will be stored in errno so if bothe os those conditions
	//are true that means the number is out of bounds
	else if ((LONG_MIN == intTest || LONG_MAX == intTest) && ERANGE == errno) {
		fprintf(stderr, "%s out of range of type long\n", buff);
	}

	//The long cannot be larger than an int or else data will be lost during casting or just an error
	else if (intTest > INT_MAX) {
		fprintf(stderr, "%ld greater than INT_MAX\n", intTest);
	}
	//same reason why long cannot be smaller than the minimum for an int
	else if (intTest < INT_MIN) {
		fprintf(stderr, "%ld less than INT_MIN\n", intTest);
	}
	else {
		//the long is explicitly casted to an int and stored in validInt
		validInt = (int)intTest;
		printf("%d is integer value ", validInt);
	}
}
