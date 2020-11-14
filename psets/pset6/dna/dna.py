from sys import argv, exit
import csv
# Imports modules


def main():
    if len(argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        exit(1)
    # Checks for correct input

    database = []

    with open(argv[1], "r") as db_file:
        db = csv.reader(db_file)

        for row in db:
            database.append(row)
    # Opens database file

    dna_file = open(argv[2])
    dna = dna_file.read()
    # Opens sequence file

    profile = ["name"]

    for column in database[0]:
        if column != database[0][0]:
            profile.append(str(count_sequence(column, dna)))
    # Creates a list with the STRs counts

    for row in database:
        profile[0] = row[0]

        if row != database[0]:
            if row == profile:
                print(row[0])
                exit(0)
    # Checks for a match in the database

    print("No match")
    exit(1)
# Main function


def count_sequence(target, sample):
    result = 0
    no_matches = 0
    matches = 0
    target_length = len(target)
    end = len(sample) + 1

    for j in range(target_length, end, 1):
        i = j - target_length

        if sample[i:j] == target:
            matches += 1
            no_matches = 0
        else:
            no_matches += 1

            if no_matches == target_length:
                if matches >= result:
                    result = matches
                    matches = 0
                else:
                    matches = 0

    return result
# Function for counting the STRs


main()
# Main function call