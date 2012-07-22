//
// Uses a greedy algorithm to determine how many coins are needed
// to make some user-specified amount of change.
//

#include <cs50.h>
#include <math.h>
#include <stdio.h>

int 
main(void)
{
	// find out how much change is owed
	printf("O hai!  How much change is owed?\n");
	float change_f = GetFloat();
	
	// make sure that the amount makes sense
	while (change_f < 0)
	{
		printf("Um, yeah, how much change is owed?\n");
		change_f = GetFloat();
	}
	
	// convert floating point value to an integer
	// (represented the change owed, in cents).
	// We use round() so that floating point issues
	// don't cause some values (e.g. .01, .02) to
	// be converted incorrectly
	int change_i = round(100 * change_f);
	
	// we start out with zero coins used for change
	int coins = 0;
	
	// first, try using quarters (the largest amount)
	while (change_i >= 25)
	{
		// for each quarter, reduce the amount of change by 25 cents
		change_i -= 25;
		// increment the coins counter by one
		coins++;
	}
	
	// next, do the same thing for dimes
	while (change_i >= 10)
	{
		change_i -= 10;
		coins++;
	}
	
	// nickels next
	while (change_i >= 5)
	{
		change_i -= 5;
		coins++;
	}
	
	// lastly, use pennies until no more change is owed
	while (change_i >= 1)
	{
		change_i -= 1;
		coins++;
	}
	
	// print out the final answer
	printf("%d\n", coins);
}
