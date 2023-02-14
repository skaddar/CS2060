#include <stdio.h>
#include <stdbool.h>

double getValidHours();

const double MIN_FLAT_FEE = 3.00;
const unsigned int FLAT_RATE_MAX_HOURS = 3;
const double HOURLY_CHARGE = 0.75;
const double MAX_CHARGE = 12.00;

int main(void)
{
	bool exitParking = false;
	
	do
	{
		puts("Welcome to the parking Garage!");
		puts("How long will you park?");

		double validHours = getValidHours();

		printf("%lf", validHours);

	} while (exitParking == false);

}

double getValidHours() 
{
	double hoursInput = 0;
	int scannedVariable = 0;

	bool aquiredValidHours = false;

	do
	{
		puts("Please enter a valid Number of hours between 0-24");

		scannedVariable = scanf("%lf", &hoursInput);
		while (getchar() != '\n');

		if (scannedVariable == 1) 
		{
			if (hoursInput > 0 && hoursInput <= 24)
			{
				aquiredValidHours = true;
			}
			else 
			{
				puts("Error: invalid Input. Try Again.");
			}
		}
	
	} while (aquiredValidHours == false);

	return hoursInput;
}
