// David J. Malan <malan@harvard.edu>

#include <cs50.h>
#include <stdio.h>

int
main(void)
{
    // ask for number of days in month
    int days = 0;
    do
    {
        printf("Days in month: ");
        days = GetInt();
    }
    while (days < 28 || 31 < days);

    // ask for number of first-day pennies
    long long pennies = 0;
    do
    {
        printf("Pennies on first day: ");
        pennies = GetLongLong();
    }
    while (pennies <= 0);

    // acculumate pennies
    long long total = pennies;
    for (int day = 2; day <= days; day++)
    {
        pennies *= 2;
        total += pennies;
    }

    // report total
    printf("$%.2f\n", total / 100.00);
}
