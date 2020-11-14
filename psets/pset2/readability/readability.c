#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int count_letters(string a);     //function declaration
int count_words(string a);     //function declaration
int count_sentences(string a);     //function declaration

int main(void)
{
    string text = get_string("Text: ");     //prompts the user for the input

    int letters = count_letters(text);     //counts the letters

    int words = count_words(text);     //counts the words

    int sentences = count_sentences(text);     //counts the sentences

    float L = (float) letters / (float) words * 100;     //calculates the constant L

    float S = (float) sentences / (float) words * 100;     //calculates the constant S

    int index = round(0.0588 * L - 0.296 * S - 15.8);     //calculates the index

    if (index > 16)     //prints out the result
    {
        printf("Grade 16+\n");
    }
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}

int count_letters(string a)     //function definition
{
    int j = 0;

    for (int i = 0; i < strlen(a); i++)
    {
        if (isalpha(a[i]))
        {
            j++;
        }
    }
    return j;
}

int count_words(string a)     //function definition
{
    int j = 1;

    for (int i = 0; i < strlen(a); i++)
    {
        if (isspace(a[i]))
        {
            j++;
        }
    }
    return j;
}

int count_sentences(string a)     //function definition
{
    int j = 0;

    for (int i = 0; i < strlen(a); i++)
    {
        if (a[i] == '.' || a[i] == '!' || a[i] == '?')
        {
            j++;
        }
    }
    return j;
}