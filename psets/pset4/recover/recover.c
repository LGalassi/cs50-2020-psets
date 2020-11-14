#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // check for correct input
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    // check for valid file
    FILE *mcard = fopen(argv[1], "r");
    if (mcard == NULL)
    {
        printf("The file cannot be opened.\n");
        return 1;
    }

    BYTE chunk[512];
    int filenumber = -1;
    char *filename = malloc(8 * sizeof(char));

    // looping through the memory card's blocks
    while (fread(chunk, sizeof(BYTE), 512, mcard) == 512)
    {
        // if start of a new JPEG
        if (chunk[0] == 0xff && chunk[1] == 0xd8 && chunk[2] == 0xff && (chunk[3] & 0xf0) == 0xe0)
        {
            // if first JPEG
            if (filenumber < 0)
            {
                filenumber++;
                sprintf(filename, "%03i.jpg", filenumber);
                FILE *img = fopen(filename, "w");
                fwrite(chunk, sizeof(BYTE), 512, img);
                fclose(img);
            }
            // if another JPEG
            else
            {
                filenumber++;
                sprintf(filename, "%03i.jpg", filenumber);
                FILE *img = fopen(filename, "w");
                fwrite(chunk, sizeof(BYTE), 512, img);
                fclose(img);
            }
        }
        // if not a JPEG header
        else
        {
            // if currently recovering a JPEG
            if (filenumber >= 0)
            {
                FILE *img = fopen(filename, "a");
                fwrite(chunk, sizeof(BYTE), 512, img);
                fclose(img);
            }
        }
    }

    // free memory and close files
    free(filename);
    fclose(mcard);
}
