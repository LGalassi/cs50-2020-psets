from cs50 import get_string


def main():
    # Prompts the user for the input
    text = get_string("Text: ")

    # Counts the letters
    letters = count_letters(text)

    # Counts the words
    words = count_words(text)

    # Counts the sentences
    sentences = count_sentences(text)

    # Calculates the constant L
    l = letters / words * 100

    # Calculates the constant S
    s = sentences / words * 100

    # Calculates the index
    index = round(0.0588 * l - 0.296 * s - 15.8)

    # Prints out the result
    if index > 16:
        print("Grade 16+")
    elif index < 1:
        print("Before Grade 1")
    else:
        print(f"Grade {index}")


def count_letters(a):
    counter = 0

    for i in a:
        if i.isalpha() == True:
            counter += 1

    return counter

# Function definition


def count_words(a):
    counter = 1

    for i in a:
        if i.isspace() == True:
            counter += 1

    return counter

# Function definition


def count_sentences(a):
    counter = 0

    for i in a:
        if i == '.' or i == '!' or i == '?':
            counter += 1

    return counter

# Function definition


main()