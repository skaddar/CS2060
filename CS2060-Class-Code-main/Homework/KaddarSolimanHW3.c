//
//
//name: Soliman Kaddar
//Class: CS2060 M/W
//Assignment: HW03
//Due Date: 16 February 2023
//
//Description:
// 
//This Program Will use modular code to create a maintainable solution which implements a parking garage program.
//This program will first prompt the user for an input for the number of hours they would like to park, a method will
//validate the input to avoid invalid inputs such as letter, negatives etc. Another method will calculate the charge 
//depending on the parking garages specifications for the minimum flat rate fee, the maximum hours for the flat rate fee,
// and so on
//
//


#include <stdio.h>
#include <stdbool.h>

//declare the methods used in the program
double getValidHours();
double calculateCharge(double);
void displayCar(int, double, double);
void displayGarageSummary(double, double, int);

//Global constants can be used in all of the methods
const double MIN_FLAT_FEE = 3.00;
const unsigned int FLAT_RATE_MAX_HOURS = 3;
const double HOURLY_CHARGE = 0.75;
const double MAX_CHARGE = 12.00;
const unsigned int MAX_HOURS_ALLOWED = 24;

int main(void)
{
	//These variables will be used to summarize the totals of the garage
	//They will each increment after each car
	//Boolean flag is to break out of the loop
	unsigned int carsInGarage = 0;
	double totalGarageCharge = 0;
	double totalGarageHours = 0;
	bool exitParking = false;
	
	do
	{
		puts("");
		puts("Welcome to the parking Garage!");
		puts("How long will you park?\n");

		//call the getValidHours method and store the returned double and store into validHours
		//validHours will already be validated so no more checkig is required in the calculate 
		//charge method or main.
		double validHours = getValidHours();
		double parkingCharge = 0;

		//Checking if the customer does not want to keep entering cars.
		//if the validHours is -1 turn the boolean into true to break out of the loop
		//If the valid hours is not -1 that means a car was added so increment the cars in the garage and
		//start calculating the fee
		if(validHours == -1)
		{
			exitParking = true;
		}
		else
		{
			carsInGarage++;
			//Call calculateCharge method and send the validHours
			parkingCharge = calculateCharge(validHours);
			//display the car
			displayCar(carsInGarage, validHours, parkingCharge);
			puts(" ");
		}

		//After charge is calculated, increment the total charge and hours in the whole garage
		totalGarageCharge = totalGarageCharge + parkingCharge;
		totalGarageHours = totalGarageHours + validHours;


	} while (exitParking == false);

	displayGarageSummary(totalGarageHours, totalGarageCharge, carsInGarage);
}//End Main

//method to prompt for user input and check that it is a valid.
//Nothing will be passed to this method and it will return the valid hours
double getValidHours() 
{
	double hoursInput = 0;
	int scannedVariable = 0;
	bool aquiredValidHours = false;

	//repeat until valid hours are entered
	do
	{
		printf("%s%i%s", "Please enter a valid Number of hours between 0-", MAX_HOURS_ALLOWED, " or enter - 1 to quit\n");
		//prompt for input and store in hoursInput variable.
		//The return of scanf will be stored in Scanned variable to check for invalid input
		scannedVariable = scanf("%lf", &hoursInput);
		//clear the buffer
		while ((getchar()) != '\n');

		//If scanf did read the input succesfully, then check if the input is in the valid range
			if (scannedVariable == 1)
			{
				//If the input is in the valid range, change the boolean flag and break out of the loop
				if (hoursInput > 0 && hoursInput <= MAX_HOURS_ALLOWED || hoursInput == -1)
				{
					aquiredValidHours = true;
				}
				else
				{
					puts("Invalid Input. Try Again.");
				}
			}
			else 
			{
				puts("A character is not a valid number of hours");
			}

		

		} while (aquiredValidHours == false);

	return hoursInput;

}//getValidInput

//Method that calculates the charge for parking based on how long they will be parked
//It will return a double for the charge in dollars, and the paramters are just the already
//validated hours so we will assume that  0 < hours <= however many hours the owner wants
double calculateCharge(double hours) 
{
	//if the valid hours is a decimal we want to round up so if the double is greater than the int
	//because casting to an int will get rid of the decimals and if there are any they we will round up
	if (hours > (int)hours)
	{
		hours = hours + 1;
	}
	
	double charge = 0;
	int hoursPastFlatRate = hours - FLAT_RATE_MAX_HOURS;
	double additionalCharges = HOURLY_CHARGE * hoursPastFlatRate;

	//if the hours are within the range for the flat fee only charge that fee
	if (hours <= FLAT_RATE_MAX_HOURS) 
	{
		charge = MIN_FLAT_FEE;
	}
	//if they are not then add the flat fee to the additional charges and store in the charge
	else 
	{
		charge = MIN_FLAT_FEE + additionalCharges;
	}
	//if the charge is greater than the macimum allowed charge then just change the charge to the maximum allowed charge
	if(charge > MAX_CHARGE)
	{
		charge = MAX_CHARGE;
	}
	return charge;

}
//printing an individual car info
void displayCar(int carsInGarage, double validHours, double parkingCharge) 
{
	puts("");
	printf("%s		%s		%s", "Car", "Hours", "Charge");
	puts("");
	printf("%i		%.2lf		$%.2lf\n", carsInGarage, validHours, parkingCharge);

}
//printing the garage info summary
void displayGarageSummary(double totalHours, double totalCharge, int carsInGarage)
{
	//account for if there were no cars parked
	if (carsInGarage == 0)
	{
		puts("");
		puts("There were no cars parked today");
	}
	else 
	{
		//we will add 1 to the total hours to acount for the -1 that will break out of the program
		puts("");
		puts("		Garage Summary");
		printf("%s		%s		%s", "Cars", "Total Hours", "Total Charge");
		puts("");
		printf("%i		%.2lf		$%.2lf\n", carsInGarage, totalHours + 1, totalCharge);
	}
} 