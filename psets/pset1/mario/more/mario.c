#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int h;

    do                                         //prompts the user for the height
    {
        h = get_int("Height: ");
    }
    while (h <= 0 || h > 8);

    for (int i = 1; i <= h; i++)               //prints the pyramid
    {
        for (int s = 0; s < h - i; s++)        //prints the left spaces
        {
            printf(" ");
        }

        for (int p = 0; p < i; p++)            //prints the left hashes
        {
            printf("#");
        }

        printf("  ");                          //prints the gap spaces

        for (int p = 0; p < i; p++)            //prints the right hashes
        {
            printf("#");
        }

        printf("\n");                          //changes the line
    }
}