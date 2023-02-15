#include <stdio.h>
#include <stdbool.h>

double getValidHours();
double calculateCharge(double);

const double MIN_FLAT_FEE = 3.00;
const unsigned int FLAT_RATE_MAX_HOURS = 3;
const double HOURLY_CHARGE = 0.75;
const double MAX_CHARGE = 12.00;

int main(void)
{
	unsigned int carsInGarage = 0;
	bool exitParking = false;
	
	do
	{
		puts("Welcome to the parking Garage!");
		puts("How long will you park?\n");

		double validHours = getValidHours();

		if(validHours == -1)
		{
			exitParking = true;
		}
		else
		{
			carsInGarage++;
			double parkingCharge = calculateCharge(validHours);
			printf("%s %lf","the charge is : ", parkingCharge);
			puts(" ");
		}


	} while (exitParking == false);

}//End Main

//method to prompt for user input and check that it is a valid value and return the 
double getValidHours() 
{
	double hoursInput = 0;
	int scannedVariable = 0;

	bool aquiredValidHours = false;

	do
	{
		puts("Please enter a valid Number of hours between 0-24, or enter -1 to quit");

		scannedVariable = scanf("%lf", &hoursInput);
		while ((getchar()) != '\n');


			if (scannedVariable == 1)
			{
				if (hoursInput > 0 && hoursInput <= 24 || hoursInput == -1)
				{
					aquiredValidHours = true;
				}
				else
				{
					puts("Invalid Input. Try Again.");
				}
			}
		

		} while (aquiredValidHours == false);

	return hoursInput;

}//getValidInput

//Method that calculates the charge for parking based on how long they will be parked
//It will return a double for the charge in dollars, and the paramters are just the already
//validated hours so we will assume that  0 < hours <=24
double calculateCharge(double hours) 
{
	if (hours > (int)hours)
	{
		hours = hours + 1;
	}

	double charge = 0;
	int hoursPastFlatRate = hours - FLAT_RATE_MAX_HOURS;
	double additionalCharges = HOURLY_CHARGE * hoursPastFlatRate;

	if (hours <= FLAT_RATE_MAX_HOURS) 
	{
		charge = MIN_FLAT_FEE;
	}
	else 
	{
		charge = MIN_FLAT_FEE + additionalCharges;
	}
	return charge;

}