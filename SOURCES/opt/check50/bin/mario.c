
#include <stdio.h>
#include <cs50.h>

int main(int argc, char * argv[])
{
    int numRows, row, numSpaces, starCount, spaceCount;
    
    do
    {
        printf("Enter a non-negative integer < 24: ");  
        numRows = GetInt();
    } while(numRows > 23 || numRows < 0);

    /* pad spaces */
    for(row = 1; row <= numRows; row++)
    {
        /* number of spaces to print:
           it's 80 - number of stars to print,
           which is row # + 1. */
           
        numSpaces = numRows - row;
        
        for(spaceCount = 0; spaceCount < numSpaces; spaceCount++)
        {
            printf(" ");
        }
        
        /* print one more "*" than the row # we're on */

        for(starCount = 0; starCount <= row; starCount++)
        {
            printf("#");
        }
        printf("\n"); // move to next row
    }
}
