#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int main(int argc, string argv[])
{
    // error message for missing key
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    // error message for incomplete key
    if (strlen(argv[1]) != 26)
    {
        printf("The key must contain 26 characters.\n");
        return 1;
    }

    // error message for invalid or duplicated characters
    for (int i = 0; i < strlen(argv[1]); i++)
    {
        if (isalpha(argv[1][i]) == false)
        {
            printf("The key must only contain letters.\n");
            return 1;
        }

        for (int j = i + 1; j < strlen(argv[1]); j++)
        {
            if (argv[1][i] == argv[1][j])
            {
                printf("The key must not contain repeated letters.\n");
                return 1;
            }
        }
    }

    // converts key to lower case
    char keylower[26];
    for (int i = 0; i < 26; i++)
    {
        keylower[i] = tolower(argv[1][i]);
    }

    // converts key to upper case
    char keyupper[26];
    for (int i = 0; i < 26; i++)
    {
        keyupper[i] = toupper(argv[1][i]);
    }

    //prompts the user for the plaintext and makes a copy for the ciphertext
    string pt = get_string("plaintext: ");
    string ct = pt;

    // standard alphabet for substitution
    string lower = "abcdefghijklmnopqrstuvwxyz";
    string upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    // checks plaintext character by character to substitute them for the key
    for (int i = 0; i < strlen(pt); i++)
    {
        if (islower(pt[i]))
        {
            for (int j = 0; j < 26; j++)
            {
                if (pt[i] == lower[j])
                {
                    ct[i] = keylower[j];
                    j = 26;
                }
            }
        }
        else if (isupper(pt[i]))
        {
            for (int j = 0; j < 26; j++)
            {
                if (pt[i] == upper[j])
                {
                    ct[i] = keyupper[j];
                    j = 26;
                }
            }
        }
    }

    // prints the ciphertext
    printf("ciphertext: %s\n", ct);
    return 0;
}