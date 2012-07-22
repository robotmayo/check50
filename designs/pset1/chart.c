// 
// Gabrielle Ehrlich
// David J. Malan
//

#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>

int 
main(void)
{
    int m4f, f4m, f4f, m4m;

    // M4M
    do
    {
        printf("M spotting F: "); 
        m4f = GetInt();
    }
    while (m4f < 0);

    // F4M
    do
    {
        printf("F spotting M: "); 
        f4m = GetInt();
    }
    while (f4m < 0);

    // F4F
    do
    {
        printf("F spotting F: "); 
        f4f = GetInt();
    }
    while (f4f < 0);

    // M4M
    do
    {
        printf("M spotting M: "); 
        m4m = GetInt();
    }
    while (m4m < 0);

    // calculate numbers of #'s
    int total = m4f + m4m + f4m + f4f;
    int mf = 0, fm = 0, ff = 0, mm = 0;
    if (total > 0)
    {
        mf = (double) m4f / total * 80;
        fm = (double) f4m / total * 80;
        ff = (double) f4f / total * 80;
        mm = (double) m4m / total * 80;
    }

    // print bar graph 
    printf("\nWho is Spotting Whom\n\n");
    printf("M spotting F\n");
    for (int i = 0; i < mf; i++)
        printf("#");
    printf("\nF spotting M\n");
    for (int i = 0; i < fm; i++)
        printf("#");
    printf("\nF spotting F\n");
    for (int i = 0; i < ff; i++)
        printf("#");
    printf("\nM spotting M\n");
    for (int i = 0; i < mm; i++)
        printf("#");
    printf("\n");
}
