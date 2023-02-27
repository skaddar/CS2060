/*******************
 Understanding pointers

 Add comments to explain the code
**************/

#include <stdio.h>


int main ()
{
	// Initalize houseNum
	int houseNum = 13;
	int calcHouseNum1 = 0;
	int calcHouseNum2 = 0;
	//initialize the houseNumPtr that points to the address of houseNum
	int *houseNumPtr = &houseNum;
	
	//print the value of houseNum (13)
	printf("houseNum %d\n", houseNum);

	//print the address of houseNum in memory, using the address operator
	printf("&houseNum %p\n\n", &houseNum);

	//print the value stored in the address that houseNumPtr is pointing to (houseNum) (13)
	printf ("*houseNumPtr  %d\n", *houseNumPtr);

	//print the address that houseNumPtr is pointing to, Not the value stored inside the address
	printf ("houseNumPtr %p\n\n", houseNumPtr);

	//print the address of the pointer
	//This is different from the address that it is pointing to
	printf ("&houseNumPtr %p\n\n", &houseNumPtr);

	//*houseNumPtr is the value stored in houseNum not an address
	//This is how we can make calculations using pointers without using the variables that store the value
	calcHouseNum1 =  *houseNumPtr + *houseNumPtr;

	//Will print 26 because the * operator is used so its storing the value that is stored in the address
	//it is pointing to
	printf("*houseNumPtr + *houseNumPtr is %d\n\n", calcHouseNum1);

	//because the multiplication operator is the same as the pointer operator
	//using parenthesis allows for both to be used 
	calcHouseNum2 = 2 * (*houseNumPtr);
	printf("2 * (*houseNumPtr) is %d\n\n", calcHouseNum2);

	//both of these will print the address that the houseNumPtr is pointing to
	//the sequence of operators did not change anything
	printf("\n&*houseNumPtr = %p"
		"\n*&houseNumPtr = %p\n", &*houseNumPtr, *&houseNumPtr);
			
	return 0;
}
