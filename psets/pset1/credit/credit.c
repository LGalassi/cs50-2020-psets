#include <stdio.h>
#include <cs50.h>

int main(void)
{
    long card = get_long("Number: ");     //prompts the user for the credit card number

    int n = 1;
    long i;

    for (i = 10; card > i; i *= 10)     //finds the number of digits
    {
        n++;
    }

    int a = 0;

    for (long j = 100; j <= i; j *= 100)     //calculates the first part of the checksum
    {
        a += (2 * ((card % j) / (j / 10)) / 10) + (2 * ((card % j) / (j / 10)) % 10);
    }

    int b = 0;

    for (long j = 10; j <= i; j *= 100)     //calculates the second part of the checksum
    {
        b += ((card % j) / (j / 10));
    }

    if ((a + b) % 10 == 0)     //checks if it's valid
    {
        if (n == 15)     //checks for AMEX
        {
            if (card / (i / 10) == 3)     //checks the first digit
            {
                if ((card % (i / 10)) / (i / 100) == 4 || (card % (i / 10)) / (i / 100) == 7)     //checks the second digit
                {
                    printf("AMEX\n");
                }
                else
                {
                    printf("INVALID\n");
                }
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else if (n == 13)     //checks
        {
            if (card / (i / 10) == 4)
            {
                printf("VISA\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else if (n == 16)
        {
            if (card / (i / 10) == 4)
            {
                printf("VISA\n");
            }
            else if (card / (i / 10) == 5)
            {
                if ((card % (i / 10)) / (i / 100) >= 1 && (card % (i / 10)) / (i / 100) <=5)
                {
                    printf("MASTERCARD\n");
                }
                else
                {
                    printf("INVALID\n");
                }
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}