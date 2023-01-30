// Fig. 2.1: fig02_01.c
// A first program in C 
#include <stdio.h>


// function main begins program execution 
int main( void )
{
	unsigned int length = 0;
	unsigned int width = 0;
	unsigned int area = 0;

	puts("enter length:");
	
	scanf("%d", &length);

	puts("enter width: ");
	scanf("%d", &width);

	printf("%s %d \n", "the length is:", length);
	printf("%s %d \n", "the width is:", width);

	puts("----------------------------------------------------");

	area = length * width;
	printf("%s %d, %s %d, %s %d \n", "length:", length, "Width:", width, "and the area is", area);

	double average = ((double)length + (double) width)/0;
	printf("%s %.1f \n", "the average is:", average);

  

} // end function main 


/**************************************************************************
 * (C) Copyright 1992-2015 by Deitel & Associates, Inc. and               *
 * Pearson Education, Inc. All Rights Reserved.                           *
 *                                                                        *
 * DISCLAIMER: The authors and publisher of this book have used their     *
 * best efforts in preparing the book. These efforts include the          *
 * development, research, and testing of the theories and programs        *
 * to determine their effectiveness. The authors and publisher make       *
 * no warranty of any kind, expressed or implied, with regard to these    *
 * programs or to the documentation contained in these books. The authors *
 * and publisher shall not be liable in any event for incidental or       *
 * consequential damages in connection with, or arising out of, the       *
 * furnishing, performance, or use of these programs.                     *
 *************************************************************************/

